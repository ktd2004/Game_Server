#include "stdafx.h"
#include <Application.h>
#include "MasterMgr.h"
#include "GatewayMgr.h"
#include "SessionMgr.h"
#include "AAAConst.h"
#include "AAA2ClientProtocol.h"
#include "Channel2GatewayHandler.h"

namespace
{
#define is_no_gateway_ret( link ) \
	GatewayPtr spGateway = (Gateway*)link->UserData(); \
	if( !spGateway ) { \
		prn_err( "can't find gateway object" ); \
		return false; \
	}

#define is_no_master_ret( link ) \
	MasterPtr spMaster = (Master*)link->UserData(); \
	if( !spMaster ) { \
		prn_err( "can't find master object" ); \
		return false; \
	}
}

Channel2GatewayHandler::Channel2GatewayHandler( uint32 iTaskID )
	: NetEventHandler(iTaskID)
	, m_iCID(0)
	, m_iPriority(0)
	, m_iThreadCnt(1)
{
	SetConnectedFunc( this, &Channel2GatewayHandler::OnConnected );
	SetClosedFunc   ( this, &Channel2GatewayHandler::OnClosed );

	EXPORT_RPC_NM_V1( this, &Channel2GatewayHandler::OnGatewayConnData,  const GatewayConnData& );
	EXPORT_RPC_NM_V1( this, &Channel2GatewayHandler::OnClientLogin,      const Session& );
	EXPORT_RPC_NM_V2( this, &Channel2GatewayHandler::OnClientLogoff,     UID, uint32 );

	EXPORT_RPC_NM_V1( this, &Channel2GatewayHandler::OnMasterStatus,     const MasterStatusData& );
}

Channel2GatewayHandler::~Channel2GatewayHandler()
{
}

bool Channel2GatewayHandler::OnConnected(NetLinkPtr spLink)
{
	ChannelConnData r;
	r.m_iCID = m_iCID;
	prn_inf("master <- send channel data, cid(%u)", m_iCID);
	return rpc_OnChannelConnData(spLink, r);
}

bool Channel2GatewayHandler::OnClosed( NetLinkPtr spLink )
{
	switch (spLink->UserDataType())
	{
		case eGATEWAY_OBJ:
		{
			// gateway 에 연결된 모든 클라 삭제 처리
			is_no_gateway_ret(spLink);
			GATEWAY_MGR()->Remove(spGateway->GetID());
			prn_inf("gateway -> gateway(%u) is closed", spGateway->GetID());
		}
		break;
		case eMASTER_OBJ:
		{
			auto it = m_ConnMap.find(spLink->GetAddr());
			if (it != m_ConnMap.end())
			{
				std::string sAddr = util::pairkey(spLink->GetAddr(), ":");
				uint32 iPort = stoint32(util::pairval(spLink->GetAddr(), ":"));
				prn_dbg("reconnect master %s:%d", cstr(sAddr), iPort);
				Connect(iPort, sAddr);
			}

			is_no_master_ret(spLink);
			MASTER_MGR()->Remove(spMaster->GetID());
			prn_inf("master -> master(%u) is closed", spMaster->GetID());
		}
		break;
	}
	
	spLink->UserData(nullptr);
	

	return true;
}

bool Channel2GatewayHandler::Initialize()
{
	m_iCID				= Env_i( CHNS, "id", 1 );
	m_iPriority			= Env_i( CHNS, "channel_priority", 1 );
	m_iThreadCnt		= Env_i( CHNS, "channel_thread_cnt", 1 );
	m_sAddr				= Env_s( CHNS, "listen_addr", "localhost" );
	m_iPort				= Env_i( CHNS, "listen_port", 9001 );
	uint32 iMaxClient	= Env_i( CHNS, "max_client", 1000 );
	bool bBlock			= Env_b( CHNS, "block_client", false );
	std::string	sMst	= Env_s( CHNS, "connect_master", MSTS );

	prn_sbj( "Channel2GatewayHandler" );
	prn_inf( "++ channel id %u", m_iCID );
	prn_inf( "++ priority %u", m_iPriority );
	prn_inf( "++ threads %u", m_iThreadCnt );
 	prn_inf( "++ max client %d", iMaxClient );
 	prn_inf( "++ block client %s", bool2str(bBlock) );
	prn_inf( "++ listen addr %s:%u", cstr(m_sAddr), m_iPort );

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

	Application::SetMaxClient( iMaxClient );
	Application::SetBlockClient( bBlock );

	return true;
}


