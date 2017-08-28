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

#include "LogHandler.h"

LogHandler::LogHandler(int argc, char* argv[])
	: ServerApp(argc, argv)
	, NetEventHandler(1)
{
	SetAcceptedFunc ( this, &LogHandler::OnAccepted );
	SetConnectedFunc( this, &LogHandler::OnConnected );
	SetClosedFunc   ( this, &LogHandler::OnClosed );

	EXPORT_RPC_NM_V2( this, &LogHandler::OnLogTransmit, uint32, const std::string& );
}

LogHandler::~LogHandler()
{
}


bool LogHandler::OnSetProperty( ServerApp::Properties& appCfg )
{
	//////////////////////////////////////////////////////////////////////////
	// config load
	appCfg.iSID						= Env_i( LOGS, "id", -1 );
	appCfg.iTaskDuration			= Env_i( LOGS, "task_duration", 10 ) ;
	appCfg.bLogArchive				= Env_b( LOGS, "log_archive_enable", true );
	appCfg.sLogArchivePath			= Env_s( LOGS, "log_path", ".\\");
	appCfg.iTaskSchedulersCount		= 1;
	appCfg.iNotifyTaskSchedulerID	= 1;
	m_iBindPort						= Env_i( LOGS, "bind_port", 4001 );
	m_sBindAddr						= Env_s( LOGS, "bind_addr", "localhost" );
	appCfg.pPAgentHandler			= nullptr;

	return true;
}


bool LogHandler::OnInitialized( int32 argc, char* argv[] )
{
	ServerApp::Properties& appCfg = GetProperty();

	//////////////////////////////////////////////////////////////////////////
	prn_sbj( LOGS );
	prn_inf( "++ id %d", appCfg.iSID );
	prn_inf( "++ log server addr %s:%d", cstr(m_sBindAddr), m_iBindPort );
	prn_inf( "++ log archive %s", bool2str(appCfg.bLogArchive) );
	prn_inf( "++ log path %s", cstr(appCfg.sLogArchivePath) );
	prn_bar();

	//////////////////////////////////////////////////////////////////////////
	// port bind
	if ( !Bind(m_iBindPort, m_sBindAddr) )
	{
		prn_err( "%s", errmsg );
		return false;
	}

	return true;
}

void LogHandler::OnQuitApplication( void )
{
	Stop();
	QuitApplication(eNORMAL_EXIT);
}

bool LogHandler::OnAccepted( NetLinkPtr spLink )
{
	prn_inf( ">> accept remote addr %s", spLink->GetAddr().c_str() );
	return true;
}


bool LogHandler::OnConnected( NetLinkPtr spLink )
{
	return true;
}

bool LogHandler::OnClosed( NetLinkPtr spLink )
{
	return true;
}

bool LogHandler::OnLogTransmit( NetLinkPtr spLink, uint32 iLevel, const std::string& sLog )
{
	SOCKADDR_IN* from = (SOCKADDR_IN*)spLink.param();
	if ( from )
	{
		printf( "recv ip = %s\n", NetLink::tostr(from).c_str() );
	}
	prn_dbg("level %d, %s", iLevel, cstr(sLog));
	return true;
}
