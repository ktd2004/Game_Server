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
#include <process.h>
#include <map>
//<
#include <MacroFunc.h>
#include <Sync.h>
#include "ThreadUtil.h"
//<
namespace
{	
	typedef std::map<HANDLE, DWORD>	ThreadAffinityMaskMap;
	ThreadAffinityMaskMap	g_ThreadAffinityMaskMap;
	Sync					g_ThreadSync;
};

void TerminateThreadEx( HANDLE hThread )
{
	Guard lock( g_ThreadSync );
	isnull_handle_ret( hThread, );
	::TerminateThread( hThread, 0 );
	g_ThreadAffinityMaskMap.erase( hThread );
}

DWORD_PTR SetThreadAffinityMaskEx( HANDLE hThread, DWORD dwThreadAffinityMask )
{
	Guard lock( g_ThreadSync );
	DWORD_PTR oldmask = ::SetThreadAffinityMask(hThread, dwThreadAffinityMask);
	if ( 0 != oldmask )
		g_ThreadAffinityMaskMap[hThread] = dwThreadAffinityMask;
	return oldmask;
}

DWORD GetThreadAffinityMaskEx( HANDLE hThread )
{
	Guard lock( g_ThreadSync );
	ThreadAffinityMaskMap::iterator it = g_ThreadAffinityMaskMap.find(hThread);
	istrue_ret( it == g_ThreadAffinityMaskMap.end(), 0 );
	return it->second;
}

DWORD_PTR ResetThreadAffinityMaskEx(HANDLE hThread)
{
	DWORD_PTR oldmask = SetThreadAffinityMaskEx(hThread, 0x1);
	if (0 == oldmask)
		return 0;
	return SetThreadAffinityMaskEx(hThread, (DWORD)oldmask);
}

HANDLE CreateThreadEx(
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		SIZE_T dwStackSize,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		DWORD dwCreationFlags,
		LPDWORD lpThreadId )
{
	DWORD _lpThreadId;
	if (NULL == lpThreadId)
		lpThreadId = &_lpThreadId;
	HANDLE handle =  CreateThread(
		lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	return handle;
}

HANDLE BeginThreadEx(
		void * pSecurity,
		unsigned int iStackSize,
		unsigned int (__stdcall * pStartAddress) (void *),
		LPVOID lpParameter,
		unsigned int iInitFlag,
		LPDWORD lpThreadId )
{
	uintptr_t handle = _beginthreadex(
		pSecurity, iStackSize, pStartAddress, lpParameter, iInitFlag, (unsigned int*)lpThreadId);
	return (HANDLE)handle;
}