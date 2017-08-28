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
//<
#include <Errorcodes.h>
#include <StringUtil.h>
#include "ProcessImpl.h"

//<
ProcessImpl::ProcessImpl()
	: m_iExitCode(0)
	, m_pUserData(nullptr)
	, m_pHandler(nullptr)
	, m_iRestartCnt(0)
	, m_bRestart(true)
{
	memset( &m_ProcessInfo, 0x00, sizeof(PROCESS_INFORMATION) );
}

ProcessImpl::~ProcessImpl()
{
	safe_delete_handle( m_ProcessInfo.hProcess );
	safe_delete_handle( m_ProcessInfo.hThread );
	if ( m_pUserData && 0 == m_pUserData->decref() )
	{
		interface_ref* pTmpUserData = (interface_ref*)locked_exchange_pointer(m_pUserData, nullptr);
		safe_delete(pTmpUserData);
	}
}

bool ProcessImpl::Spawn( const ProcessOption& Opt, ProcessHandler* pHandler )
{
	safe_delete_handle( m_ProcessInfo.hProcess );
	safe_delete_handle( m_ProcessInfo.hThread );

	m_Opt = Opt;
	m_pHandler = pHandler;

	STARTUPINFO si ; 
	memset( &si, 0, sizeof(STARTUPINFO) ); 
	si.cb = sizeof(STARTUPINFO); 
// 아래코드를 살리면 표준입출력을 사용하지 못한다.
// 	si.dwFlags = STARTF_USESTDHANDLES; 
// 	si.wShowWindow = SW_SHOWNORMAL; //SW_HIDE; 

	// 실행파일위치
	std::string sExec = m_Opt.Path();
	sExec += "\\";
	sExec += m_Opt.Exec();

	// 실행파일명에 확장자가 없어야 한다.
	std::string sCmd = m_Opt.Exec();
	sCmd = util::pairkey( sCmd, "." );
	sCmd += " ";
	sCmd += m_Opt.CommandLine();

	BOOL bRet = ::CreateProcessA(
		(char*)sExec.c_str(),		// application name
		(char*)sCmd.c_str(),		// command line
		NULL,						// process security attributes
		NULL,						// thread security attributes
		m_Opt.InheritHandle(),		// inherit handle
		m_Opt.CreationFlag(),		// creation flags
		m_Opt.Environment(),		// environment
		m_Opt.WorkingDirectory(),	// current directory
		&si,						// startup info
		&m_ProcessInfo	);			// process information

	if ( !bRet )
	{
		m_iExitCode = GetLastError();
		return false;
	}

	if ( !m_Opt.Background() )
	{
		int iRet = Wait( INFINITE );
		safe_delete_handle( m_ProcessInfo.hProcess );
		safe_delete_handle( m_ProcessInfo.hThread );
		return iRet != -1 ? true : false;
	}

	return true;
}


bool ProcessImpl::IsRunning( void )
{
	if ( m_ProcessInfo.hProcess == NULL )
	{
		SetLastError( PROCESS_NOT_YET_RUNNING );
		return false;
	}

	DWORD iExitCode;
	BOOL bRet = ::GetExitCodeProcess(m_ProcessInfo.hProcess, &iExitCode);
	if ( bRet && iExitCode == STILL_ACTIVE )
	{
		return true;
	}
	m_iExitCode = iExitCode;
	return false;
}

int ProcessImpl::Wait( unsigned long iTm )
{
	if ( m_ProcessInfo.hProcess == NULL )
	{
		SetLastError( PROCESS_NOT_YET_RUNNING );
		return -1;
	}

	switch (::WaitForSingleObject(m_ProcessInfo.hProcess, iTm))
	{
	case WAIT_OBJECT_0:
		::GetExitCodeProcess(m_ProcessInfo.hProcess, &m_iExitCode);
		return 1;
	case WAIT_TIMEOUT:
		return 0;
	default:
		return -1;
	}
}

bool ProcessImpl::Terminate( void )
{
	if ( m_ProcessInfo.hProcess == NULL )
	{
		SetLastError( PROCESS_NOT_YET_RUNNING );
		return false;
	}

	::TerminateProcess( m_ProcessInfo.hProcess, 0 );
	::CloseHandle( m_ProcessInfo.hProcess );

	return true;
}

void ProcessImpl::UserData(interface_ref* pUserData)
{
	if ( pUserData )
	{ pUserData->incref(); }

	if ( m_pUserData && 0 == m_pUserData->decref() )
	{
		interface_ref* pTmpUserData = (interface_ref*)locked_exchange_pointer(m_pUserData, nullptr);
		safe_delete(pTmpUserData);
	}
	locked_exchange_pointer(m_pUserData, pUserData);
}

interface_ref* ProcessImpl::UserData(void)
{
	return m_pUserData;
}

bool ProcessImpl::IsRestart( void )
{
	return m_bRestart;
}

void ProcessImpl::SetRestart( bool bRestart )
{
	m_bRestart = bRestart;
}

unsigned int ProcessImpl::GetRestartCount( void )
{
	return m_iRestartCnt;
}

HANDLE ProcessImpl::GetHandle( void )
{
	return m_ProcessInfo.hProcess;
}

void ProcessImpl::SetHandle( HANDLE hProcess )
{
	m_ProcessInfo.hProcess = hProcess;
}

unsigned long ProcessImpl::GetProcessId( void )
{
	return m_ProcessInfo.dwProcessId;
}

void ProcessImpl::SetProcessId( unsigned long iProcessId )
{
	m_ProcessInfo.dwProcessId = iProcessId;
}

DWORD ProcessImpl::ExitCode( void )
{
	return m_iExitCode;
}