bool Channel2GatewayHandler::StartupChannelHandler()
{
	if( !Start(m_iPriority, m_iThreadCnt) )
	{
		prn_err( "%s", errmsg );
		return false;
	}

	if ( !Listen(m_iPort, m_sAddr) )
	{
		prn_err("%s", errmsg);
		return false;
	}

// 	std::function<void(NetLinkPtr, bool)> func = [this](NetLinkPtr spLink, bool bResult) {
// 		if (bResult)
// 		{
// 			MasterPtr spMaster = MASTER_MGR()->Add(1, spLink);
// 			spLink->UserData(spMaster, eMASTER_OBJ);
// 		}
// 		else
// 		{
// 			MASTER_MGR()->Remove(1);
// 			spLink->UserData(nullptr);
// 		}
// 	};

	for (auto it = m_ConnMap.begin(); it != m_ConnMap.end(); ++it)
	{
		m_sAddr = util::pairkey(it->first, ":");
		m_iPort = stouint32(util::pairval(it->first, ":"));

//		if (!Connect(m_iPort, m_sAddr, 4096, &func))
		if (!Connect(m_iPort, m_sAddr))
		{
			prn_err("%s", errmsg);
			return false;
		}
	}

	return true;
}

bool Channel2GatewayHandler::OnGatewayConnData( NetLinkPtr spLink, const GatewayConnData& g )
{
	GatewayPtr spGateway = GATEWAY_MGR()->Add(g.m_iGID, spLink);
	if (spGateway)
	{
		prn_inf("gateway(%u) is accepted", g.m_iGID);
		spLink->UserData(spGateway, eGATEWAY_OBJ);
		ChannelStatusData r;
		r.m_iCID = m_iCID;
		r.m_iCurrentUser = GATEWAY_MGR()->GetSessionCount();
		r.m_iMaxClient = Application::GetMaxClient();
		r.m_bBlock = Application::GetBlockClient();
		return rpc_OnChannelStatus(spLink, r);
	}

	return false;
}

bool Channel2GatewayHandler::OnMasterStatus( NetLinkPtr spLink, const MasterStatusData& r )
{
	MasterPtr spMaster = (Master*)spLink->UserData();
	if (!spMaster)
	{
		spMaster = MASTER_MGR()->Add(r.m_iMID, spLink);
		spLink->UserData(spMaster, eMASTER_OBJ);
	}

	spMaster->SetBlock(r.m_bBlock);

	return true;
}

void Channel2GatewayHandler::OnUpdateChannelStatus( void )
{
	ChannelStatusData r;
	r.m_iCID = m_iCID;
	r.m_iCurrentUser = GATEWAY_MGR()->GetSessionCount();
	r.m_iMaxClient = Application::GetMaxClient();
	r.m_bBlock = Application::GetBlockClient();

	GatewayMgr::GatewayMap* pGtMap = GATEWAY_MGR()->GetGatewayMap();
	for (auto it = pGtMap->begin(); it != pGtMap->end(); ++it)
	{
		rpc_OnChannelStatus(it->second->GetLink(), r);
	}
}

bool Channel2GatewayHandler::ProcessClientLogin(NetLinkPtr spLink, const Session& r)
{
	is_no_gateway_ret(spLink);
	prn_inf("add new session, uid(%I64u)", r.m_iUID);
	spGateway->Add(r);
	rpc_OnClientLoginResult(spGateway->GetLink(), eSR_SUCCESS, r.m_iUID);

	OnUpdateChannelStatus();
	return true;
}

bool Channel2GatewayHandler::OnClientLogin(NetLinkPtr spLink, const Session& r)
{
	is_no_gateway_ret(spLink);

	Session* pSession = GATEWAY_MGR()->GetSession(r.m_iUID);
	if (pSession)
	{
		prn_inf("duplicated session, kick uid(%I64u) gid(%u)",
			pSession->m_iUID, pSession->m_iGID);
		OnDuplicatedSession(spLink, *pSession);
	}
	else
	{
		OnNewSession(spLink, r);
	}

	return true;
}

bool Channel2GatewayHandler::OnClientLogoff(NetLinkPtr spLink, UID iUID, uint32 iAuthKey)
{
	is_no_gateway_ret(spLink);

	Session* pSession = GATEWAY_MGR()->GetSession(iUID);
	if (pSession)
	{
		if (pSession->m_iAuthKey == iAuthKey)
		{
			rpc_OnClientLogoffResult(spLink, eSL_SUCCESS, iUID, pSession->m_iAuthKey);
			OnLogoffSession(spLink, *pSession);
			spGateway->Remove(iUID);
			prn_dbg("logoff success, uid(%I64u) gid(%u)", iUID, spGateway->GetID());

			OnUpdateChannelStatus();	
		}
		else
		{
			rpc_OnClientLogoffResult(spLink, eSL_INVALID_AUTHKEY, iUID, pSession->m_iAuthKey);
			prn_dbg("logoff failed, authkey mismatch, uid(%I64u) gid(%u)", iUID, spGateway->GetID());
		}
	}
	else
	{
		rpc_OnClientLogoffResult(spLink, eSL_NOT_FOUND_UID, iUID, iAuthKey);
		prn_dbg("logoff failed, session not found, uid(%I64u) gid(%u)", iUID, spGateway->GetID());
	}

	return true;
}