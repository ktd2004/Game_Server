#include "stdafx.h"
#include <Application.h>
#include "AAAConst.h"
#include "SessionMgr.h"
#include "ChannelMgr.h"
#include "AAA2ClientProtocol.h"
#include "Gateway2MasterHandler.h"
#include "Gateway2ClientHandler.h"

Gateway2ClientHandler::Gateway2ClientHandler( uint32 iTaskID )
	: NetEventHandler(iTaskID)
	, m_pMasterHandler(nullptr)
	, m_iGID(0)
	, m_iMID(0)
	, m_iPriority(0)
	, m_iThreadCnt(1)
{
	SetAcceptedFunc( this, &Gateway2ClientHandler::OnAccepted );
	SetClosedFunc  ( this, &Gateway2ClientHandler::OnClosed );
	SetReceivedFunc( this, &Gateway2ClientHandler::OnReceived );
}

Gateway2ClientHandler::~Gateway2ClientHandler()
{
}

bool Gateway2ClientHandler::OnAccepted( NetLinkPtr spLink )
{
	if( Application::GetBlockClient() )
	{
		// 접속 금지
		prn_dbg( "block client true, kick session" );
		SESSION_MGR()->Kick( spLink, eKICK_ADMIN, __FUNCTION__ );
		return false;
	}

	return true;
}

bool Gateway2ClientHandler::OnClosed( NetLinkPtr spLink )
{
	if( !spLink )
	{
		return false;
	}

	SessionPtr spSession = (Session*)spLink->UserData();
	if( !spSession )
	{
		return false;
	}

	m_pMasterHandler->OnClientLogoff(
		spSession->m_iMasterID,
		spSession->m_iChannelID,
		spSession->m_iUID,
		spSession->m_iAuthKey);

	prn_inf( "session is closed %s(%I64u)", cstr(spSession->GetAccount()), spSession->GetUID() );
	SESSION_MGR()->UnregAll( spSession->GetUID() );
	spLink->UserData(nullptr);
	return true;
}

bool Gateway2ClientHandler::Initialize()
{
	m_iGID				= Env_i( GTWS, "id", 1 );
	m_iPriority			= Env_i( GTWS, "client_priority", 1 );
	m_iThreadCnt		= Env_i( GTWS, "client_thread_cnt", 1 );
	m_sAddr				= Env_s( GTWS, "listen_addr", "localhost" );
	m_iPort				= Env_i( GTWS, "listen_port", 9001 );
	m_MinVersion		= Env_s( GTWS, "min_version", "0.0.0" );
	uint32 iMaxClient	= Env_i( GTWS, "max_client", 1000 );
	bool bBlock			= Env_b( GTWS, "block_client", false );
	std::string sMst	= Env_s( GTWS, "connect_master", MSTS );
	stringvector token;
	util::tokenizer(sMst, token, ',');
	if (token.size() != 0)
	{
		m_iMID = Env_i(token[0], "id", 1);
	}
	
	prn_sbj( "Gateway2ClientHandler" );
	prn_inf( "++ gateway id %u", m_iGID );
	prn_inf( "++ priority %u", m_iPriority );
	prn_inf( "++ threads %u", m_iThreadCnt );
	prn_inf( "++ max client %d", iMaxClient );
	prn_inf( "++ block client %s", bool2str(bBlock) );
	prn_inf( "++ listen addr %s:%u", cstr(m_sAddr), m_iPort );
	prn_inf( "++ minimum client version %s", cstr(m_MinVersion.ToString()) );
	prn_bar();

	Application::SetMaxClient( iMaxClient );
	Application::SetBlockClient( bBlock );

	return true;
}


bool Gateway2ClientHandler::StartupClientHandler()
{
	if( !Start(m_iPriority, m_iThreadCnt) )
	{
		prn_err( "%s", errmsg );
		return false;
	}

	if( !Listen(m_iPort, m_sAddr) )
	{
		prn_err( "%s", errmsg );
		return false;
	}

	SESSION_MGR()->StartKickTimer( m_iTaskSchedulerID );

	return true;
}


