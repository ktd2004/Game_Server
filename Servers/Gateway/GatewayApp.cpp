#include "stdafx.h"
#include "GatewayApp.h"
#include "ClientHandler.h"
#include <AAAUtil.h>

GatewayApp::GatewayApp( int32 iArgc, char* szArgv[] )
	: ServerApp(iArgc, szArgv)
	, m_pProvider(nullptr)
	, m_pMasterHandler(nullptr)
	, m_pClientHandler(nullptr)
{
}

GatewayApp::~GatewayApp()
{
	safe_delete( m_pClientHandler );
	safe_delete( m_pMasterHandler );
	safe_delete(m_pChannelHandler);

	if( m_pProvider )
	{
		m_pProvider->Shutdown();
		safe_delete( m_pProvider );
	}
}

bool GatewayApp::OnSetProperty( ServerApp::Properties& appCfg )
{
	//////////////////////////////////////////////////////////////////////////
	// config load
	appCfg.iSID						= Env_i( GTWS, "id", null_id );
	appCfg.iTaskDuration			= Env_i( GTWS, "task_duration", 10 );
	appCfg.bLogArchive				= Env_b( GTWS, "log_archive_enable", true );
	appCfg.sLogArchivePath			= Env_s( GTWS, "log_path", ".\\" );
	appCfg.sLogBindAddr				= Env_s( GTWS, "log_bind_addr", "localhost" );
	appCfg.bLogTransmit				= Env_b( GTWS, "log_transmit_enable", false );
	appCfg.iCompressOverSize		= Env_i( GTWS, "log_compress_size", 10240 );
	appCfg.iTaskSchedulersCount		= 1;
	appCfg.iNotifyTaskSchedulerID	= 1;
	std::string sLogSection			= Env_s( GTWS, "connect_log", "LogS" );
	appCfg.sLogSAddr				= Env_s( sLogSection, "log_bind_addr", "localhost" );
	appCfg.iLogSPort				= Env_i( sLogSection, "log_bind_port", 4001 );
	appCfg.pPAgentHandler			= nullptr;	// this

	return true;
}

bool GatewayApp::OnInitialized( int32 argc, char* argv[] )
{
	ServerApp::Properties& appCfg = GetProperty();
	
// 	IDBProvider::Properties config;
// 	config.iNotifyTaskSchedulerID = appCfg.iNotifyTaskSchedulerID;
// 
// 	util::load_dbcfg( "DB_Game", config );
// 
// 	prn_sbj( "Database" );
// 	util::show_dbcfg( config );
// 	prn_bar();
// 
// 	if( !m_pProvider )
// 	{
// 		m_pProvider = IDBProviderFactory::CreateDirect();
// 	}
// 
// 	if( !m_pProvider->Startup( config ) )
// 	{
// 		prn_err( "%s", errmsg );
// 		return false;
// 	}

	m_pMasterHandler = new Gateway2MasterHandler( appCfg.iNotifyTaskSchedulerID );
	m_pChannelHandler = new Gateway2ChannelHandler(appCfg.iNotifyTaskSchedulerID);
	m_pClientHandler = new ClientHandler( appCfg.iNotifyTaskSchedulerID );
	if (!AAAUtil::GatewayInitialize(m_pMasterHandler, m_pClientHandler, m_pChannelHandler, m_pProvider))
	{
		return false;
	}

	return true;
}

void GatewayApp::OnQuitApplication()
{
	if(m_pClientHandler)
	{
		m_pClientHandler->Stop();
	}
	
	if (m_pChannelHandler)
	{
		m_pMasterHandler->Stop();
	}

	if(m_pMasterHandler)
	{
		m_pMasterHandler->Stop();
	}

	if (m_pProvider)
	{
		m_pProvider->Shutdown();
	}

	QuitApplication( eNORMAL_EXIT );
}

// pagent 
bool GatewayApp::OnMaxClient( uint32 iMaxClient )
{
	return true;
}

bool GatewayApp::OnBlockClient( bool bBlock )
{
	return true;
}
