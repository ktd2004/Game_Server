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
#include "PAgentHandler.h"

PAgentHandler::PAgentHandler(int argc, char* argv[])
	: ServerApp(argc, argv)
	, NetEventHandler(1)
{
	SetAcceptedFunc ( this, &PAgentHandler::OnAccepted );
	SetConnectedFunc( this, &PAgentHandler::OnConnected );
	SetClosedFunc   ( this, &PAgentHandler::OnClosed );

	EXPORT_RPC_NM_V3( this, &PAgentHandler::OnPAgentConnectResult, bool, const std::string&, const IDBProvider::Properties& );
	EXPORT_RPC_NM_V1( this, &PAgentHandler::OnPAgentAddProcess, const ServerProcess& );
	EXPORT_RPC_NM_V2( this, &PAgentHandler::OnPAgentLaunchProcess, uint32, uint32 );
	EXPORT_RPC_NM_V2( this, &PAgentHandler::OnPAgentShutdownProcess, uint32, uint32 );
	EXPORT_RPC_NM_V2( this, &PAgentHandler::OnPAgentKillProcess, uint32, uint32 );
	EXPORT_RPC_NM_V3( this, &PAgentHandler::OnPAgentBlockClient, uint32, uint32, bool );
	EXPORT_RPC_NM_V3( this, &PAgentHandler::OnPAgentMaxClient, uint32, uint32, uint32 );

	EXPORT_RPC_NM_V2( this, &PAgentHandler::OnPAgentRegSID, uint32, uint32 );
	EXPORT_RPC_NM_V4( this, &PAgentHandler::OnPAgentReport, uint32, bool, uint32, uint32 );
}

PAgentHandler::~PAgentHandler()
{
}

bool PAgentHandler::OnSetProperty( ServerApp::Properties& appCfg )
{
	//////////////////////////////////////////////////////////////////////////
	// config load
	m_iSAgentPort			= Env_i( SAGENT, "bind_port", 4001 );
	m_sSAgentAddr			= Env_s( SAGENT, "bind_addr", "localhost" );
	appCfg.iSID				= Env_i( PAGENT, "id", -1 );
	appCfg.iTaskDuration	= Env_i( PAGENT, "task_duration", 10 ) ;
	appCfg.bLogArchive		= Env_b( PAGENT, "log_archive_enable", true );
	appCfg.sLogArchivePath	= Env_s( PAGENT, "log_path", ".\\");
	m_iMaxRestartCnt		= Env_i( PAGENT, "max_restart_count", 3 );
	m_iPAgentPort			= Env_i( PAGENT, "bind_port", 3234 );
	m_sPAgentAddr			= Env_s( PAGENT, "bind_addr", "localhost" );

	return true;
}

bool PAgentHandler::OnInitialized( int32 argc, char* argv[] )
{
	ServerApp::Properties& appCfg = GetProperty();
	//////////////////////////////////////////////////////////////////////////
	prn_sbj( PAGENT );
	prn_inf( "++ id %d", appCfg.iSID );
	prn_inf( "++ server agent addr %s:%d", cstr(m_sSAgentAddr), m_iSAgentPort );
	prn_inf( "++ process agent addr %s:%d", cstr(m_sPAgentAddr), m_iPAgentPort );
	prn_inf( "++ max restart count %d", m_iMaxRestartCnt );
	prn_inf( "++ log archive %s", bool2str(appCfg.bLogArchive) );
	prn_inf( "++ log path %s", cstr(appCfg.sLogArchivePath) );
	prn_bar();

	//////////////////////////////////////////////////////////////////////////
	// config sid
	m_ServerProcessManager.m_iPAID = appCfg.iSID;

	//////////////////////////////////////////////////////////////////////////
	// process manager
	ProcessFactory::Start( 1, appCfg.iTaskDuration );

	//////////////////////////////////////////////////////////////////////////
	// connect sagent

	if (!Start(0, 1))
	{
		prn_err("%s", errmsg);
		return false;
	}

	m_SAgent = Connect( m_iSAgentPort, m_sSAgentAddr );
	if ( !m_SAgent )
	{
		prn_err( "%s", errmsg );
		return false;
	}

	return true;
}

