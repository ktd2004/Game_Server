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
	미니덤프가 생성될때 파일명을 설정하기 위해서 호출되는 크래쉬 핸들러.
*/
struct ICrashHandler
{
	virtual void OnCrash( IN LPTSTR lpszDumpPath ) = 0;
};

/** 
	미니텀프를 생성하는 클래스.

	minidump 생성이므로, !analyze -v 정도로 사용될 수 있다.
	dbghelp.dll을 사용하는데, 실행 .exe의 module path 다음 system32 path 순서로 LoadLibrary를 시도한다.

	참고)
	CrashDump::CrashDump(...) / CrashDump::RegisterExectionFilter(...)의 Argument를 설명한다.
	* lpszDumpPreFixName : 덤프파일의 이름을 지정한다. 지정되지 않는 경우, 실행 .exe의 파일명이 된다.
		ex) "abc"로 지정하는 경우, abc_yyyymmddhhmmssmmm.dmp 파일명으로 강제 지정된다.
	* nMiniDumpType : 덤프파일의 타입 지정. 기본값은 MiniDumpNormal이다.
		**) 타입은 http://msdn.microsoft.com/en-us/library/ms680519(VS.85).aspx 에서 참고하시오.
	* lpszDumpPath : 덤프파일을 생성할 Path. 기본값은 .exe의 Path 하부의 Temp 폴더이다.
		ex) 기본값인 경우, C:\ttt\abc.exe는 C:\ttt\temp\abc_~~~~.dmp로 저장된다)
*/
class CrashDump
{
public:
	CrashDump();
	virtual ~CrashDump();

public:
	/** 
		미니덤프가 생성될때 호출되는 크래쉬 핸들러 설정
		@param pCallback 크래쉬 핸들러의 주소값
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