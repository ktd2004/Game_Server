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
#include <getopt.h>

#include <MacroFunc.h>
#include <TimeUtil.h>
#include <FileUtil.h>
#include <StringUtil.h>
#include <INILoader.h>
#include <Logger.h>
#include <ServerApp.h>

ServerApp* theApp = nullptr;

ServerApp::ServerApp(int32 argc, char* argv[], bool bArgParse)
	: m_argc(argc)
	, m_argv(argv)
	, m_iState(eINIT)
	, m_tCheckDuration(1000)
	, m_iExitCode(0)
	, m_PAgent(nullptr)
{
	char* exe = argv[0];
	::SetConsoleTitle( exe );
	CrashDump::RegisterExceptionFilter(this);
	theApp = this;
	if (bArgParse)
	{
		ParseCommandLine(m_argc, m_argv);
	}
}

ServerApp::~ServerApp()
{
	theApp = nullptr;
}

void ServerApp::OnCrash( IN LPTSTR lpszDumpPath )
{
	sprintf_s(lpszDumpPath, 1024,
		"%s_%d",
		m_argv[0],
		m_AppProp.iSID );
}

void ServerApp::ParseCommandLine( int32 argc, char* argv[] )
{
// for debug
// 	printf( "argc = %d\n", argc );
// 	for ( int i = 0; i < argc; i++ )
// 	{
// 		printf( "%s ", argv[i] );
// 	}
// 	printf( "\n" );
// 	getchar();

	// name, has_arg, flag, val 
	struct  option  long_options[]  =
	{
		{"config", 1, 0, 0},
		{"agent",  1, 0, 0},
		{"id",     1, 0, 0},
		{0, 0, 0, 0}
	};

	int  opt;
	while(1)
	{
		int  option_index = 0; 
		opt  =  getopt_long(argc, argv, "f:a:i:", long_options, &option_index); 
		if  (opt  ==  -1)
			break; 

		switch(opt)
		{
		case  0:	// long option
			if  (long_options[option_index].flag  !=  0)
				break;
			if (optarg)
			{
				switch (option_index)
				{
				case 0 :	// config
					m_sSetupFileName = optarg;
					break;
				case 1 :	// agent
					m_AppProp.sPAgentAddr = util::pairkey( optarg, "," );
					m_AppProp.iPAgentPort = toint32(util::pairval( optarg, "," ).c_str());
					break;
				case 2 :	// id
					m_AppProp.iSID = toint32(optarg);
					break;
				}
			}
			break; 
		case  'f':
			m_sSetupFileName = optarg;
			break; 
		case  'a':
//			printf( "option -a %s\n", optarg );
			m_AppProp.sPAgentAddr = util::pairkey( optarg, "," );
			m_AppProp.iPAgentPort = toint32(util::pairval( optarg, "," ).c_str());
//			printf( "addr(%s), port(%d)\n", cstr(m_AppProp.sPAgentAddr), m_AppProp.iPAgentPort );
			break; 
		case  'i':
			m_AppProp.iSID = toint32(optarg);
			break; 
		case  '?':
			break; 
		default:
			break;
		}
	}
}

std::string ServerApp::GetProgramName()
{
	return util::getfilename(m_argv[0]);
}

std::string ServerApp::GetSectionName()
{
	return m_sSectionName;
}

bool ServerApp::IsAgentMode()
{
	return m_AppProp.iPAgentPort != 0;
}

std::string ServerApp::GetAgentAddr()
{
	return m_AppProp.sPAgentAddr;
}

int32 ServerApp::GetAgentPort()
{
	return m_AppProp.iPAgentPort;
}

int32 ServerApp::GetSID()
{
	return m_AppProp.iSID;
}

void ServerApp::SetSID( int32 iSID )
{
	m_AppProp.iSID = iSID;
}

void ServerApp::CheckStateDuration( uint32 tDuration )
{
	m_tCheckDuration = tDuration;
}

void ServerApp::QuitApplication(int32 iExitCode)
{
	m_iState = eQUIT;
	m_iExitCode = iExitCode;
}

bool ServerApp::LoadSetupFile( const std::string& sFileName, const std::string& sSectionName )
{
	if ( m_sSetupFileName.size() == 0 )
	{ m_sSetupFileName = sFileName; }

	if( !INILoader::instance()->Load(m_sSetupFileName) )
	{
		prn_err( "Load failed config - %s", m_sSetupFileName.c_str() );
		m_iExitCode = eNOT_FOUND_INI_FILE;
		return false;
	}

	m_sSectionName = sSectionName;
	return true;
}

