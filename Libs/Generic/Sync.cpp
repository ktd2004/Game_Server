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
#include <Sync.h>

Sync::Sync()
: m_iLocker(0)
, m_iLockCnt(0)
{
#if _WIN32_WINNT<0x0403
#error for InitializeCriticalSectionAndSpinCount you must define this in project property.
	InitializeCriticalSection( &m_CS );
#else
	InitializeCriticalSectionAndSpinCount( &m_CS, 40000 );
#endif
}

Sync::~Sync()
{
	DeleteCriticalSection( &m_CS );
}

bool Sync::Lock( void )
{
	DWORD iThreadID = ::GetCurrentThreadId();
	if ( m_iLocker == iThreadID )
	{
		m_iLockCnt++;
		return true;
	}

	EnterCriticalSection( &m_CS );
	m_iLockCnt++;
	m_iLocker = iThreadID;

	return true;
}

bool Sync::Unlock( void )
{
	DWORD iThreadID = ::GetCurrentThreadId();
	if ( m_iLocker != iThreadID )
	{
		return false;
	}

	if ( m_iLockCnt - 1 == 0 )
	{
		m_iLockCnt--;
		m_iLocker = 0;
		LeaveCriticalSection( &m_CS );
	}

	return true;
}
