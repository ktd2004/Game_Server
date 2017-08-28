#include "stdafx.h"
#include "ChannelApp.h"
#include "ChannelHandler.h"
#include <AAAUtil.h>


ChannelApp::ChannelApp( int32 iArgc, char* szArgv[] )
	: ServerApp(iArgc, szArgv)
	, m_pChannelHandler(nullptr)
	, m_pProvider(nullptr)
{
}

ChannelApp::~ChannelApp()
{
	safe_delete(m_pChannelHandler);
}

bool ChannelApp::OnSetProperty( ServerApp::Properties& appCfg )
{
	//////////////////////////////////////////////////////////////////////////
	// config load
	appCfg.iSID						= Env_i( CHNS, "id", null_id );
	appCfg.iTaskDuration			= Env_i( CHNS, "task_duration", 10 );
	appCfg.bLogArchive				= Env_b( CHNS, "log_archive_enable", true );
	appCfg.sLogArchivePath			= Env_s( CHNS, "log_path", ".\\" );
	appCfg.sLogBindAddr				= Env_s( CHNS, "log_bind_addr", "localhost" );
	appCfg.bLogTransmit				= Env_b( CHNS, "log_transmit_enable", false );
	appCfg.iCompressOverSize		= Env_i( CHNS, "log_compress_size", 10240 );
	appCfg.iTaskSchedulersCount		= 1;
	appCfg.iNotifyTaskSchedulerID	= 1;
	std::string sLogSection			= Env_s( CHNS, "connect_log", "LogS" );
	appCfg.sLogSAddr				= Env_s( sLogSection, "log_bind_addr", "localhost" );
	appCfg.iLogSPort				= Env_i( sLogSection, "log_bind_port", 4001 );
	appCfg.pPAgentHandler			= nullptr;	// this

	return true;
}

bool ChannelApp::OnInitialized( int32 argc, char* argv[] )
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

	m_pChannelHandler = new ChannelHandler( appCfg.iNotifyTaskSchedulerID );
	if (!AAAUtil::ChannelInitialize(m_pChannelHandler, m_pProvider))
	{
		return false;
	}

	return true;
}

void ChannelApp::OnQuitApplication()
{
	if (m_pChannelHandler)
	{
		m_pChannelHandler->Stop();
	}

	if (m_pProvider)
	{
		m_pProvider->Shutdown();
	}

	QuitApplication( eNORMAL_EXIT );
}

bool ChannelApp::OnMaxClient( uint32 iMaxClient )
{
	return true;
}

bool ChannelApp::OnBlockClient( bool bBlock )
{
	return true;
}