void PAgentHandler::OnQuitApplication( void )
{
	Stop();
	m_SAgent = nullptr;
	QuitApplication(eNORMAL_EXIT);
}


bool PAgentHandler::OnAccepted( NetLinkPtr spLink )
{
	prn_sys(">> accept remote addr %s", spLink->GetAddr().c_str());
	return true;
}


bool PAgentHandler::OnConnected( NetLinkPtr spLink )
{
	prn_inf( ">> connected server agent" );
	rpc_OnSAgentConnected( m_SAgent, GetSID() );
	return true;
}

bool PAgentHandler::OnClosed( NetLinkPtr spLink )
{
	if ( m_SAgent == spLink )
	{
		m_SAgent = Connect( m_iSAgentPort, m_sSAgentAddr );
	}
	else
	{
		ServerProcess* pServer = (ServerProcess*)spLink->UserData();
		if ( pServer )
		{
			pServer->m_spServerLink = nullptr;
		}
		spLink->UserData(nullptr);
	}

	return true;
}

bool PAgentHandler::OnPAgentConnectResult( NetLinkPtr spLink, bool bRet, const std::string& sPAgentName, const IDBProvider::Properties& DbCfg )
{
	prn_inf( ">> server agent connect result %s", bool2str(bRet) );
	if ( bRet )
	{
		m_ServerProcessManager.m_sPAgentName = sPAgentName;
		m_sDbconnectstring = DbCfg.stDatabase.sHost + ",";
		m_sDbconnectstring += DbCfg.stDatabase.sPort + ",";
		m_sDbconnectstring += DbCfg.stDatabase.sCategory + ",";
		m_sDbconnectstring += DbCfg.stDatabase.sUid + ",";
		m_sDbconnectstring += DbCfg.stDatabase.sPassword + ",";
		m_sDbconnectstring += DbCfg.stDatabase.sCharset;

		prn_inf( "  + dbconnect %s", cstr(m_sDbconnectstring) );
		m_ServerProcessManager.Dump();

		foreach_do( ServerProcessMap, it, m_ServerProcessManager.GetMap() )
		{
			rpc_OnSAgentUpdateProcess( m_SAgent, *(it->second) );
		}

		if ( !Listen(m_iPAgentPort, m_sPAgentAddr, 5) )
		{
			prn_err( "%s", errmsg );
			return false;
		}
	}

	return true;
}

bool PAgentHandler::OnPAgentAddProcess( NetLinkPtr spLink, const ServerProcess& Pkt )
{
	ServerProcessPtr spServer = new ServerProcess( Pkt );
	m_ServerProcessManager.Add(spServer);
	return true;
}

