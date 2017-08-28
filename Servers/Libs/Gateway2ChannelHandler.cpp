#include "stdafx.h"
#include <Application.h>
#include "SessionMgr.h"
#include "ChannelMgr.h"
#include "MasterMgr.h"
#include "AAAConst.h"
#include "Gateway2ChannelHandler.h"
#include "Gateway2ClientHandler.h"

Gateway2ChannelHandler::Gateway2ChannelHandler( uint32 iTaskID )
	: NetEventHandler(iTaskID)
	, m_pClientHandler(nullptr)
	, m_iGID(0)
	, m_iPriority(0)
	, m_iThreadCnt(1)
{
	SetConnectedFunc( this, &Gateway2ChannelHandler::OnConnected );
	SetClosedFunc   ( this, &Gateway2ChannelHandler::OnClosed );
	SetReceivedFunc ( this, &Gateway2ChannelHandler::OnReceived );

	EXPORT_RPC_NM_V1( this, &Gateway2ChannelHandler::OnChannelStatus,      const ChannelStatusData& );
	EXPORT_RPC_NM_V2( this, &Gateway2ChannelHandler::OnClientLoginResult,  int32, UID );
	EXPORT_RPC_NM_V3( this, &Gateway2ChannelHandler::OnClientLogoffResult, int32, UID, uint32 );
}

Gateway2ChannelHandler::~Gateway2ChannelHandler()
{
}

bool Gateway2ChannelHandler::OnConnected( NetLinkPtr spLink )
{
	GatewayConnData r;
	r.m_iGID = m_iGID;
	prn_inf("channel <- send gateway data, gid(%u)", m_iGID);
	return rpc_OnGatewayConnData(spLink, r);
}

bool Gateway2ChannelHandler::OnClosed( NetLinkPtr spLink )
{
	auto it = m_ConnMap.find(spLink->GetAddr());
	if (it != m_ConnMap.end())
	{
		std::string sAddr = util::pairkey(spLink->GetAddr(), ":");
		uint32 iPort = stoint32(util::pairval(spLink->GetAddr(), ":"));
		prn_dbg("reconnect channel %s:%d", cstr(sAddr), iPort);
		Connect(iPort, sAddr);
	}

	ChannelPtr spChannel = (Channel*)spLink->UserData();
	if (!spChannel)
	{
		return false;
	}
	prn_inf("channel -> channel(%u) is closed", spChannel->GetID());
	CHANNEL_MGR()->Remove(spChannel->GetID());
	spLink->UserData(nullptr);

	return true;
}

bool Gateway2ChannelHandler::Initialize()
{
	m_iGID					= Env_i( GTWS, "id", 1 );
	m_iPriority				= Env_i( GTWS, "channel_priority", 1 );
	m_iThreadCnt			= Env_i( GTWS, "channel_thread_cnt", 1 );
	std::string sChn		= Env_s( GTWS, "connect_channel", CHNS );
	
	prn_sbj("Gateway2ChannelHandler");
	prn_inf("++ priority %u", m_iPriority);
	prn_inf("++ threads %u", m_iThreadCnt);

	stringvector token;
	util::tokenizer(sChn, token, ',');
	for (auto it = token.begin(); it != token.end(); ++it)
	{
		CID iCID  = Env_i(*it, "id", 1);
		std::string sAddr = Env_s(*it, "listen_addr", "localhost");
		uint32 iPort = Env_i(*it, "listen_port", 9010);
		prn_inf("++ channel id %u", iCID);
		prn_inf("++ channel addr %s:%u", cstr(sAddr), iPort);
		std::string s = sAddr + ":" + util::intstr(iPort);
		m_ConnMap.insert( ConnectionMap::value_type(s,iCID) );
	}

	prn_bar();

	return true;
}

bool Gateway2ChannelHandler::StartupChannelHandler()
{
	if( !Start( m_iPriority, m_iThreadCnt ) )
	{
		prn_err( "%s", errmsg );
		return false;
	}

	for (auto it = m_ConnMap.begin(); it != m_ConnMap.end(); ++it)
	{
		std::string sAddr = util::pairkey(it->first, ":");
		uint32 iPort = stouint32(util::pairval(it->first, ":"));
		if (!Connect(iPort, sAddr))
		{
			prn_err("%s", errmsg);
			return false;
		}
	}

	return true;
}

bool Gateway2ChannelHandler::OnChannelStatus( NetLinkPtr spLink, const ChannelStatusData& r )
{
	ChannelPtr spChannel = (Channel*)spLink->UserData();
	if (!spChannel)
	{
		spChannel = CHANNEL_MGR()->Add(r.m_iCID, spLink);
		spLink->UserData(spChannel, eCHANNEL_OBJ);
	}
	spChannel->SetCurrentUser(r.m_iCurrentUser);
	spChannel->SetBlock(r.m_bBlock);
	spChannel->SetMaxClient(r.m_iMaxClient);

	CHANNEL_MGR()->UpdateLoadBalancing();

	prn_dbg("channel -> current user %u, max client %u, block client %s",
		r.m_iCurrentUser, r.m_iMaxClient, bool2str(r.m_bBlock));

	return true;
}

