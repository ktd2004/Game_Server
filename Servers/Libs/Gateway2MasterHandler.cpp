#include "stdafx.h"
#include <Application.h>
#include "Gateway2MasterHandler.h"
#include "Gateway2ClientHandler.h"
#include "Gateway2ChannelHandler.h"
#include "MasterMgr.h"
#include "SessionMgr.h"
#include "ChannelMgr.h"

namespace
{
#define is_not_found_master_ret( link ) \
	MasterPtr spMaster = (Master*)link->UserData(); \
	if( !spMaster ) { \
		prn_err( "can't find master object" ); \
		return false; \
	}
}

Gateway2MasterHandler::Gateway2MasterHandler( uint32 iTaskID )
	: NetEventHandler(iTaskID)
	, m_iGID(0)
	, m_pClientHandler(nullptr)
	, m_iPriority(0)
	, m_iThreadCnt(1)
{
	SetConnectedFunc( this, &Gateway2MasterHandler::OnConnected );
	SetClosedFunc   ( this, &Gateway2MasterHandler::OnClosed );

	EXPORT_RPC_NM_V1( this, &Gateway2MasterHandler::OnMasterStatus,       const MasterStatusData& );
	EXPORT_RPC_NM_V2( this, &Gateway2MasterHandler::OnClientLoginResult,  int32, UID );
	EXPORT_RPC_NM_V4( this, &Gateway2MasterHandler::OnClientLogoffResult, int32, CID, UID, uint32 );
}

Gateway2MasterHandler::~Gateway2MasterHandler()
{
}

bool Gateway2MasterHandler::OnConnected( NetLinkPtr spLink )
{
	GatewayConnData r;
	r.m_iGID = m_iGID;
	prn_inf("master <- send gateway data, gid(%u)", m_iGID);
	return rpc_OnGatewayConnData(spLink, r);
}

bool Gateway2MasterHandler::OnClosed( NetLinkPtr spLink )
{
	auto it = m_ConnMap.find(spLink->GetAddr());
	if (it != m_ConnMap.end())
	{
		std::string sAddr = util::pairkey(spLink->GetAddr(), ":");
		uint32 iPort = stoint32(util::pairval(spLink->GetAddr(), ":"));
		prn_dbg("reconnect master %s:%d", cstr(sAddr), iPort);
		Connect(iPort, sAddr);
	}

	// Master 종료
	is_not_found_master_ret( spLink );

	MASTER_MGR()->Remove(spMaster->GetID());
	prn_inf("master -> master(%u) is closed", spMaster->GetID());

	spLink->UserData(nullptr);
	return true;
}

bool Gateway2MasterHandler::Initialize()
{
	m_iGID				= Env_i( GTWS, "id", 1 );
	m_iPriority			= Env_i( GTWS, "master_priority", 1 );
	m_iThreadCnt		= Env_i( GTWS, "master_thread_cnt", 1 );
	std::string	sMst	= Env_s( GTWS, "connect_master", MSTS );

	prn_sbj("Gateway2MasterHandler");
	prn_inf("++ priority %u", m_iPriority);
	prn_inf("++ threads %u", m_iThreadCnt);

	stringvector token;
	util::tokenizer(sMst, token, ',');
	for (auto it = token.begin(); it != token.end(); ++it)
	{
		MID iMID = Env_i(*it, "id", 1);
		std::string sAddr = Env_s(*it, "listen_addr", "localhost");
		uint32 iPort = Env_i(*it, "listen_port", 9000);
		prn_inf("++ master id %u", iMID);
		prn_inf("++ master addr %s:%u", cstr(sAddr), iPort);
		std::string s = sAddr + ":" + util::intstr(iPort);
		m_ConnMap.insert(ConnectionMap::value_type(s, iMID));
	}

	prn_bar();

	return true;
}