bool PAgentHandler::OnPAgentLaunchProcess( NetLinkPtr spLink, uint32 iSeq, uint32 iSID )
{
	ProcessPtr spProcess;
	ServerProcess* pServer = m_ServerProcessManager.Find( iSID );
	if ( pServer )
	{
		pServer->m_iSeq = iSeq;

		std::string sArg, sIniFile, sSection;
		ProcessOption	opt;
/*
		// 서버구성 파일 조회
		opt.Path( ".\\" );
		opt.Exec( "Config.exe" );
		
		if ( pServer->m_sAlias.find(CHNS) != std::string::npos )
		{
			sSection = CHNS;
		}
		else if ( pServer->m_sAlias.find(GTWS) != std::string::npos )
		{
			sSection = GTWS;
		}
		else if ( pServer->m_sAlias.find(LOGS) != std::string::npos )
		{
			sSection = LOGS;
		}
		else if ( pServer->m_sAlias.find(MSTS) != std::string::npos )
		{
			sSection = MSTS;
		}
		else
		{
			sSection = pServer->m_sAlias;
		}

		// config -w World1 -s ChnS,10001 -d iLog.ini
		sIniFile = pServer->m_sAlias + "_" + util::intstr(pServer->m_iSID) + ".ini";
		sArg = " -d " + sIniFile;
		sArg += " -w " + pServer->m_sWorld;
		sArg += " -s " + sSection + std::string(",") + util::intstr(pServer->m_iSID);
		sArg += " -h " + m_sDbconnectstring; 
		opt.CommandLine( sArg );
		opt.WorkingDirectory( pServer->m_sExecPath );
		opt.CreationFlag( CREATE_NEW_CONSOLE );	
		opt.MaxRestartCnt( 0 );
//		opt.InheritHandle(true);
		printf( "cmd = %s\n", opt.CommandLine() );

		spProcess = ProcessFactory::Spawn( opt );
		spProcess->Wait(INFINITE);
		if ( spProcess->ExitCode() != 0 )
		{
			prn_err( ">> launch process - download config error" );
			rpc_OnSAgentExecuteCmdResult( spLink, iSeq, iSID, spProcess ? true : false );
			return true;
		}
*/
		// 서버 실행 정보
		opt.Path( pServer->m_sExecPath );
		opt.Exec( pServer->m_sExec );
		sArg = " -a " + m_sPAgentAddr + "," + util::intstr(m_iPAgentPort);
		sArg += " -i " + util::intstr(pServer->m_iSID);
		sArg += " -f " + sIniFile;
		opt.CommandLine( pServer->m_sArgs + sArg );
		opt.WorkingDirectory( pServer->m_sExecPath );
		opt.CreationFlag( CREATE_NEW_CONSOLE );
		opt.Environment( "" );
		opt.MaxRestartCnt( m_iMaxRestartCnt );
//		opt.InheritHandle(true);
//		prn_inf( "cmd = %s", opt.CommandLine() );
		spProcess = ProcessFactory::Spawn( opt, this, pServer );

		prn_inf( ">> launch process" );
		prn_inf( "  + server id   %d", iSID );
		prn_inf( "  + server name %s", cstr(pServer->m_sAlias) );
		prn_inf( "  + path %s", cstr(pServer->m_sExecPath) );
		prn_inf( "  + exec %s %s", cstr(pServer->m_sExec), cstr(opt.m_sCommandLine) );
		if ( !spProcess )
			prn_inf( "  + execute failed" );
	}

	rpc_OnSAgentExecuteCmdResult( spLink, iSeq, iSID, spProcess ? true : false );
	return true;
}

bool PAgentHandler::OnPAgentShutdownProcess( NetLinkPtr spLink, uint32 iSeq, uint32 iSID )
{
	ServerProcess* pServer = m_ServerProcessManager.Find( iSID );
	if ( pServer )
	{
		prn_inf( ">> shutdown process" );
		prn_inf( "  + server id   %d", iSID );
		prn_inf( "  + server name %s", cstr(pServer->m_sAlias) );
		prn_inf( "  + server pid  %d", pServer->m_iPID );

		Process* pProcess = ProcessFactory::GetProcess( pServer->m_iPID );
		if ( pProcess )
		{
			pProcess->SetRestart( false );
		}

		rpc_OnServerShutdown( pServer->m_spServerLink, iSeq );
	}

	rpc_OnSAgentExecuteCmdResult( spLink, iSeq, iSID, pServer ? true : false );

	return true;
}

bool PAgentHandler::OnPAgentKillProcess( NetLinkPtr spLink, uint32 iSeq, uint32 iSID )
{
	ServerProcess* pServer = m_ServerProcessManager.Find( iSID );
	if ( pServer )
	{
		prn_inf( ">> kill process" );
		prn_inf( "  + server id   %d", iSID );
		prn_inf( "  + server name %s", cstr(pServer->m_sAlias) );
		prn_inf( "  + server pid  %d", pServer->m_iPID );

		ProcessFactory::Terminate( pServer->m_iPID );
	}

	rpc_OnSAgentExecuteCmdResult( spLink, iSeq, iSID, pServer ? true : false );
	
	return true;
}