bool Gateway2ChannelHandler::OnClientLogin(SessionPtr spSession)
{
	ChannelPtr spChannel = CHANNEL_MGR()->Get(spSession->GetChannelID());
	if (!spChannel)
	{
		prn_err("can't find channel object, channel(%u)", spSession->GetChannelID());
		m_pClientHandler->ProcessClientError(spSession, eLR_SYSTEM_ERROR, eKICK_INVALID_CHANNEL, __FUNCTION__);
		return false;
	}

	return rpc_OnClientLogin(spChannel->GetLink(), *(spSession.get()));
}

bool Gateway2ChannelHandler::OnClientLoginResult(NetLinkPtr spLink, int32 iResult, UID iUID)
{
	SessionPtr spSession = SESSION_MGR()->Get3A(iUID);
	if (!spSession)
	{
		prn_err("channel -> account is not registered, uid(%I64u)", iUID);
		return false;
	}

	switch (iResult)
	{
		// 로그인 성공
		case eSR_SUCCESS:
		{
			prn_inf("channel -> login success, %s(%I64u)", cstr(spSession->GetAccount()), iUID);
			SESSION_MGR()->Authentication(iUID);
			m_pClientHandler->OnAuthentication(spSession);
		}
		break;
		// 중복 로그인
		case eSR_DUPLICATE:
		{
			prn_inf("channel -> login success, kick duplicated session %s(%I64u)", cstr(spSession->GetAccount()), iUID);
			SESSION_MGR()->Authentication(iUID);
			m_pClientHandler->OnAuthentication(spSession);
		}
		break;
		// 그외 에러 처리
		default:
		{
			prn_inf("channel -> login failed, %s(%I64u)", cstr(spSession->GetAccount()), iUID);
			m_pClientHandler->ProcessClientError(spSession, iResult, eKICK_SYSTEM_ERROR, __FUNCTION__);
		}
		return false;
	}

	return true;
}

bool Gateway2ChannelHandler::OnClientLogoff(CID iChannelID, UID iUID, uint32 iAuthKey)
{
	ChannelPtr spChannel = CHANNEL_MGR()->Get(iChannelID);
	if (!spChannel)
	{
		prn_err("can't find channel object, channel(%u)", iChannelID);
		SESSION_MGR()->CompareAuthKeyUnregAll(iUID, iAuthKey, eKICK_INVALID_CHANNEL, __FUNCTION__);
		return false;
	}

	return rpc_OnClientLogoff(spChannel->GetLink(), iUID, iAuthKey);
}

bool Gateway2ChannelHandler::OnClientLogoffResult(NetLinkPtr spLink, int32 iResult, UID iUID, uint32 iAuthKey)
{
	ChannelPtr spChannel = (Channel*)spLink->UserData();

	switch (iResult)
	{
		// 정상적으로 로그오프됨
		case eSL_SUCCESS:
		{
			prn_inf("channel -> logoff success, uid(%I64u)", iUID);
			SESSION_MGR()->CompareAuthKeyUnregAll(iUID, iAuthKey, eKICK_NORMAL, __FUNCTION__);
		}
		break;
// 		// 중복로그인
// 		case eSL_DUPLICATE:
// 		{
// 			prn_dbg("channel -> logoff success, kick duplicated session, uid(%I64u)", iUID);
// 			SESSION_MGR()->CompareAuthKeyUnregAll(iUID, iAuthKey, eKICK_DUPLICATED_SESSION, __FUNCTION__);
// 		}
// 		break;
		// 인증키가 맞지 않아 로그오프 못시킴
		case eSL_INVALID_AUTHKEY:
		{
			prn_inf("channel -> logoff failed, authkey mismatch, uid(%I64u)", iUID);
		}
		break;
		case eSL_NOT_FOUND_UID:
		{
			SessionPtr spSession = SESSION_MGR()->Get(iUID);
			if (spSession && spChannel)
			{
				if (spSession->GetChannelID() == spChannel->GetID())
				{
					SESSION_MGR()->Kick(spSession, eKICK_INVALID_ACCOUNT, __FUNCTION__);
				}
			}
		}
		break;
		default:
		{
			prn_inf("channel -> logoff failed, uid(%I64u)", iUID);
		}
		return false;
	}

	return true;
}

bool Gateway2ChannelHandler::OnReceived(NetLinkPtr spLink, Buffer* pBuffer)
{
	unsigned int iProcID = Packet::id(*pBuffer);
	pBuffer->rd_ptr(sizeof(Packet::header));
	UID iUID = 0; *pBuffer >> iUID;
	Session* pSession = SESSION_MGR()->Get(iUID);
	if (pSession != nullptr && pSession->GetLink())
	{
		Buffer* pBuf = new Buffer(pBuffer->size() + 256);
		Packet pk(pBuf);
		pk.id(iProcID);
		pk << pBuffer;
		return pSession->GetLink()->Send(pk.get());
	}

	return false;
}