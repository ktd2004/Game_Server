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

/** 
	�̴ϴ����� �����ɶ� ���ϸ��� �����ϱ� ���ؼ� ȣ��Ǵ� ũ���� �ڵ鷯.
*/
struct ICrashHandler
{
	virtual void OnCrash( IN LPTSTR lpszDumpPath ) = 0;
};

/** 
	�̴������� �����ϴ� Ŭ����.

	minidump �����̹Ƿ�, !analyze -v ������ ���� �� �ִ�.
	dbghelp.dll�� ����ϴµ�, ���� .exe�� module path ���� system32 path ������ LoadLibrary�� �õ��Ѵ�.

	����)
	CrashDump::CrashDump(...) / CrashDump::RegisterExectionFilter(...)�� Argument�� �����Ѵ�.
	* lpszDumpPreFixName : ���������� �̸��� �����Ѵ�. �������� �ʴ� ���, ���� .exe�� ���ϸ��� �ȴ�.
		ex) "abc"�� �����ϴ� ���, abc_yyyymmddhhmmssmmm.dmp ���ϸ����� ���� �����ȴ�.
	* nMiniDumpType : ���������� Ÿ�� ����. �⺻���� MiniDumpNormal�̴�.
		**) Ÿ���� http://msdn.microsoft.com/en-us/library/ms680519(VS.85).aspx ���� �����Ͻÿ�.
	* lpszDumpPath : ���������� ������ Path. �⺻���� .exe�� Path �Ϻ��� Temp �����̴�.
		ex) �⺻���� ���, C:\ttt\abc.exe�� C:\ttt\temp\abc_~~~~.dmp�� ����ȴ�)
*/
class CrashDump
{
public:
	CrashDump();
	virtual ~CrashDump();

public:
	/** 
		�̴ϴ����� �����ɶ� ȣ��Ǵ� ũ���� �ڵ鷯 ����
		@param pCallback ũ���� �ڵ鷯�� �ּҰ�
	*/
	static BOOL RegisterExceptionFilter( IN ICrashHandler* pCallback );

private:
	static LONG WINAPI CbTopLevelExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionInfo);
	static HMODULE	LoadLibrary_DbgHelp(VOID);
	static HMODULE	LoadLibraryFromSystem(IN LPCTSTR lpFileName);
	static BOOL		GetDumpFileFullPathName(
		IN LPCTSTR lpszPath,
		OUT LPTSTR lpszDumpFileFullPathName,
		IN DWORD dwCchDumpFileFullPathName,
		OUT PSYSTEMTIME pstTime);
};