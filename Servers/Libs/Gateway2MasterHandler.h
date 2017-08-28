#pragma once

#include <Proactor.h>
#include "AAA2ClientProtocol.h"
#include "AAAServerProtocol.h"

PTR(Session)

class Gateway2ClientHandler;
class Gateway2ChannelHandler;

/** 
	Gateway에서 Master서버와 통신을 위해 사용하는 네트웍 이벤트 핸들러
*/
class Gateway2MasterHandler : public NetEventHandler
{
	friend class Gateway2ClientHandler;
protected:
	GID						m_iGID;				///< 게이트웨이아이디
	Gateway2ClientHandler*	m_pClientHandler;
	Gateway2ChannelHandler* m_pChannelHandler;
	uint32					m_iPriority;
	uint16					m_iThreadCnt;
	std::string				m_sAddr;
	uint16					m_iPort;

	typedef std::map<std::string, uint32> ConnectionMap;
	ConnectionMap m_ConnMap;

public:
	Gateway2MasterHandler( uint32 iTaskID );
	virtual ~Gateway2MasterHandler();

	bool	Initialize();
	void	SetClientHandler( Gateway2ClientHandler* pHandler ) { m_pClientHandler = pHandler; }
	void	SetChannelHandler(Gateway2ChannelHandler* pHandler) { m_pChannelHandler = pHandler; }
	bool	StartupMasterHandler();

private:
	// connect master
	bool			OnConnected( NetLinkPtr spLink );
	// disconnect master
	bool			OnClosed( NetLinkPtr spLink );
	/**
		마스터서버에 게이트웨이 정보를 전송한다.
	*/
	IMPORT_RPC_NM_V1(OnGatewayConnData, const GatewayConnData&, 1024);
	/**
		마스터서버에 로그인을 요청한다.
	*/
	IMPORT_RPC_NM_V1( OnClientLogin, const Session&, 1024 );
	bool	OnClientLogin( SessionPtr spSession );
	/*
		마스터 서버에 로그오프 시키라고 요청한다.
	*/
	IMPORT_RPC_NM_V3( OnClientLogoff, CID, UID, uint32, 1024 );		// cid, uid, authkey
	bool	OnClientLogoff( MID iMasterID, CID iChannelID, UID iUID, uint32 iAuthKey );

	// from master
	bool	OnMasterStatus( NetLinkPtr spLink, const MasterStatusData& r );
	bool	OnClientLoginResult( NetLinkPtr spLink, int32 iResult, UID iUID );
	bool	OnClientLogoffResult( NetLinkPtr spLink, int32 iResult, CID iChannelID, UID iUID, uint32 iAuthKey );
};
