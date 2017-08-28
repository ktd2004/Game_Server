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
#include <PAgentManager.h>
#include <StringUtil.h>
#include <DBEngine.h>
#include <ConsoleManager.h>

#include "SAgentHandler.h"

SAgentHandler::SAgentHandler(int argc, char* argv[])
	: ServerApp(argc, argv)
	, NetEventHandler(1)
	, m_iUniqueID(1)
{
	SetAcceptedFunc ( this, &SAgentHandler::OnAccepted );
	SetConnectedFunc( this, &SAgentHandler::OnConnected );
	SetClosedFunc   ( this, &SAgentHandler::OnClosed );

	EXPORT_RPC_NM_V1( this, &SAgentHandler::OnSAgentConnected, uint32 );
	EXPORT_RPC_NM_V1( this, &SAgentHandler::OnSAgentUpdateProcess, const ServerProcess& );
	EXPORT_RPC_NM_V1( this, &SAgentHandler::OnSAgentExitProcess, const ServerProcess& );
	EXPORT_RPC_NM_V3( this, &SAgentHandler::OnSAgentExecuteCmdResult, uint32, uint32, bool );

	EXPORT_RPC_NM_V0( this, &SAgentHandler::OnConsoleConnected );
	EXPORT_RPC_NM_V1( this, &SAgentHandler::OnConsoleExecCommand, const std::string& );

	//////////////////////////////////////////////////////////////////////////
	EXPORT_PROC_NM_V2( 1, this, &SAgentHandler::OnCommandLaunchProcess, const std::string&, const std::string& );
	EXPORT_PROC_NM_V2( 1, this, &SAgentHandler::OnCommandShutdownProcess, const std::string&, const std::string& );
	EXPORT_PROC_NM_V2( 1, this, &SAgentHandler::OnCommandKillProcess, const std::string&, const std::string& );
	EXPORT_PROC_NM_V2( 1, this, &SAgentHandler::OnCommandDumpServerStatus, const std::string&, const std::string& );
	EXPORT_PROC_NM_V3( 1, this, &SAgentHandler::OnCommandBlockClient, const std::string&, const std::string&, const std::string& );
	EXPORT_PROC_NM_V3( 1, this, &SAgentHandler::OnCommandMaxClient, const std::string&, const std::string&, const std::string& );
	EXPORT_PROC_NM_V1( 1, this, &SAgentHandler::OnCommandVerbose, const std::string& );
}

SAgentHandler::~SAgentHandler()
{
}


bool SAgentHandler::OnSetProperty( ServerApp::Properties& appCfg )
{
	//////////////////////////////////////////////////////////////////////////
	// config load
	appCfg.iSID				= Env_i( SAGENT, "id", -1 );
	appCfg.iTaskDuration	= Env_i( SAGENT, "task_duration", 10 ) ;
	m_iBindPort				= Env_i( SAGENT, "bind_port", 4001 );
	m_sBindAddr				= Env_s( SAGENT, "bind_addr", "localhost" );
	appCfg.bLogArchive		= Env_b( SAGENT, "log_archive_enable", true );
	appCfg.sLogArchivePath	= Env_s( SAGENT, "log_path", ".\\");
	std::string sConfigDB	= Env_s( SAGENT, "database_serverconfig", "" );

	//
	m_DbCfg.sProviderName				= "server_config";
	m_DbCfg.stDatabase.sHost			= Env_s( sConfigDB, "host", "localhost" );
	m_DbCfg.stDatabase.sPort			= Env_s( sConfigDB, "port", "3306" );
	m_DbCfg.stDatabase.bAutoReconnect	= Env_b( sConfigDB, "auto_reconnect", true );
	m_DbCfg.stDatabase.sCategory		= Env_s( sConfigDB, "category", "server_config" );
	m_DbCfg.stDatabase.sUid				= Env_s( sConfigDB, "uid", "root" );
	m_DbCfg.stDatabase.sPassword		= Env_s( sConfigDB, "passwd", "j2soft" );
	m_DbCfg.stDatabase.sCharset			= Env_s( sConfigDB, "charset", "euckr" );

	return true;
}


