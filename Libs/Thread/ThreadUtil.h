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

#include <windows.h>

/** 
	�����带 �����Ѵ�.
*/
HANDLE		BeginThreadEx(
	void* pSecurity,
	unsigned int iStackSize,
	unsigned int (__stdcall* pStartAddress) (void *),
	LPVOID lpParameter,
	unsigned int iInitFlag,
	LPDWORD lpThreadId);
/** 
	�����带 �����Ѵ�.
*/
HANDLE			CreateThreadEx(
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	DWORD dwCreationFlags,
	LPDWORD lpThreadId);
/** 
	������ ��ȣ�� ����Ʈ ���·� �ǵ���
*/
DWORD_PTR	ResetThreadAffinityMaskEx( HANDLE hThread );
/** 
	������ ��ȣ�� ����ũ ��ȸ
*/
DWORD		GetThreadAffinityMaskEx	( HANDLE hThread );
/** 
	������ ��ȣ�� ����ũ ����
*/
DWORD_PTR	SetThreadAffinityMaskEx	( HANDLE hThread, DWORD dwThreadAffinityMask );
/** 
	�����带 �����Ѵ�.
*/
void		TerminateThreadEx( HANDLE hThread );
