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
//<
#include <string>
#include <Ptr.h>

/** 
	프로세스의 경로, 전달할 매개변수, 콘솔의 부착 여부, 백그라운드 실행 여부, 재실행 횟수등의 프로세스 실행 옵션을 지정한다.

	@verbatim
	ProcessOption	opt;
	opt.Path( "C:\\WINDOWS\\system32" );
	opt.Exec( "notepad.exe" );
	opt.CommandLine( "" );
	opt.WorkingDirectory( ".\\" );
	opt.CreationFlag( CREATE_NEW_CONSOLE );
	opt.Environment( "this=is" );
	opt.MaxRestartCnt( 3 );

	ProcessFactory::Spawn( opt, nullptr );
	@endverbatim
*/
class ProcessOption
{
public:
	std::string			m_sExecPath;		///< 프로세스의 경로
	std::string			m_sExec;			///< 프로세스의 이름
	std::string			m_sCommandLine;		///< 프로세스에 전달할 매개변수
	std::string			m_sDirectory;		///< 프로세스의 실행 디렉토리
	std::string			m_sEnvironment;		///< 프로세스의 환경변수
	unsigned long		m_iCreationFlag;	///< 프로세스 생성 옵션
	int					m_iInheritHandle;	///< 핸들 상속 여부
	bool				m_bBackground;		///< 백그라운드로 실행 여부
	unsigned short		m_iMaxRestartCnt;	///< 재실행 횟수

public:
	ProcessOption();
	ProcessOption( const ProcessOption& r );
	virtual ~ProcessOption();
	/** 
		프로세스의 경로를 설정한다.
		@param sExecPath 프로세스의 경로
	*/
	void			Path( const std::string& sExecPath );
	/** 
		프로세스의 경로를 조회한다.
	*/
	char*			Path( void );
	/** 
		프로세스의 이름을 설정한다.
		@param sExecPath 프로세스의 경로
	*/
	void			Exec( const std::string& sExec );
	/** 
		프로세스의 경로를 조회한다.
	*/
	char*			Exec( void );
	/** 
		프로세스에 전달할 아규먼트를 설정한다.
		@param sCommandLine 프로세스에 전달할 아규먼트
	*/
	void			CommandLine( const std::string& sCommandLine );
	/** 
		프로세스에 전달할 아규먼트를 조회한다.
	*/
	char*			CommandLine( void );
	/** 
		프로세스가 현재의 디렉토리를 설정한다.
		@param sDirectory 프로세스의 현재 디렉토리
	*/
	void			WorkingDirectory( const std::string& sDirectory );
	/** 
		프로세스가 현재의 디렉토리를 조회한다.
	*/
	char*			WorkingDirectory( void );
	/** 
		프로세스의 환경변수를 설정한다.
		@param sEnv 프로세스의 환경변수
	*/
	void			Environment( const std::string& sEnv );
	/** 
		프로세스의 환경변수를 조회한다.
	*/
	char*			Environment( void );
	/** 
		프로세스의 생성 옵션을 설정한다.
		@param iFlag 프로세스의 생성옵션
		참고 ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.ko/dllproc/base/process_creation_flags.htm
	*/
	void			CreationFlag( unsigned long iFlag );
	/** 
		프로세스의 생성 옵션을 조회한다.
	*/
	unsigned long	CreationFlag( void );
	/** 
		프로세스 핸들의 상속 여부를 설정한다.
		@param bInhrerit 프로세스 핸들의 상속 여부
	*/
	void			InheritHandle( bool bInhrerit );
	/** 
		프로세스 핸들의 상속 여부를 조회한다.
	*/
	bool			InheritHandle( void );
	/** 
		프로세스를 백그라운드로 실행할지 여부를 설정한다.
		@param bBackground 프로세스를 백그라운드로 실행할지 여부
	*/
	void			Background( bool bBackground );
	/** 
		프로세스를 백그라운드로 실행할지 여부를 조회한다.
	*/
	bool			Background( void );
	/** 
		프로세스 비정상 종료시 재실행 횟수를 설정한다.
		@param iMaxRestartCnt 프로세스 재실행 횟수
	*/
	void			MaxRestartCnt( unsigned short iMaxRestartCnt );
	/** 
		프로세스 비정상 종료시 재실행 횟수를 조회한다.
	*/
	unsigned short	MaxRestartCnt( void );
};

struct ProcessHandler;
/** 
	프로세스 객체의 스마트 포인터 타입 선언.
*/
PTR(Process);
/** 
	프로세스의 실행 정보를 관리하는 객체 클래스.
*/
class Process : public single_ref
{
public:
	Process() {}
	virtual ~Process() {}

	/** 
		프로세스를 실행한다.
		@param opt 프로세스 실행 옵션
		@verbatim
		ProcessOption	opt;
		opt.Path( "C:\\WINDOWS\\system32" );
		opt.Exec( "notepad.exe" );
		opt.CommandLine( "" );
		opt.WorkingDirectory( ".\\" );
		opt.CreationFlag( CREATE_NEW_CONSOLE );
		opt.Environment( "this=is" );
		opt.MaxRestartCnt( 3 );

		ProcessFactory::Spawn( opt, nullptr );
		@endverbatim
	*/
	virtual bool			Spawn( const ProcessOption& opt, ProcessHandler* pHandler ) = 0;
	/** 
		프로세스가 실행 상태인지 조회한다.
	*/
	virtual bool			IsRunning( void ) = 0;
	/** 
		프로세스가 종료하기를 대기한다.
		@param iTm 프로세스의 종료를 대기하는 시간(ms)
	*/
	virtual int				Wait( unsigned long iTm ) = 0;
	/** 
		프로세스를 종료한다.
	*/
	virtual bool			Terminate( void ) = 0;
	/** 
		프로세스의 핸들을 조회한다.
	*/
	virtual HANDLE			GetHandle( void ) = 0;
	/** 
		프로세스의 ID를 조회한다.
	*/
	virtual unsigned long	GetProcessId( void ) = 0;
	/** 
		프로세스의 종료코드를 조회한다.
	*/
	virtual DWORD			ExitCode( void ) = 0;
	/** 
		프로세스에 사용자 객체를 설정한다.
		@param pUserData 프로세스에 저장할 사용자 객체
	*/
	virtual void			UserData( interface_ref* pUserData ) = 0;
	/** 
		프로세스의 사용자 객체를 조회한다.
	*/
	virtual interface_ref*	UserData(void) = 0;
	/** 
		프로세스의 재시작 여부를 조회한다.
	*/
	virtual bool			IsRestart( void ) = 0;
	/** 
		프로세스의 재시작 여부를 설정한다.
	*/
	virtual void			SetRestart( bool bRestart ) = 0;
	/** 
		프로세스의 재시작 횟수를 조회한다.
	*/
	virtual unsigned int	GetRestartCount( void ) = 0;
};