bool SAgentHandler::OnInitialized( int32 argc, char* argv[] )
{
	ServerApp::Properties& appCfg = GetProperty();

	//////////////////////////////////////////////////////////////////////////
	prn_sbj( SAGENT );
	prn_inf( "++ id %d", appCfg.iSID );
	prn_inf( "++ server agent addr %s:%d", cstr(m_sBindAddr), m_iBindPort );
	prn_inf( "++ log archive %s", bool2str(appCfg.bLogArchive) );
	prn_inf( "++ log path %s", cstr(appCfg.sLogArchivePath) );
	prn_bar();

	//////////////////////////////////////////////////////////////////////////
	/// PAgent에서 관리하는 프로세스 정보 로딩
	PAgentManager::instance()->Load();
	PAgentManager::instance()->Dump();

	if (!Start(0, 1))
	{
		prn_err("%s", errmsg);
		return false;
	}

	if (!Listen(m_iBindPort, m_sBindAddr, 20))
	{
		prn_err( "%s", errmsg );
		return false;
	}

	return true;
}

void SAgentHandler::OnQuitApplication( void )
{
	Stop();
	QuitApplication(eNORMAL_EXIT);
}

bool SAgentHandler::OnAccepted( NetLinkPtr spLink )
{
	prn_inf( ">> accept remote addr %s", spLink->GetAddr().c_str() );
	return true;
}


bool SAgentHandler::OnConnected( NetLinkPtr spLink )
{
	return true;
}

bool SAgentHandler::OnClosed( NetLinkPtr spLink )
{
	switch (spLink->UserDataType())
	{
		case ePAgent:
		{
			ServerProcessManager* pManager = (ServerProcessManager*)spLink->UserData();
			if (pManager)
			{
				prn_inf("pagent(%u) is closed", pManager->m_iPAID);
				pManager->SetAgent(nullptr);
			}
		}
		break;
		case eConsole:
		{
			CONSOLE_MGR()->Remove(spLink);
			prn_inf("console is closed");
		}
		break;
	}
	
	spLink->UserData(nullptr);

	return true;
}

bool SAgentHandler::OnSAgentConnected( NetLinkPtr spLink, uint32 iPAID )
{
	prn_inf( ">> process agent (%d) connected", iPAID );
	ServerProcessManager* pManager = PAgentManager::instance()->GetServerProcessManager(iPAID);
	if ( pManager )
	{
		spLink->UserData( pManager, ePAgent );
		pManager->SetAgent( spLink );
		ServerProcessMap& Map = pManager->GetMap();
		foreach_do( ServerProcessMap, it, Map )
		{
			// server process
			rpc_OnPAgentAddProcess( spLink, *(it->second) );
		}
	}

	rpc_OnPAgentConnectResult( spLink, pManager ? true : false, pManager->m_sPAgentName, m_DbCfg );

	return true;
}


bool SAgentHandler::OnConsoleConnected(NetLinkPtr spLink)
{
	Console* pConsole = CONSOLE_MGR()->Add(spLink);
	prn_inf(">> console (%d) connected", pConsole->m_iIndex);
	spLink->UserData(pConsole, eConsole);
	rpc_OnReady(spLink);
	return true;
}

bool SAgentHandler::OnConsoleExecCommand(NetLinkPtr spLink, const std::string& sCommand)
{
	if (sCommand.empty())
	{
		return rpc_OnConsoleExecResult(spLink, "command invalid\n");
	}

	Console* pConsole = (Console*)spLink->UserData();

	stringvector args;
	util::tokenizer(sCommand, args, ' ');

	if (args.size() >= 3 && util::upper(args[0]) == "LAUNCH")
	{
		OnCommandLaunchProcess(pConsole, args[1], args[2]);
	}
	else if (args.size() >= 3 && util::upper(args[0]) == "SHUTDOWN")
	{
		OnCommandShutdownProcess(pConsole, args[1], args[2]);
	}
	else if (args.size() >= 3 && util::upper(args[0]) == "KILL")
	{
		OnCommandKillProcess(pConsole, args[1], args[2]);
	}
	else
	if (util::upper(args[0]) == "SHOW")
	{
		switch (args.size())
		{
		case 2:
			OnCommandDumpServerStatus(pConsole, args[1], null_string);
			break;
		case 3:
			OnCommandDumpServerStatus(pConsole, args[1], args[2]);
			break;
		default:
			OnCommandDumpServerStatus(pConsole, null_string, null_string);
			break;
		}
	}
	else if (args.size() >= 4 && util::upper(args[0]) == "BLOCK")
	{
		OnCommandBlockClient(pConsole, args[1], args[2], args[3]);
	}
	else if (args.size() >= 4 && util::upper(args[0]) == "MAXCLIENT")
	{
		OnCommandMaxClient(pConsole, args[1], args[2], args[3]);
	}
	else
	{
		rpc_OnConsoleExecResult(spLink, "command invalid\n");
	}

	return true;
}

