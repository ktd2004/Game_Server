#pragma once

#include <Proactor.h>
#include <ServerFrame.h>
#include "AAATypes.h"

class IDBProvider;
class ChannelHandler;
class ChannelApp : public ServerApp, public PAgentReceiver
{
public:
	ChannelApp( int32 iArgc, char* szArgv[] );
	virtual ~ChannelApp();

	virtual bool	OnSetProperty( ServerApp::Properties& appCfg );
	virtual bool	OnInitialized( int32 argc, char* argv[] );
	virtual void	OnQuitApplication();

	bool	OnMaxClient( uint32 iMaxClient );
	bool	OnBlockClient( bool bBlock );

private:
	IDBProvider*		m_pProvider;
	ChannelHandler*		m_pChannelHandler;
};
