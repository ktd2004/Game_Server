#pragma once

#include <Proactor.h>
#include <ServerFrame.h>
#include "AAATypes.h"

class IDBProvider;
class Master2GatewayHandler;
class MasterApp : public ServerApp, public PAgentReceiver
{
public:
	MasterApp( int32 iArgc, char* szArgv[] );
	virtual ~MasterApp();

	bool	OnSetProperty( ServerApp::Properties& appCfg );
	bool	OnInitialized( int32 argc, char* argv[] );
	void	OnQuitApplication();

	bool	OnMaxClient( uint32 iMaxClient );
	bool	OnBlockClient( bool bBlock );

private:
	IDBProvider*				m_pProvider;
	Master2GatewayHandler*		m_pMasterHandler;
};