bool SAgentHandler::OnSAgentUpdateProcess( NetLinkPtr spLink, const ServerProcess& Pkt )
{
	ServerProcessManager* pManager = (ServerProcessManager*)spLink->UserData();
	if ( pManager )
	{
		ServerProcess* pServer = pManager->Find( Pkt.m_iSID );
		if ( pServer )
		{
			*pServer = Pkt;
			prn_inf( ">> update server process" );
			prn_inf( "  + pagent id   %d", Pkt.m_iPAID );
			prn_inf( "  + server id   %d", Pkt.m_iSID );
			prn_inf( "  + server name %s", cstr(pServer->m_sAlias) );
			prn_inf( "  + server pid  %d", Pkt.m_iPID );
			prn_inf( "  + block       %d", Pkt.m_bBlock );
			prn_inf( "  + max         %d", Pkt.m_iMaxClient );
		}
	}
	return true;
}

bool SAgentHandler::OnSAgentExitProcess( NetLinkPtr spLink, const ServerProcess& Pkt )
{
	ServerProcessManager* pManager = (ServerProcessManager*)spLink->UserData();
	if ( pManager )
	{
		ServerProcess* pServer = pManager->Find( Pkt.m_iSID );
		if ( pServer )
		{
			*pServer = Pkt;
			prn_inf( ">> exit server process" );
			prn_inf( "  + pagent id   %d", Pkt.m_iPAID );
			prn_inf( "  + server id   %d", Pkt.m_iSID );
			prn_inf( "  + server name %s", cstr(pServer->m_sAlias) );
			prn_inf( "  + server pid  %d", Pkt.m_iPID );
			prn_inf( "  + block       %d", Pkt.m_bBlock );
			prn_inf( "  + max         %d", Pkt.m_iMaxClient );
		}
	}

	return true;
}

bool SAgentHandler::OnSAgentExecuteCmdResult( NetLinkPtr spLink, uint32 iSeq, uint32 iSID, bool bRet )
{
	NetLinkPtr spConsoleLink = CONSOLE_MGR()->Find(iSeq);
	std::string sResult = ">> execute command result\n";
	ServerProcessManager* pManager = (ServerProcessManager*)spLink->UserData();
	if ( pManager )
	{
		ServerProcess* pServer = pManager->Find( iSID );
		if ( pServer )
		{
			sResult += "  + pagent id   " + util::intstr(pServer->m_iPAID) + "\n";
			sResult += "  + server name " + pServer->m_sAlias + "\n";
		}
	}
	sResult += "  + server id " + util::intstr(iSID) + "\n";
	sResult += "  + result " + std::string(bool2str(bRet)) + "\n";

	if (spConsoleLink)
	{
		rpc_OnConsoleExecResult(spConsoleLink, sResult);
	}
	else
	{
		prn_inf("\n%s", cstr(sResult));
	}
	return true;
}

void Usage( void )
{
	std::string bar( 50, '-' );
	printf( "%s\n", bar.c_str() );
	printf( "usage)\n" );
	printf( "%s\n", bar.c_str() );
	printf( "  show\n" 
			"  show user-PC\n"
			"  launch user-PC LogS\n"
			"  shutdown user-PC LogS\n"
			"  kill user-PC LogS\n"
			"  maxclient user-PC GtwS 1000\n"
			"  blockclient user-PC GtwS true\n"
			"  autorestart user-PC GtwS true\n"
			"  kickclient user-PC GtwS accountid\n"
			"  kickallclient user-PC GtwS\n");
	printf( "%s\n", bar.c_str() );
}