bool ServerApp::Initialize(void)
{
	if ( !OnSetProperty(m_AppProp) )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// 시스템 로그 설정
	if (m_AppProp.bSyslog)
	{
		prn_sys_on();
	}

	//////////////////////////////////////////////////////////////////////////
	// 로그 파일 생성 설정
	if ( m_AppProp.bLogArchive )
	{
		prn_file_on();
		prn_path( m_AppProp.sLogArchivePath, 0 );
	}

	//////////////////////////////////////////////////////////////////////////
	// 기본 테스트스케줄러를 생성한다.
	for ( int32 i = 1; i <= m_AppProp.iTaskSchedulersCount; i++ )
	{
		CREATE_TASKSCEDULER( i, m_AppProp.iTaskDuration, 0 );
		if ( GetLastError() != NOERROR )
		{
			prn_err( "%s", errmsg );
			return false;
		}
	}
	START_TASKSCHEDULER( 0 );
	if (IsAgentMode())
	{
		m_PAgent = new PAgentConnector;
		m_PAgent->NotifyTask(m_AppProp.iNotifyTaskSchedulerID);
		//////////////////////////////////////////////////////////////////////////
		// proactor 실행
		if (!m_PAgent->Start(0, m_AppProp.iNetworkThreadsCount))
		{
			prn_err("%s", errmsg);
			return false;
		}
	}
	

	//////////////////////////////////////////////////////////////////////////
	if ( !OnInitialized(m_argc, m_argv) )
	{
		if (m_PAgent != nullptr)
		{
			m_PAgent->Stop();
		}
		STOP_TASKSCHEDULER(0);
		QuitApplication(eARGUMENT_PARSE_ERROR);
		return false;
	}

	PAgentConnector::Properties agentCfg;
	agentCfg.pHandler = m_AppProp.pPAgentHandler;
	agentCfg.bLogTransmit = m_AppProp.bLogTransmit;
	agentCfg.iLogsPort = m_AppProp.iLogSPort;
	agentCfg.sLogsAddr = m_AppProp.sLogSAddr;
	agentCfg.sBindAddr = m_AppProp.sLogBindAddr;

	if (m_PAgent != nullptr)
	{
		m_PAgent->Initialize(agentCfg);
		m_PAgent->Connect();
	}

	//////////////////////////////////////////////////////////////////////////
	m_iState = eRUNNING;

	return true;
}

void ServerApp::RunEventLoop(void)
{
	while (m_iState == eRUNNING)
	{
		if(_kbhit())
		{
			std::cout << "\n>> ";
			std::string s;
			std::getline(cin, s);
			if( util::upper(s) == "QUIT" )
			{ 
				OnQuitApplication();
			}
			else if (util::upper(s) == "SYSLOG")
			{
				if (m_AppProp.bSyslog)
					prn_sys_off();
				else
					prn_sys_on();
				m_AppProp.bSyslog = !m_AppProp.bSyslog;
			}
			else if ( s.size() > 0 )
			{
				DoCommand( s );
			}
		}

		Sleep(m_tCheckDuration);
	}

	STOP_TASKSCHEDULER(0);
	prn_stop();
}

//////////////////////////////////////////////////////////////////////////

bool ServerApp::OnInitialized(int32 argc, char* argv[])
{
	return true;
}

bool ServerApp::OnSetProperty( ServerApp::Properties& appCfg )
{
	return true;
}

void ServerApp::OnQuitApplication()
{
	QuitApplication(eNORMAL_EXIT);
}

void ServerApp::DoCommand( const std::string& sCommand )
{

}

int32 ServerApp::ExitCode( void )
{
	return m_iExitCode;
}

std::string ServerApp::GetSetupFileName()
{
	return m_sSetupFileName;
}

void ServerApp::SetBlockClient( bool bBlock )
{
	m_AppProp.bBlock = bBlock;
}

bool ServerApp::GetBlockClient( void )
{
	return m_AppProp.bBlock;
}

void ServerApp::SetMaxClient( uint32 iMaxClient )
{
	m_AppProp.iMaxClient = iMaxClient;
}

uint32 ServerApp::GetMaxClient( void )
{
	return m_AppProp.iMaxClient;
}

void ServerApp::SetUsers( uint32 iUsers )
{
	m_AppProp.iUsers = iUsers;
}

uint32 ServerApp::GetUsers( void )
{
	return m_AppProp.iUsers;
}

void ServerApp::SetProperty( ServerApp::Properties& Prop )
{
	if ( Prop.iPAgentPort != 0 )
	{
		m_AppProp.iPAgentPort = Prop.iPAgentPort;
		m_AppProp.sPAgentAddr = Prop.sPAgentAddr;
	}

	m_AppProp.iSID = Prop.iSID;
	m_AppProp.iTaskDuration = Prop.iTaskDuration;
	m_AppProp.bLogArchive = Prop.bLogArchive;
	m_AppProp.sLogArchivePath = Prop.sLogArchivePath;
	m_AppProp.sLogBindAddr = Prop.sLogBindAddr;
	m_AppProp.bLogTransmit = Prop.bLogTransmit;
	m_AppProp.sLogSAddr = Prop.sLogSAddr;
	m_AppProp.iLogSPort = Prop.iLogSPort;
	m_AppProp.iCompressOverSize = Prop.iCompressOverSize;
	m_AppProp.iTaskSchedulersCount = Prop.iTaskSchedulersCount;
	m_AppProp.iNotifyTaskSchedulerID = Prop.iNotifyTaskSchedulerID;
	m_AppProp.pPAgentHandler = Prop.pPAgentHandler;
	m_AppProp.iNetworkThreadsCount = Prop.iNetworkThreadsCount;
	m_AppProp.bBlock = Prop.bBlock;
	m_AppProp.iMaxClient = Prop.iMaxClient;
	m_AppProp.iUsers = Prop.iUsers;
}

ServerApp::Properties& ServerApp::GetProperty(void)
{
	return m_AppProp;
}


