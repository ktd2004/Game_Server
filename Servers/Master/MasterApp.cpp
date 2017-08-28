#include "stdafx.h"
#include "MasterApp.h"
#include <AAAUtil.h>


MasterApp::MasterApp( int32 iArgc, char* szArgv[] )
	: ServerApp(iArgc, szArgv)
	, m_pMasterHandler(nullptr)
	, m_pProvider(nullptr)
{
}

MasterApp::~MasterApp()
{
	safe_delete(m_pMasterHandler);
}

bool MasterApp::OnSetProperty( ServerApp::Properties& appCfg )
{
	//////////////////////////////////////////////////////////////////////////
	// config load
	appCfg.iSID						= Env_i( MSTS, "id", null_id );
	appCfg.iTaskDuration			= Env_i( MSTS, "task_duration", 10 );
	appCfg.bLogArchive				= Env_b( MSTS, "log_archive_enable", true );
	appCfg.sLogArchivePath			= Env_s( MSTS, "log_path", ".\\" );
	appCfg.sLogBindAddr				= Env_s( MSTS, "log_bind_addr", "localhost" );
	appCfg.bLogTransmit				= Env_b( MSTS, "log_transmit_enable", false );
	appCfg.iCompressOverSize		= Env_i( MSTS, "log_compress_size", 10240 );
	appCfg.iTaskSchedulersCount		= 1;
	appCfg.iNotifyTaskSchedulerID	= 1;
	std::string sLogSection			= Env_s( MSTS, "connect_log", "LogS" );
	appCfg.sLogSAddr				= Env_s( sLogSection, "log_bind_addr", "localhost" );
	appCfg.iLogSPort				= Env_i( sLogSection, "log_bind_port", 4001 );
	appCfg.pPAgentHandler			= nullptr;	// this

	return true;
}

bool MasterApp::OnInitialized( int32 argc, char* argv[] )
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
	m_pMasterHandler = new Master2GatewayHandler(appCfg.iNotifyTaskSchedulerID);
	if (!AAAUtil::MasterInitialize(m_pMasterHandler,m_pProvider))
	{
		return false;
	}

	return true;
}

void MasterApp::OnQuitApplication()
{
	if (m_pMasterHandler)
	{
		m_pMasterHandler->Stop();
	}

	if (m_pProvider)
	{
		m_pProvider->Shutdown();
	}

	QuitApplication( eNORMAL_EXIT );
}

bool MasterApp::OnMaxClient( uint32 iMaxClient )
{
	return true;
}

bool MasterApp::OnBlockClient( bool bBlock )
{
	return true;
}