/////////////////////////////////////////////////////////////////////////////////////
void Gateway2ClientHandler::ProcessClientError(SessionPtr spSession, int32 eResult, int32 eKick, const char* szFunc)
{
	ProcessClientError(spSession->GetLink(), eResult, eKick, szFunc);
}

void Gateway2ClientHandler::ProcessClientError(NetLinkPtr spLink, int32 eResult, int32 eKick, const char* szFunc)
{
	rpc_ErrorToClient(spLink, eResult);
	if (eResult != 0)
	{
		SESSION_MGR()->Kick(spLink, eKick, szFunc);
	}
}

bool Gateway2ClientHandler::ProcessClientLogin( NetLinkPtr spLink, UID iUID, const std::string& sAccount )
{
	SessionPtr spSession = SESSION_MGR()->IsExists(iUID);
	if (!spSession)
	{
		spSession = SESSION_MGR()->Reg3A(iUID, spLink);
		spSession->SetAccount(sAccount);
		spSession->SetAuthTime();
		spLink->UserData(spSession,eSESSION_OBJ);
	}
	else
	{
		NetLinkPtr spDupLink = spSession->GetLink();
		SESSION_MGR()->UnregAll(spSession->GetUID());
		if (spDupLink)
		{
			spDupLink->UserData(nullptr);
			spDupLink->Kick(eKICK_DUPLICATED_SESSION, __FUNCTION__);
		}

		spSession = SESSION_MGR()->Reg3A(iUID, spLink);
		spSession->SetAccount(sAccount);
		spSession->SetAuthTime();
		spLink->UserData(spSession, eSESSION_OBJ);
	}

	if( spSession->GetMasterID() == 0 && spSession->GetChannelID() == 0 )
	{
		CID iCID = CHANNEL_MGR()->GetFewChannelID();
		if( iCID == null_id )
		{
			ProcessClientError(spSession, eLR_SYSTEM_ERROR, eKICK_UNKNOWN, __FUNCTION__);
			return false;
		}
		prn_inf("connect channel(%u), uid(%I64u)", spSession->GetChannelID(), spSession->GetUID());

		spSession->SetLoginedInfo( m_iGID, m_iMID, iCID );
	}

	// 클라의 인증 정보를 마스터에 전달한다
	if( !m_pMasterHandler->OnClientLogin(spSession) )
	{
		ProcessClientError(spSession, eLR_SYSTEM_ERROR, eKICK_INVALID_MASTER, __FUNCTION__);
		return false;
	}

	return true;
}

bool Gateway2ClientHandler::ProcessClientLogoff(SessionPtr spSession)
{
	if (!m_pMasterHandler->OnClientLogoff(
		spSession->m_iMasterID,
		spSession->m_iChannelID,
		spSession->m_iUID,
		spSession->m_iAuthKey))
	{
		ProcessClientError(spSession, eLR_SYSTEM_ERROR, eKICK_INVALID_MASTER, __FUNCTION__);
	}

	return true;
}

bool Gateway2ClientHandler::OnReceived( NetLinkPtr spLink, Buffer* pBuffer )
{
	Session* pSession = (Session*)spLink->UserData();
	if (pSession == nullptr || !pSession->IsAuthencation())
	{
		prn_err("client not authentication");
		return false;
	}

	Channel* pChannel = CHANNEL_MGR()->Get(pSession->GetChannelID());
	if (pChannel && pChannel->GetLink())
	{
		unsigned int iProcID = Packet::id(*pBuffer);
		pBuffer->rd_ptr(sizeof(Packet::header));
		Buffer* pBuf = new Buffer(sizeof(UID)+pBuffer->size()+256);
		Packet pk(pBuf);
		pk.id(iProcID);
		pk << pSession->m_iUID;
		pk << pBuffer;
		pChannel->GetLink()->Send(pk.get());
	}

	return false;
}