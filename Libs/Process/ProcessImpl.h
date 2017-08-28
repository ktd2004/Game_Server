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

#include <DataTypes.h>
#include <Ptr.h>
#include <ProcessFactory.h>

/** 
	프로세스의 실행 정보를 관리하는 객체 클래스.
*/
class ProcessImpl : public Process
{
public:
	DWORD					m_iExitCode;
	PROCESS_INFORMATION		m_ProcessInfo;
	interface_ref*			m_pUserData;
	ProcessHandler*			m_pHandler;
	ProcessOption			m_Opt;
	unsigned int			m_iRestartCnt;
	bool					m_bRestart;

public:
	ProcessImpl();
	ProcessImpl(const ProcessImpl& r) {}
	virtual ~ProcessImpl();
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
	bool				Spawn( const ProcessOption& opt, ProcessHandler* pHandler ) override;
	/** 
		프로세스가 실행 상태인지 조회한다.
	*/
	bool				IsRunning( void ) override;
	/** 
		프로세스가 종료하기를 대기한다.
		@param iTm 프로세스의 종료를 대기하는 시간(ms)
	*/
	int					Wait( unsigned long iTm ) override;
	/** 
		프로세스를 종료한다.
	*/
	bool				Terminate( void ) override;
	/** 
		프로세스의 핸들을 조회한다.
	*/
	HANDLE				GetHandle( void ) override;
	/** 
		프로세스의 핸들을 설정한다.
	*/
	void				SetHandle( HANDLE hProcess );
	/** 
		프로세스의 ID를 조회한다.
	*/
	unsigned long		GetProcessId( void ) override;
	/** 
		프로세스의 ID를 설정한다.
	*/
	void				SetProcessId( unsigned long iProcessId );
	/** 
		프로세스의 종료코드를 얻어온다.
	*/
	DWORD				ExitCode( void ) override;
	/** 
		프로세스에 사용자 객체를 설정한다.
		@param pUserData 프로세스에 저장할 사용자 객체
	*/
	void				UserData(interface_ref* pUserData) override;
	/** 
		프로세스의 사용자 객체를 조회한다.
	*/
	interface_ref*		UserData(void) override;
	/** 
		프로세스의 재시작 여부를 조회한다.
	*/
	bool				IsRestart( void ) override;
	/** 
		프로세스의 재시작 여부를 설정한다.
	*/
	void				SetRestart( bool bRestart ) override;
	/** 
		프로세스의 재시작 횟수를 조회한다.
	*/
	unsigned int		GetRestartCount( void ) override;
};