bool Gateway2MasterHandler::StartupMasterHandler()
{
	if( !Start(m_iPriority, m_iThreadCnt) )
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

bool Gateway2MasterHandler::OnMasterStatus( NetLinkPtr spLink, const MasterStatusData& r )
{
	MasterPtr spMaster = (Master*)spLink->UserData();
	if (!spMaster)
	{
		spMaster = MASTER_MGR()->Add(r.m_iMID, spLink);
		spLink->UserData(spMaster);
	}

	spMaster->SetBlock( r.m_bBlock );

	return true;
}

bool Gateway2MasterHandler::OnClientLogin( SessionPtr spSession )
{
	MasterPtr spMaster = MASTER_MGR()->Get(spSession->GetMasterID());
	if (!spMaster)
	{
		prn_err("can't find master object, master(%u)", spSession->GetMasterID());
		return false;
	}

	return rpc_OnClientLogin(spMaster->GetLink(), *(spSession.get()));
}

bool Gateway2MasterHandler::OnClientLoginResult( NetLinkPtr spLink, int32 iResult, UID iUID )
{
	is_not_found_master_ret( spLink );
	
	SessionPtr spSession = SESSION_MGR()->Get3A( iUID );
	if( !spSession )
	{
		prn_err( "master -> account is not registered, uid(%I64u)", iUID );
		return false;
	}

	switch( iResult )
	{
		// 로그인 성공
		case eSR_SUCCESS:
		{
			prn_inf("master -> login success, %s(%I64u)", cstr(spSession->GetAccount()), iUID);
			m_pChannelHandler->OnClientLogin(spSession);
		}
		break;
		// 중복 로그인
		case eSR_DUPLICATE:
		{
			prn_inf( "master -> login success, kick duplicated session %s(%I64u)", cstr(spSession->GetAccount()), iUID);
			m_pChannelHandler->OnClientLogin(spSession);
		}
		break;
		// 로그인 실패
		default:
		{
			prn_err("master -> login failed, %s(%I64u)", cstr(spSession->GetAccount()), iUID);
			m_pClientHandler->ProcessClientError(spSession, iResult, eKICK_SYSTEM_ERROR, __FUNCTION__);
		}
		return false;
	}
	
	return true;
}

bool Gateway2MasterHandler::OnClientLogoff(MID iMasterID, CID iChannelID, UID iUID, uint32 iAuthKey)
{
	MasterPtr spMaster = MASTER_MGR()->Get(iMasterID);
	if (!spMaster)
	{
		prn_err("can't find master object, master(%u)", iMasterID);
		return false;
	}

	return rpc_OnClientLogoff(spMaster->GetLink(), iChannelID, iUID, iAuthKey);
}

bool Gateway2MasterHandler::OnClientLogoffResult( NetLinkPtr spLink, int32 iResult, CID iChannelID, UID iUID, uint32 iAuthKey )
{
	is_not_found_master_ret(spLink);

	switch (iResult)
	{
		// 정상적으로 로그오프
		case eSL_SUCCESS:
		{
			prn_inf("master -> send logoff success, uid(%I64u)", iUID);
			m_pChannelHandler->OnClientLogoff(iChannelID, iUID, iAuthKey);
		}
		break;
		// 중복로그인 로그오프
		case eSL_DUPLICATE:
		{
			prn_inf("master -> send logoff success, kick duplicated session, uid(%I64u)", iUID);
			m_pChannelHandler->OnClientLogoff(iChannelID, iUID, iAuthKey);
		}
		break;
		// 클라이언트를 찾을 수 없음( 마스터에 없어도 채널에 전송한다 )
		case eSL_NOT_FOUND_UID:
		{
			prn_inf("master -> send logoff success, can't find uid(%I64u) in master", iUID);
			m_pChannelHandler->OnClientLogoff(iChannelID, iUID, iAuthKey);
		}
		break;
		// 인증키가 맞지 않아 로그오프 못시킴
		case eSL_INVALID_AUTHKEY:
		{
			prn_inf("master -> logoff failed, authkey mismatch, uid(%I64u)", iUID);
		}
		break;
		// 다른 게이트웨이에 있는 클라는 로그오프 못시킴
		case eSL_INVALID_GATEWAY:
		{
			prn_inf("master -> logoff failed, gateway id mismatch, uid(%I64u)", iUID);
		}
		break;
		default:
		{
			prn_inf("master -> logoff failed, result(%u) uid(%I64u)", iResult, iUID);
		}
		return false;
	}

	return true;
}