//////////////////////////////////////////////////////////////////////////
void SAgentHandler::DoCommand( const std::string& sCommand )
{
	if ( sCommand.empty() )
	{
		return;
	}

	stringvector args;
	util::tokenizer( sCommand, args, ' ' );

	if ( args.size() >= 3 && util::upper(args[0]) == "LAUNCH" )
	{
		proc_OnCommandLaunchProcess(nullptr, args[1], args[2]);
	}
	else
	if ( args.size() >= 3 && util::upper(args[0]) == "SHUTDOWN" )
	{
		proc_OnCommandShutdownProcess(nullptr, args[1], args[2]);
	}
	else
	if ( args.size() >= 3 && util::upper(args[0]) == "KILL" )
	{
		proc_OnCommandKillProcess(nullptr, args[1], args[2]);
	}
	else
	if ( util::upper(args[0]) == "SHOW" )
	{
		switch ( args.size() )
		{
		case 2 :
			proc_OnCommandDumpServerStatus(nullptr, args[1], null_string);
			break;
		case 3 :
			proc_OnCommandDumpServerStatus(nullptr, args[1], args[2]);
			break;
		default:
			proc_OnCommandDumpServerStatus(nullptr, null_string, null_string);
			break;
		}
	}
	else
	if ( args.size() >= 4 && util::upper(args[0]) == "BLOCK" )
	{
		proc_OnCommandBlockClient(nullptr, args[1], args[2], args[3]);
	}
	else
	if ( args.size() >= 4 && util::upper(args[0]) == "MAXCLIENT" )
	{
		proc_OnCommandMaxClient(nullptr, args[1], args[2], args[3]);
	}
	else
	if ( args.size() >= 2 && util::upper(args[0]) == "VERBOSE" )
	{
		proc_OnCommandVerbose(nullptr, args[1]);
	}
	else
	if ( args.size() >= 1 && util::upper(args[0]) == "HELP" )
	{
		Usage();
	}
	else
	{
		Usage();
	}
}

#define AGENT_LEN		9
#define PAID_LEN		5
#define SID_LEN			5
#define NAME_LEN		9
#define PID_LEN			6
#define STATUS_LEN		5
#define BLOCKCLIENT_LEN	5
#define USER_LEN		10
#define RETSTART_LEN	5

bool SAgentHandler::OnCommandDumpServerStatus(interface_ref* pCaller, const std::string& sPAgentName, const std::string& sAlias)
{
	Console* pConsole = (Console*)pCaller;
	PAgentManagerMap& PMap = PAgentManager::instance()->GetPAgentManagerMap();
	if ( PMap.size() == 0 )
	{
		if (pConsole == nullptr)
			prn_inf(">> manage server map is empty");
		else
			rpc_OnConsoleExecResult(pConsole->m_spLink, ">> manage server map is empty");
		return true;
	}

	std::string bar(79, '-');
	std::string sRet = bar + "\n";
	// title
	sRet += util::lpad(PAGENT, AGENT_LEN) + " ";
	sRet += util::rpad("PAID", PAID_LEN) + " ";
	sRet += util::rpad("SID", SID_LEN) + " ";
	sRet += util::lpad("Name", NAME_LEN) + " ";
	sRet += util::lpad("PID", PID_LEN) + " ";
	sRet += util::lpad("STATE", STATUS_LEN) + " ";
	sRet += util::lpad("BLOCK", BLOCKCLIENT_LEN) + " ";
	sRet += util::lpad("RSTRT", RETSTART_LEN) + " ";
	sRet += util::rpad("USER/MAX", USER_LEN) + "\n";
	sRet += bar + "\n";

	foreach_do( PAgentManagerMap, itp, PMap )
	{
		ServerProcessMap& SMap = itp->second->GetMap();
		foreach_do( ServerProcessMap, its, SMap )
		{
			sRet +=	align_s(itp->second->m_sPAgentName, AGENT_LEN) + " ";
			sRet += align_i(itp->second->m_iPAID, PAID_LEN) + " ";
			sRet += align_i(its->second->m_iSID, SID_LEN) + " ";
			sRet += align_s(its->second->m_sAlias, NAME_LEN) + " ";
			sRet += align_i(its->second->m_iPID, PID_LEN) + " ";
			std::string sState;
			if ( its->second->m_iState == eINIT )
				sState = "INIT";
			else if ( its->second->m_iState == eRUNNING )
				sState = "RUN";
			else if ( its->second->m_iState == eQUIT )
				sState = "QUIT";
			sRet += align_s(sState, STATUS_LEN) + " ";
			sRet += align_s(bool2str(its->second->m_bBlock), BLOCKCLIENT_LEN) + " ";
			sRet += align_s(bool2str(its->second->m_bAutoStart), RETSTART_LEN) + " ";
			char szUsers[100];
			sprintf_s( szUsers, sizeof(szUsers), "%d/%d", its->second->m_iUsers, its->second->m_iMaxClient );
			sRet += util::rpad(szUsers, USER_LEN) + " ";
			sRet += "\n";
		}
		sRet += bar + "\n";
	}

	if (pConsole == nullptr)
	{
		prn_inf("\n%s", sRet.c_str());
	}
	else
	{
		rpc_OnConsoleExecResult(pConsole->m_spLink, sRet);
	}

	return true;
}

