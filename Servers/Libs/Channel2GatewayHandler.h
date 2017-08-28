#pragma once

#include <Proactor.h>
#include "AAATypes.h"
#include "AAAServerProtocol.h"

PTR(Session);

class IDBProvider;
class Channel2GatewayHandler : public NetEventHandler
{
protected:
	CID						m_iCID;
	uint32					m_iPriority;
	uint16					m_iThreadCnt;
	std::string				m_sAddr;
	uint16					m_iPort;

	typedef std::map<std::string, uint32> ConnectionMap;
	ConnectionMap m_ConnMap;

public:
	Channel2GatewayHandler( uint32 iTaskID );
	virtual ~Channel2GatewayHandler();

	bool			Initialize( void );
	bool			StartupChannelHandler( void );
	virtual void	SetDBProvider( IDBProvider* pProvider ) {}
	bool			ProcessClientLogin( NetLinkPtr spLink, const Session& r );
	void			OnUpdateChannelStatus(void);

	virtual void	OnDuplicatedSession( NetLinkPtr spLink, const Session& r ) {};
	virtual void	OnNewSession( NetLinkPtr spLink, const Session& r ) {};
	virtual void	OnLogoffSession( NetLinkPtr spLink, const Session& r ) {};
	
private:
	// connect master
	bool	OnConnected( NetLinkPtr spLink );
	// disconnect gateway
	bool	OnClosed( NetLinkPtr spLink );

	/**
		마스터서버에 채널 정보를 전송한다.
	*/
	IMPORT_RPC_NM_V1( OnChannelConnData,    const ChannelConnData&, 1024 );

	// from gateway
	bool	OnClientLogin( NetLinkPtr spLink, const Session& r );
	bool	OnClientLogoff( NetLinkPtr spLink, UID iUID, uint32 iAuthKey );

	// to gateway
	IMPORT_RPC_NM_V1( OnChannelStatus,      const ChannelStatusData&, 1024 );
	IMPORT_RPC_NM_V2( OnClientLoginResult,  int32, UID, 1024 );
	IMPORT_RPC_NM_V3( OnClientLogoffResult, int32, UID, uint32, 1024 );

	// from gateway
	bool	OnGatewayConnData( NetLinkPtr spLink, const GatewayConnData& g );

	// from master
	bool	OnMasterStatus( NetLinkPtr spLink, const MasterStatusData& r );
};
