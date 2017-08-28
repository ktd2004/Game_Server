#pragma once

#include <Proactor.h>
#include <ServerFrame.h>
#include "AAATypes.h"

class IDBProvider;
class Gateway2MasterHandler;
class Gateway2ChannelHandler;
class ClientHandler;

class GatewayApp : public ServerApp, public PAgentReceiver
{
public:
	GatewayApp( int32 iArgc, char* szArgv[] );
	virtual ~GatewayApp();

	bool	OnSetProperty( ServerApp::Properties& appCfg );
	bool	OnInitialized( int32 argc, char* argv[] );
	void	OnQuitApplication();

	bool	OnMaxClient( uint32 iMaxClient );
	bool	OnBlockClient( bool bBlock );

private:
	IDBProvider*			m_pProvider;
	Gateway2MasterHandler*	m_pMasterHandler;
	Gateway2ChannelHandler* m_pChannelHandler;
	ClientHandler*			m_pClientHandler;
};
