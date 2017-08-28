#include "stdafx.h"
#include <Application.h>
#include "GatewayMgr.h"
#include "ChannelMgr.h"
#include "SessionMgr.h"
#include "AAAConst.h"
#include "AAA2ClientProtocol.h"
#include "Master2GatewayHandler.h"

namespace
{
#define is_no_gateway_ret( link ) \
	GatewayPtr spGateway = (Gateway*)link->UserData(); \
	if( !spGateway ) { \
		prn_err( "can't find gateway object" ); \
		return false; \
	}

#define is_no_channel_ret( link ) \
	ChannelPtr spChannel = (Channel*)link->UserData(); \
	if( !spChannel ) { \
		prn_err( "can't find channel object" ); \
		return false; \
	}
}

Master2GatewayHandler::Master2GatewayHandler( uint32 iTaskID )
	: NetEventHandler(iTaskID)
	, m_iMID(0)
	, m_iPriority(0)
	, m_iThreadCnt(1)
{
	SetClosedFunc  ( this, &Master2GatewayHandler::OnClosed );

	EXPORT_RPC_NM_V1( this, &Master2GatewayHandler::OnGatewayConnData, const GatewayConnData& );
	EXPORT_RPC_NM_V1( this, &Master2GatewayHandler::OnClientLogin,     const Session& );
	EXPORT_RPC_NM_V3( this, &Master2GatewayHandler::OnClientLogoff,    CID, UID, uint32 );

	EXPORT_RPC_NM_V1( this, &Master2GatewayHandler::OnChannelConnData, const ChannelConnData& );
}

Master2GatewayHandler::~Master2GatewayHandler()
{
}

bool Master2GatewayHandler::OnClosed( NetLinkPtr spLink )
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
		case eCHANNEL_OBJ:
		{
			// channel을 삭제한다.
			is_no_channel_ret(spLink);
			CHANNEL_MGR()->Remove(spChannel->GetID());
			prn_inf("channel -> channel(%u) is closed", spChannel->GetID());
		}
		break;
	}

	spLink->UserData(nullptr);
	
	return true;
}

bool Master2GatewayHandler::Initialize()
{
	m_iMID				= Env_i( MSTS, "id", 1 );
	m_iPriority			= Env_i( MSTS, "master_priority", 1 );
	m_iThreadCnt		= Env_i( MSTS, "master_thread_cnt", 1 );
	m_sAddr				= Env_s( MSTS, "listen_addr", "localhost" );
	m_iPort				= Env_i( MSTS, "listen_port", 9001 );
	uint32 iMaxClient	= Env_i( MSTS, "max_client", 10000 );
	bool bBlock			= Env_b( MSTS, "block_client", false );

	prn_sbj( "Master2GatewayHandler" );
	prn_inf( "++ master id %u", m_iMID );
	prn_inf( "++ priority %u", m_iPriority );
	prn_inf( "++ threads %u", m_iThreadCnt );
 	prn_inf( "++ max client %d", iMaxClient );
 	prn_inf( "++ block client %s", bool2str(bBlock) );
	prn_inf( "++ listen addr %s:%u", cstr(m_sAddr), m_iPort);
	prn_bar();

	Application::SetMaxClient( iMaxClient );
	Application::SetBlockClient( bBlock );

	return true;
}


bool Master2GatewayHandler::StartupMasterHandler()
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

	return true;
}

bool Master2GatewayHandler::OnGatewayConnData(NetLinkPtr spLink, const GatewayConnData& g)
{
	GatewayPtr spGateway = GATEWAY_MGR()->Add(g.m_iGID, spLink);
	if (spGateway)
	{
		prn_inf("gateway(%u) is accepted", g.m_iGID);
		spLink->UserData(spGateway,eGATEWAY_OBJ);
		MasterStatusData r;
		r.m_iMID = m_iMID;
		r.m_bBlock = Application::GetBlockClient();
		return rpc_OnMasterStatus(spLink, r);
	}

	return false;
}


bool Master2GatewayHandler::OnChannelConnData(NetLinkPtr spLink, const ChannelConnData& c)
{
	ChannelPtr spChannel = CHANNEL_MGR()->Add(c.m_iCID, spLink);
	if (spChannel)
	{
		prn_inf("channel(%u) is accepted", c.m_iCID);
		spLink->UserData(spChannel, eCHANNEL_OBJ);
		MasterStatusData r;
		r.m_iMID = m_iMID;
		r.m_bBlock = Application::GetBlockClient();
		return rpc_OnMasterStatus(spLink, r);
	}

	return false;
}

