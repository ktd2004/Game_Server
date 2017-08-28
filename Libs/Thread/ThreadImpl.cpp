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
#include <MacroFunc.h>
#include <Errorcodes.h>
#include "ThreadUtil.h"
#include "ThreadImpl.h"
//<
ThreadImpl::ThreadImpl()
: m_hThread(INVALID_HANDLE_VALUE)
, m_iThreadId(0)
{
}

ThreadImpl::~ThreadImpl()
{
	if ( m_hThread != INVALID_HANDLE_VALUE )
	{
		TerminateThreadEx(m_hThread);
		Wait();
	}
}

unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
{
	ThreadRunnable* pSelf = (ThreadRunnable*)arg;
	pSelf->Running();
	delete pSelf;
	return 0;
}

bool ThreadImpl::Start( ThreadRunnable *pRunCore, unsigned short iBindCPU )
{
	if ( m_hThread != INVALID_HANDLE_VALUE )
	{
		SetLastError( THREAD_ALREADY_STARTED );
		return false;
	}

	//m_hThread = CreateThreadEx(
	m_hThread = BeginThreadEx(
		NULL,					// Pointer to thread security attributes 
		0,						// Initial thread stack size, in bytes 
		__ThreadEntryPoint,     // Pointer to thread function 
		pRunCore,				//
		CREATE_SUSPENDED,       // Creation flags 
		&m_iThreadId);			// Pointer to returned thread identifier 

	if ( NULL == m_hThread )
	{
		SetLastError( THREAD_CREATE_FAILED );
		return false;
	}
	istrue_do( iBindCPU, SetBindCPU(iBindCPU) );
	return true;
}

bool ThreadImpl::Resume( void )
{
	if ( m_hThread == NULL )
	{
		SetLastError( THREAD_NOT_YET_RUNNING );
		return false;
	}

	::ResumeThread( m_hThread );
	return true;
}

bool ThreadImpl::Terminate( void )
{
	if ( m_hThread == NULL )
	{
		SetLastError( THREAD_NOT_YET_RUNNING );
		return false;
	}
	TerminateThreadEx( m_hThread );
	return true;
}

bool ThreadImpl::Wait( void )
{
	if ( m_hThread == INVALID_HANDLE_VALUE )
	{
		SetLastError( THREAD_NOT_YET_RUNNING );
		return false;
	}
	::WaitForSingleObject( m_hThread, INFINITE );
	::CloseHandle( m_hThread );
	m_hThread = INVALID_HANDLE_VALUE;
	m_iThreadId = 0;
	return true;
}

bool ThreadImpl::SetPriority( int iPrio )
{
	if ( m_hThread == NULL )
	{
		SetLastError( THREAD_NOT_YET_RUNNING );
		return false;
	}

	if ( SetThreadPriority(m_hThread, iPrio) != 0 )
		return true;

	return false;
}

DWORD ThreadImpl::GetAffinity( void )
{
	return GetThreadAffinityMaskEx(m_hThread);
}

unsigned short ThreadImpl::GetBindCPU( void )
{
	DWORD dwMask = GetAffinity();
	for ( unsigned short i = 0; i < 32; i++ )
	{
		DWORD dwOffset = 1 << i;
		if (dwMask & dwOffset)
		{
			return i + 1;
		}
	}
	return 0;
}

void ThreadImpl::SetBindCPU( unsigned short iCPU )
{
	if ( m_hThread && iCPU )
	{
		DWORD dwSetMask;
		dwSetMask = 1 << (iCPU-1);
		HANDLE hProcess = ::GetCurrentProcess();
		DWORD_PTR dwProcessMask;
		DWORD_PTR dwSystemMask;
		GetProcessAffinityMask(hProcess, &dwProcessMask, &dwSystemMask);
		if( (dwProcessMask & dwSetMask) )
		{
			SetThreadAffinityMaskEx(m_hThread, dwSetMask);
		}
	}
}

bool ThreadImpl::SetAffinity( DWORD dwMask )
{
	istrue_ret( !m_hThread || !dwMask, false );

	DWORD_PTR dwProcessMask;
	DWORD_PTR dwSystemMask;
	HANDLE hProcess = GetCurrentProcess();
	isfalse_ret( GetProcessAffinityMask(hProcess, &dwProcessMask, &dwSystemMask), false );

	// 코어의 개수를 얻어와 그 범위 내에서 Affinity 를 설정한다.
	DWORD dwResult = 0;
	for ( int i = 0, j = 0; i < 32; ++i )
	{
		DWORD dwOffset = 1 << i;
		if (dwProcessMask & dwOffset)
		{
			if (dwMask & (1 << j++))
			{
				dwResult = dwResult | dwOffset;
			}
		}
	}

	istrue_ret( SetThreadAffinityMaskEx(m_hThread, dwResult) != 0, true );

	return false;
}