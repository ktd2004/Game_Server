#pragma once

#include <Proactor.h>
#include "AAATypes.h"
#include "AAAServerProtocol.h"

PTR(Session);

class IDBProvider;
class Master2GatewayHandler : public NetEventHandler
{
protected:
	MID						m_iMID;
	uint32					m_iPriority;
	uint16					m_iThreadCnt;
	std::string				m_sAddr;
	uint16					m_iPort;

public:
	Master2GatewayHandler( uint32 iTaskID );
	virtual ~Master2GatewayHandler();

	bool			Initialize( void );
	bool			StartupMasterHandler( void );
	virtual void	SetDBProvider( IDBProvider* pProvider ) {}
	void			OnUpdateMasterStatus( void );

private:
	// disconnect gateway
	bool	OnClosed( NetLinkPtr spLink );

	// to gateway
	IMPORT_RPC_NM_V1( OnMasterStatus, const MasterStatusData&, 1024 );
	IMPORT_RPC_NM_V2( OnClientLoginResult, int32, UID, 1024 );					// result, uid
	IMPORT_RPC_NM_V4( OnClientLogoffResult, int32, CID, UID, uint32, 1024 );	// result, cid, uid, authkey

	// from gateway
	bool	OnGatewayConnData( NetLinkPtr spLink, const GatewayConnData& g );
	bool	OnClientLogin( NetLinkPtr spLink, const Session& r );
	bool	OnClientLogoff( NetLinkPtr spLink, CID iChannelID, UID iUID, uint32 iAuthKey );

	// from channel
	bool	OnChannelConnData( NetLinkPtr spLink, const ChannelConnData& c );
};