bool PAgentHandler::OnPAgentRegSID( NetLinkPtr spLink, uint32 iSID, uint32 iPID )
{
	ServerProcess* pServer = m_ServerProcessManager.Find( iSID );
	if ( pServer )
	{
		prn_inf( ">> reg process" );
		prn_inf( "  + server id   %d", iSID );
		prn_inf( "  + server name %s", cstr(pServer->m_sAlias) );
		prn_inf( "  + server pid  %d", iPID );

		pServer->m_iPID = iPID;
		pServer->m_spServerLink = spLink;
		spLink->UserData( pServer );
		ProcessFactory::Monitor( iPID, this, pServer );
		rpc_OnServerConnectResult( spLink, true, pServer->m_bBlock, pServer->m_iMaxClient );
		rpc_OnSAgentUpdateProcess( m_SAgent, *pServer );
	}
	else
	{
		rpc_OnServerConnectResult( spLink, false, false, 0 );
	}

	return true;
}


bool PAgentHandler::OnPAgentReport( NetLinkPtr spLink, uint32 iSID, bool bBlock, uint32 iMaxClient, uint32 iUsers )
{
	ServerProcess* pServer = m_ServerProcessManager.Find( iSID );
	if ( pServer )
	{
		prn_inf( ">> report process" );
		prn_inf( "  + server id   %d", iSID );
		prn_inf( "  + server name %s", cstr(pServer->m_sAlias) );
		prn_inf( "  + server pid  %d", pServer->m_iPID );
		prn_inf( "  + block       %d", bBlock );
		prn_inf( "  + max         %d", iMaxClient );
		prn_inf( "  + users       %d", iUsers );

		pServer->m_bBlock = bBlock;
		pServer->m_iMaxClient = iMaxClient;
		pServer->m_iUsers = iUsers;

		rpc_OnSAgentUpdateProcess( m_SAgent, *pServer );
	}

	return true;
}


void PAgentHandler::OnLaunchProcess( ProcessPtr spProcess )
{
	ServerProcess* pServer = (ServerProcess*)spProcess->UserData();
	if ( pServer )
	{
		pServer->m_iState = eRUNNING;
		pServer->m_iPID = spProcess->GetProcessId();
		rpc_OnSAgentUpdateProcess( m_SAgent, *pServer );
	}
}

void PAgentHandler::OnExitProcess( ProcessPtr spProcess )
{
	ServerProcess* pServer = (ServerProcess*)spProcess->UserData();
	if ( pServer )
	{
		prn_inf( ">> exit process" );
		prn_inf( "  + server id   %d", pServer->m_iSID );
		prn_inf( "  + server name %s", cstr(pServer->m_sAlias) );
		prn_inf( "  + server pid  %d", pServer->m_iPID );

		pServer->m_iState = eQUIT;
		rpc_OnSAgentExitProcess( m_SAgent, *pServer );
	}
}

bool PAgentHandler::OnPAgentBlockClient( NetLinkPtr spLink, uint32 iSeq, uint32 iSID, bool bBlock )
{
	ServerProcess* pServer = m_ServerProcessManager.Find( iSID );
	if ( pServer )
	{
		prn_inf( ">> block client" );
		prn_inf( "  + server id   %d", iSID );
		prn_inf( "  + server name %s", cstr(pServer->m_sAlias) );
		prn_inf( "  + server pid  %d", pServer->m_iPID );
		prn_inf( "  + block       %d", bBlock );

		pServer->m_bBlock = bBlock;

		rpc_OnServerBlockClient( pServer->m_spServerLink, bBlock );
	}

	rpc_OnSAgentExecuteCmdResult( spLink, iSeq, iSID, pServer ? true : false );

	return true;
}

bool PAgentHandler::OnPAgentMaxClient( NetLinkPtr spLink, uint32 iSeq, uint32 iSID, uint32 iMaxClient )
{
	ServerProcess* pServer = m_ServerProcessManager.Find( iSID );
	if ( pServer )
	{
		prn_inf( ">> max client" );
		prn_inf( "  + server id   %d", iSID );
		prn_inf( "  + server name %s", cstr(pServer->m_sAlias) );
		prn_inf( "  + server pid  %d", pServer->m_iPID );
		prn_inf( "  + max         %d", iMaxClient );
		
		pServer->m_iMaxClient = iMaxClient;
		rpc_OnServerMaxClient( pServer->m_spServerLink, iMaxClient );
	}

	rpc_OnSAgentExecuteCmdResult( spLink, iSeq, iSID, pServer ? true : false );

	return true;
}