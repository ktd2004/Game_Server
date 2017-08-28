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
#pragma once

#include <conio.h>	// for _kbhit()
#include <CrashDump.h>
#include <Management.h>

/** 
	서버의 기본 기능을 처리하는 클래스
*/
class ServerApp
	: public ICrashHandler
{
public:
	struct Properties
	{
		uint32				iSID;						///< 서버아이디
		uint32				iTaskDuration;				///< 테스크스케줄러의 테스크 처리 간격
		bool				bLogArchive;				///< 로그파일 생성여부
		std::string			sLogArchivePath;			///< 로그파일 생성 경로
		std::string			sLogBindAddr;				///< 로그서버로 로그 전송하기 위해 생성되는 UDPLink의 주소값
		bool				bLogTransmit;				///< 로그서버로 로그 전송 여부
		std::string			sLogSAddr;					///< 로그서버 주소
		uint32				iLogSPort;					///< 로그서버 포트
		uint32				iCompressOverSize;			///< 패킷 압축 여부
		int32				iTaskSchedulersCount;		///< 테스크스케줄러 갯수
		int32				iNotifyTaskSchedulerID;		///< 네트웍에서 수신된 패킷을 처리할 테스크스케줄러ID
		PAgentReceiver*		pPAgentHandler;				///< PAgent의 운영명령을 처리하는 이벤트 핸들러
		int32				iNetworkThreadsCount;		///< 네트웍 쓰레드의 갯수
		std::string			sPAgentAddr;				///< PAgent의 주소
		int32				iPAgentPort;				///< PAgent의 포트
		bool				bBlock;						///< 클라이언트 접속 제한 여부
		uint32				iMaxClient;					///< 최대 클라이언트 수
		uint32				iUsers;						///< 현재 접속된 클라이언트 수
		bool				bSyslog;					///< 시스템 로그 on/off

		Properties()
			: iSID(-1)
			, sPAgentAddr(null_string)
			, iPAgentPort(0)
			, iTaskDuration(100)
			, bLogArchive(true)
			, sLogArchivePath(".\\")
			, sLogBindAddr("localhost")
			, bLogTransmit(false)
			, sLogSAddr("localhost")
			, iLogSPort(4001)
			, iCompressOverSize(0)
			, iTaskSchedulersCount(1)
			, iNotifyTaskSchedulerID(1)
			, pPAgentHandler(nullptr)
			, iNetworkThreadsCount(1)
			, bBlock(false)
			, iMaxClient(2000)
			, iUsers(0)
			, bSyslog(false)
		{
		}
		Properties( const Properties& r )
			: iSID(r.iSID)
			, sPAgentAddr(r.sPAgentAddr)
			, iPAgentPort(r.iPAgentPort)
			, iTaskDuration(r.iTaskDuration)
			, bLogArchive(r.bLogArchive)
			, sLogArchivePath(r.sLogArchivePath)
			, sLogBindAddr(r.sLogBindAddr)
			, bLogTransmit(r.bLogTransmit)
			, sLogSAddr(r.sLogSAddr)
			, iLogSPort(r.iLogSPort)
			, iCompressOverSize(r.iCompressOverSize)
			, iTaskSchedulersCount(r.iTaskSchedulersCount)
			, iNotifyTaskSchedulerID(r.iNotifyTaskSchedulerID)
			, pPAgentHandler(r.pPAgentHandler)
			, iNetworkThreadsCount(r.iNetworkThreadsCount)
			, bBlock(r.bBlock)
			, iMaxClient(r.iMaxClient)
			, iUsers(r.iUsers)
			, bSyslog(r.bSyslog)
		{
		}
	};

protected:
	uint32				m_tCheckDuration;				///< 서버 상태 체크 간격, 1초
	int32				m_argc;							///< 아규먼트 갯수
	char**				m_argv;							///< 아규먼트들
	int32				m_iState;						///< 서버상태
	std::string			m_sSetupFileName;				///< 서버구성파일명
	std::string			m_sSectionName;					///< 서버구성섹션명
	int32				m_iExitCode;					///< 종료코드
	PAgentConnector*	m_PAgent;						///< PAgent 연결관리자
	Properties			m_AppProp;						///< 서버구성값

public:
	ServerApp(int32 argc, char* argv[], bool bArgParse = true);
	virtual ~ServerApp();

private:
	ServerApp(const ServerApp& r) {}

public:
	/** 
		서버가 크래쉬될때 덤프파일명을 설정하기 위해서 호출되는 콜백함수
		@param lpszDumpPath 크래쉬덤프 파일명
	*/
	void OnCrash( IN LPTSTR lpszDumpPath ) override;
	/** 
		커맨드라인을 파싱한다.
		@param argc 아규먼트 갯수
		@param argv 아규먼트들
	*/
	void ParseCommandLine( int32 argc, char* argv[] );
	/** 
		서버의 상태를 체크하는 간격을 설정한다.
		@param tDuration 체크간격(msec)
	*/
	void CheckStateDuration( uint32 tDuration );
	/** 
		서버의 실행파일 이름을 조회한다.
	*/
	std::string		GetProgramName();
	/** 
		서버의 구성파일 이름을 조회한다.
	*/
	std::string		GetSetupFileName();
	/** 
		서버의 섹션명을 조회한다.
	*/
	std::string		GetSectionName();
	/** 
		PAgent가 서버를 실행했는지 조사한다.
	*/
	bool			IsAgentMode();
	/** 
		PAgent의 주소를 조회한다.
	*/
	std::string		GetAgentAddr();
	/** 
		PAgent의 포트를 조회한다.
	*/
	int32			GetAgentPort();
	/** 
		서버아이디를 조회한다.
	*/
	int32			GetSID();
	/** 
		서버아이디를 설정한다.
	*/
	void			SetSID(int32 iSID);
	/** 
		클라이언트 접속 제한을 설정한다.
		@param bBlock 접속제한 여부
	*/
	void			SetBlockClient( bool bBlock );
	/** 
		클라이언트 접속 제한 여부를 조회한다.
	*/
	bool			GetBlockClient( void );
	/** 
		최대 클라이언트수를 설정한다.
		@param iMaxClient 최대클라이언트수
	*/
	void			SetMaxClient( uint32 iMaxClient );
	/** 
		최대 클라이언트수를 조회한다.
	*/
	uint32			GetMaxClient( void );
	/** 
		현재 접속된 클라이언트수를 설정한다.
		@param iUsers 현재 접속된 클라이언트수
	*/
	void			SetUsers( uint32 iUsers );
	/** 
		현재 접속된 클라이언트수를 조회한다.
	*/
	uint32			GetUsers( void );
	/** 
		서버를 종료한다.
		@param iExitCode 종료코드
	*/
	void			QuitApplication(int32 iExitCode);
	/** 
		서버가 실행된면 서버의 구성정보를 얻어오기 위해서 OnSetProperty 함수가 호출된다.
		OnSetProperty 함수내에서 서버구성에 필요한 설정을 한후 SetProperty 함수로 설정값을 전달한다.
		@param Prop 서버설정값
	*/
	void			SetProperty( ServerApp::Properties& Prop );
	/** 
		서버설정값을 조회한다.
	*/
	Properties&		GetProperty( void );
	/** 
		서버구성파일을 로딩한다.
		@param sFileName 서버구성파일명
		@param sSectionName 서버섹션명
	*/
	bool			LoadSetupFile( const std::string& sFileName, const std::string& sSectionName );
	/** 
		서버를 초기화한다.
	*/
	bool			Initialize(void);
	/** 
		서버를 이벤트 처리를 시작한다.
	*/
	virtual void	RunEventLoop(void);
	/** 
		서버종료 코드를 조회한다.
	*/
	int32			ExitCode(void);

protected:
	virtual bool	OnInitialized(int32 argc, char* argv[]);
	virtual bool	OnSetProperty( ServerApp::Properties& appCfg );
	virtual void	OnQuitApplication();
	virtual void	DoCommand( const std::string& sCommand );
};

inline int32 RUN_MAIN(ServerApp* pHldr, const char* pConfigFile,const char* pSectionName, int argc, char* argv[])
{
	if ( !pHldr->LoadSetupFile(pConfigFile, pSectionName) || !pHldr->Initialize() )
	{
		return pHldr->ExitCode();
	}
	pHldr->RunEventLoop();
#if defined(_DEBUG)
	DWORD iProcessId = ::GetCurrentProcessId();
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, iProcessId);
	::TerminateProcess( hProcess, pHldr->ExitCode() );
#endif
	return pHldr->ExitCode();
}

#define RUN_SERVER_APP(app)	\
app Hldr(argc, argv); \
return RUN_MAIN( &Hldr, CONFIG_FILE, SECTION_NAME, argc, argv );