void Master2GatewayHandler::OnUpdateMasterStatus( void )
{
	MasterStatusData r;
	r.m_iMID = m_iMID;
	r.m_bBlock = Application::GetBlockClient();

	GatewayMgr::GatewayMap* pGtMap = GATEWAY_MGR()->GetGatewayMap();
	for (auto it = pGtMap->begin(); it != pGtMap->end(); ++it)
	{
		rpc_OnMasterStatus(it->second->GetLink(), r);
	}

	ChannelMgr::ChannelMap* pChMap = CHANNEL_MGR()->GetChannelMap();
	for (auto it = pChMap->begin(); it != pChMap->end(); ++it)
	{
		rpc_OnMasterStatus(it->second->GetLink(), r);
	}
}

bool Master2GatewayHandler::OnClientLogin( NetLinkPtr spLink, const Session& r )
{
	is_no_gateway_ret(spLink);

	int32 iRst = eSR_SUCCESS;
	Session* pSession = GATEWAY_MGR()->GetSession(r.m_iUID);
	if (pSession != nullptr)
	{
		// 중복 세션이면 킥하라고 요청한다.

		if (pSession->m_iGID != spGateway->GetID())
		{
			GatewayPtr spCurrentGateway = GATEWAY_MGR()->Get(pSession->m_iGID);
			if (spCurrentGateway)
			{
				rpc_OnClientLogoffResult(
					spCurrentGateway->GetLink(), eSL_DUPLICATE,
					pSession->m_iChannelID, pSession->m_iUID, pSession->m_iAuthKey);
				spCurrentGateway->Remove(r.m_iUID);
			}
		}
		else
		{
			rpc_OnClientLogoffResult(
				spLink, eSL_DUPLICATE,
				pSession->m_iChannelID, pSession->m_iUID, pSession->m_iAuthKey);
			spGateway->Remove(r.m_iUID);
		}
		
		iRst = eSR_DUPLICATE;

		prn_inf("duplicated session, kick uid(%I64u) channel(%u) gid(%u)",
			pSession->m_iUID, pSession->m_iChannelID, pSession->m_iGID);
	}

	prn_inf( "add new session, uid(%I64u)", r.m_iUID);
	spGateway->Add(r);
	rpc_OnClientLoginResult(spLink, iRst, r.m_iUID);

	return true;
}

bool Master2GatewayHandler::OnClientLogoff(NetLinkPtr spLink, CID iChannelID, UID iUID, uint32 iAuthKey)
{
	is_no_gateway_ret(spLink);

	Session* pSession = GATEWAY_MGR()->GetSession(iUID);
	if (pSession != nullptr)
	{
		if (pSession->m_iAuthKey != iAuthKey)
		{
			prn_inf("logoff failed, authkey mismatch, uid(%I64u) registed(%u) != received(%u)",
				iUID, pSession->m_iAuthKey, iAuthKey);
			return rpc_OnClientLogoffResult(spLink, eSL_INVALID_AUTHKEY, pSession->m_iChannelID, iUID, iAuthKey);
		}

		if (pSession->m_iGID != spGateway->GetID())
		{
			prn_inf("logoff failed, gateway id mismatch, uid(%I64u)", iUID);
				rpc_OnClientLogoffResult(spLink, eSL_INVALID_GATEWAY, pSession->m_iChannelID, iUID, pSession->m_iAuthKey);
			return false;
		}
		else
		{
			rpc_OnClientLogoffResult(spLink, eSL_SUCCESS, pSession->m_iChannelID, iUID, pSession->m_iAuthKey);
			spGateway->Remove(iUID);
		}
		
		prn_inf("logoff uid(%I64u) channel(%u) gid(%u)", iUID, pSession->m_iChannelID, pSession->m_iGID);
	}
	else
	{
		rpc_OnClientLogoffResult(spLink, eSL_NOT_FOUND_UID, iChannelID, iUID, iAuthKey);
		prn_inf("logoff failed, session not found, uid(%I64u) channel(%u) gid(%u)", iUID, iChannelID, spGateway->GetID());
	}

	return true;
}