/* Copyright 2013 by Lee yong jun
 * All rights reserved
 *
 * Distribute freely, except: don't remove my name from the source or
 * documentation (don't take credit for my work), mark your changes (don't
 * get me blamed for your possible bugs), don't alter or remove this
 * notice.  May be sold if buildable source is provided to buyer.  No
 * warrantee of any kind, express or implied, is included with this
 * software; use at your own risk, responsibility for damages (if any) to
 * anyone resulting from the use of this software rests entirely with the
 * user.
 *
 * Send bug reports, bug fixes, enhancements, requests, flames, etc., and
 * I'll try to keep a version up to date.  I can be reached as follows:
 * Lee yong jun          iamhere01@naver.com
 */
#include "stdafx.h"
#include <INILoader.h>
#include <DBEngine.h>
#include <Logger.h>
#include "DbgwHandler.h"

DbgwHandler::DbgwHandler(int argc, char* argv[])
	: ServerApp(argc, argv)
{
}

DbgwHandler::~DbgwHandler()
{
}

bool DbgwHandler::OnSetProperty( ServerApp::Properties& appCfg )
{
	//////////////////////////////////////////////////////////////////////////
	// config load
	appCfg.iSID						= Env_i( DBGW, "id", -1 );
	appCfg.iTaskDuration			= Env_i( DBGW, "task_duration", 10 ) ;
	appCfg.bLogArchive				= Env_b( DBGW, "log_archive_enable", true );
	appCfg.sLogArchivePath			= Env_s( DBGW, "log_path", ".\\");
	appCfg.sLogBindAddr				= Env_s( DBGW, "log_bind_addr", "localhost" );
	appCfg.bLogTransmit				= Env_b( DBGW, "log_transmit_enable", false );
	appCfg.iCompressOverSize		= Env_i( DBGW, "log_compress_size", 10240 );
	appCfg.iTaskSchedulersCount		= 1;
	appCfg.iNotifyTaskSchedulerID	= 1;
	std::string sLogSection			= Env_s( DBGW, "connect_log", "LogS" );
	appCfg.sLogSAddr				= Env_s( sLogSection, "log_bind_addr", "localhost" );
	appCfg.iLogSPort				= Env_i( sLogSection, "log_bind_port", 4001 );
	appCfg.pPAgentHandler			= nullptr;

	return true;
}

bool DbgwHandler::OnInitialized( int32 argc, char* argv[] )
{
	ServerApp::Properties& appCfg = GetProperty();
	//////////////////////////////////////////////////////////////////////////
	prn_sbj( DBGW );
	prn_inf( "++ id %d", appCfg.iSID );
	prn_inf( "++ log archive %s", bool2str(appCfg.bLogArchive) );
	prn_inf( "++ log archive path %s", cstr(appCfg.sLogArchivePath) );
	prn_bar();


	//////////////////////////////////////////////////////////////////////////
	// config database
	std::string sDatabase			= Env_s( DBGW, "connect_database", "DB_Account,DB_GameData" );

	stringlist dblist;
	util::tokenizer(sDatabase, dblist, ',');
	foreach_do( stringlist, it, dblist )
	{
		if ( !InitDatabase( *it ) )
		{
			return false;
		}
	}

	return true;
}

void DbgwHandler::OnQuitApplication( void )
{
	Shutdown();
	QuitApplication(eNORMAL_EXIT);
}

bool DbgwHandler::InitDatabase(const std::string& sDbName)
{
	IDBProvider::Properties config;

	// 데이타 베이스 설정
	util::load_dbcfg(sDbName, config);
	// 서버 접속 설정
	util::load_remotedbcfg(sDbName, config);

	// DB 설정 출력
	prn_sbj( "database" );
	util::show_dbcfg(config);
	util::show_remotedbcfg(config);
	prn_bar();

	IDBProvider* pProxyProvider = IDBProviderFactory::CreateProxy();
	if( !pProxyProvider )
	{
		prn_err( "create proxy failed(%s)", cstr(sDbName) );
		return false;
	}

	int32 rst = pProxyProvider->Startup( config );
	if( !rst )
	{
		prn_err( "can't startup (%s), %s", cstr(sDbName), errmsg );
		return false;
	}

	m_listDB.push_back( pProxyProvider );

	return true;
}

void DbgwHandler::Shutdown(void)
{
	foreach_do( std::list<IDBProviderPtr>, it , m_listDB )
	{
		(*it)->Shutdown();
	}
}