bool SAgentHandler::OnCommandLaunchProcess(interface_ref* pCaller, const std::string& sPAgentName, const std::string& sAlias)
{
	Console* pConsole = (Console*)pCaller;
	ServerProcessManager* pManager = PAgentManager::instance()->GetServerProcessManager( sPAgentName );
	if ( pManager )
	{
		ServerProcess* pServer = pManager->Find( sAlias );
		if ( pServer )
		{
			rpc_OnPAgentLaunchProcess( pManager->GetAgent(), pConsole == nullptr ? m_iUniqueID : pConsole->m_iIndex, pServer->m_iSID );
		}
	}

	return true;
}

bool SAgentHandler::OnCommandShutdownProcess(interface_ref* pCaller, const std::string& sPAgentName, const std::string& sAlias)
{
	Console* pConsole = (Console*)pCaller;
	ServerProcessManager* pManager = PAgentManager::instance()->GetServerProcessManager( sPAgentName );
	if ( pManager )
	{
		ServerProcess* pServer = pManager->Find( sAlias );
		if ( pServer )
		{
			rpc_OnPAgentShutdownProcess(pManager->GetAgent(), pConsole == nullptr ? m_iUniqueID : pConsole->m_iIndex, pServer->m_iSID);
		}
	}

	return true;
}

bool SAgentHandler::OnCommandKillProcess(interface_ref* pCaller, const std::string& sPAgentName, const std::string& sAlias)
{
	Console* pConsole = (Console*)pCaller;
	ServerProcessManager* pManager = PAgentManager::instance()->GetServerProcessManager( sPAgentName );
	if ( pManager )
	{
		ServerProcess* pServer = pManager->Find( sAlias );
		if ( pServer )
		{
			rpc_OnPAgentKillProcess(pManager->GetAgent(), pConsole == nullptr ? m_iUniqueID : pConsole->m_iIndex, pServer->m_iSID);
		}
	}

	return true;
}

bool SAgentHandler::OnCommandBlockClient(interface_ref* pCaller, const std::string& sPAgentName, const std::string& sAlias, const std::string& sBlock)
{
	Console* pConsole = (Console*)pCaller;
	ServerProcessManager* pManager = PAgentManager::instance()->GetServerProcessManager( sPAgentName );
	if ( pManager )
	{
		ServerProcess* pServer = pManager->Find( sAlias );
		if ( pServer )
		{
			bool bBlock = false;
			if ( util::upper(sBlock) == "TRUE" )
			{
				bBlock = true;
			}
			rpc_OnPAgentBlockClient(pManager->GetAgent(), pConsole == nullptr ? m_iUniqueID : pConsole->m_iIndex, pServer->m_iSID, bBlock);
		}
	}

	return true;
}

bool SAgentHandler::OnCommandMaxClient(interface_ref* pCaller, const std::string& sPAgentName, const std::string& sAlias, const std::string& sMaxClient)
{
	Console* pConsole = (Console*)pCaller;
	ServerProcessManager* pManager = PAgentManager::instance()->GetServerProcessManager( sPAgentName );
	if ( pManager )
	{
		ServerProcess* pServer = pManager->Find( sAlias );
		if ( pServer )
		{
			rpc_OnPAgentMaxClient(pManager->GetAgent(), pConsole == nullptr ? m_iUniqueID : pConsole->m_iIndex, pServer->m_iSID, stouint32(sMaxClient));
		}
	}

	return true;
}

bool SAgentHandler::OnCommandVerbose(interface_ref* pCaller, const std::string& sVerbose)
{
	if ( util::upper(sVerbose) == "ON" )
		prn_console_on();
	else if ( util::upper(sVerbose) == "OFF" )
		prn_console_off();

	return true;
}
