#include "stdafx.h"
#include <CrashDump.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdlib.h>
#include <dbghelp.h>

// SelfDump�� ���� �Ķ����
namespace
{
	// dbghelp.dll::MinuDumpWriteDump(...)�� prototype
	typedef BOOL (WINAPI *LPFN_MinuDumpWriteDump)(
		HANDLE								hProcess, 
		DWORD								dwPid, 
		HANDLE								hFile, 
		MINIDUMP_TYPE						DumpType,
		PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam,
		PMINIDUMP_USER_STREAM_INFORMATION	UserStreamParam,
		PMINIDUMP_CALLBACK_INFORMATION		CallbackParam);

	static TCHAR			g_szDumpPath			 [1024 * 2] = {0,};
	static MINIDUMP_TYPE	g_nMiniDumpType			= MiniDumpNormal;
	static ICrashHandler*	g_pCallback				= NULL;
}

CrashDump::CrashDump()
{
}

CrashDump::~CrashDump()
{
}

LONG CrashDump::CbTopLevelExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	LONG							lRtnValue					= EXCEPTION_CONTINUE_SEARCH;
	HANDLE							hFile						= INVALID_HANDLE_VALUE;
	BOOL							bRtnValue					= FALSE;
	TCHAR							szPath[1024 * 2]			= {0,};
	HMODULE							hDll						= NULL;
	LPFN_MinuDumpWriteDump			pfn							= NULL;
	SYSTEMTIME						stTime						= {0,};
	_MINIDUMP_EXCEPTION_INFORMATION	stExceptInfo				= {0,};

	// Callback ��� �Ǿ� ������ �켱 ȣ���Ѵ�.
	if (NULL != g_pCallback)
	{
		g_pCallback->OnCrash(g_szDumpPath);
	}

	if ( g_szDumpPath[0] == NULL )
	{
		GetModuleFileName(NULL, g_szDumpPath, sizeof(g_szDumpPath));
	}

	// ����, MiniDumpType�� -1 �̶��,... ����
	if ((MINIDUMP_TYPE)-1 == g_nMiniDumpType)
	{
		lRtnValue = EXCEPTION_CONTINUE_SEARCH;
		goto FINAL;
	}

	// DbgHelp.dll�� �ε��Ѵ�.
	hDll = CrashDump::LoadLibrary_DbgHelp();
	if (NULL == hDll)
	{
		lRtnValue = EXCEPTION_CONTINUE_SEARCH;
		goto FINAL;
	}

	// DbgHelp.dll::MinuDumpWriteDump(...)�� ���Ѵ�.
	pfn = (LPFN_MinuDumpWriteDump)::GetProcAddress(hDll, "MiniDumpWriteDump");
	if (NULL == pfn)
	{
		lRtnValue = EXCEPTION_CONTINUE_SEARCH;
		goto FINAL;
	}

	::GetLocalTime(&stTime);
	// Dump ���� �̸��� ���Ѵ�.
	if (FALSE == CrashDump::GetDumpFileFullPathName(g_szDumpPath, 
													szPath, 
													MAX_PATH, 
													&stTime))
	{

		lRtnValue = EXCEPTION_CONTINUE_SEARCH;
		goto FINAL;
	}

	// Dump ������ Create �Ѵ�.
	hFile = ::CreateFile(szPath, 
						 GENERIC_WRITE, 
						 0, 
						 NULL, 
						 CREATE_ALWAYS, 
						 FILE_ATTRIBUTE_NORMAL, 
						 NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		lRtnValue = EXCEPTION_CONTINUE_SEARCH;
		goto FINAL;
	}

	// ���� ������ ���� ���� �ʱ�ȭ
	stExceptInfo.ThreadId			= ::GetCurrentThreadId();
	stExceptInfo.ExceptionPointers	= pExceptionInfo;
	stExceptInfo.ClientPointers		= NULL;

	// Dump ����
	bRtnValue = (*pfn)(::GetCurrentProcess(),
					   ::GetCurrentProcessId(), 
					   hFile, 
					   MiniDumpNormal, 
					   &stExceptInfo, 
					   NULL, 
					   NULL);
	if (TRUE == bRtnValue)
	{
		// EXCEPTION_EXECUTE_HANDLER ���� �ϸ�,
		// drwtsn32.exe�� ���� ����Ÿ� ���� ������ ���� �ʴ´�.
		// lRtnValue = EXCEPTION_EXECUTE_HANDLER;
		lRtnValue = EXCEPTION_CONTINUE_SEARCH;
	}
	else
	{
		lRtnValue = EXCEPTION_CONTINUE_SEARCH;
	}

FINAL:

	if (INVALID_HANDLE_VALUE != hFile)
	{
		::CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
		pfn   = NULL;
	}

	if (NULL != hDll)
	{
		::FreeLibrary(hDll);
		hDll = NULL;
		pfn  = NULL;
	}

	return lRtnValue;
}

BOOL CrashDump::RegisterExceptionFilter(
	IN ICrashHandler* pCallback )
{
	BOOL	bRtnValue	= TRUE;
	DWORD	dwRtnValue	= 0;

	g_pCallback		= pCallback;

	// Exception Filter�� ����Ѵ�.
	::SetUnhandledExceptionFilter(CrashDump::CbTopLevelExceptionFilter);

	bRtnValue = TRUE;

	return bRtnValue;
}

BOOL CrashDump::GetDumpFileFullPathName(
	IN LPCTSTR lpszPath,
	OUT LPTSTR lpszDumpFileFullPathName,
	IN DWORD dwCchDumpFileFullPathName,
	OUT PSYSTEMTIME pstTime)
{
	BOOL bRtnValue	= TRUE;

	if ((NULL == lpszPath) || 
		(NULL == lpszDumpFileFullPathName) ||
		(NULL == pstTime))
	{
		bRtnValue = FALSE;
		goto FINAL;
	}

	::GetLocalTime(pstTime);
	StringCchPrintf(lpszDumpFileFullPathName, 
					dwCchDumpFileFullPathName, 
					TEXT("%s_%.4d%.2d%.2d_%.2d%.2d%.2d.dmp"), 
					lpszPath, 
					pstTime->wYear, 
					pstTime->wMonth, 
					pstTime->wDay, 
					pstTime->wHour, 
					pstTime->wMinute, 
					pstTime->wSecond);
	bRtnValue = TRUE;
FINAL:
	return bRtnValue;
}

HMODULE CrashDump::LoadLibrary_DbgHelp(VOID)
{
	HMODULE	hDLL							= NULL;
	TCHAR	szDbgHelp[MAX_PATH]				= {0,};
	LPTSTR	lpszFound						= NULL;

	if (0 != ::GetModuleFileName(NULL, szDbgHelp, MAX_PATH))
	{
		// .exe�� Path�� DbgHelp.dll�� �ִٸ�, �̰��� LoadLibrary �Ѵ�.
		lpszFound = _tcsrchr(szDbgHelp, TEXT('\\'));
		if ((NULL != lpszFound) && (szDbgHelp < lpszFound))
		{
			*lpszFound = TEXT('\0');
			StringCchCat(szDbgHelp, MAX_PATH, TEXT("\\DbgHelp.dll"));

			hDLL = ::LoadLibrary(szDbgHelp);
			if (NULL != hDLL)
			{
				// success!
				goto FINAL;
			}
		}
	}

	// System32 ������ ���� dbghelp.dll�� LoadLibrary �Ѵ�.
	hDLL = CrashDump::LoadLibraryFromSystem(TEXT("DbgHelp.dll"));

FINAL:
	return hDLL;
}

HMODULE CrashDump::LoadLibraryFromSystem(IN LPCTSTR lpFileName)
{
	if( NULL == lpFileName ) {
		return NULL;
	}

	if( 0 == lpFileName[0] ) {
		return NULL;
	}

	const int BUF_SIZ = 4096;

	LPTSTR lpszSysDir = NULL;
	UINT nNeedSize = GetSystemDirectory( NULL, 0 );
	lpszSysDir = (LPTSTR)malloc( (nNeedSize + 1) * sizeof(TCHAR) );
	if( NULL == lpszSysDir ) {
		return NULL;
	}

	if( 0 == GetSystemDirectory( lpszSysDir, nNeedSize + 1 ) ) {
		free( lpszSysDir );
		lpszSysDir = NULL;
		return NULL;
	}

	TCHAR szFullPath[BUF_SIZ] = {0,};
	_tcsncpy_s( szFullPath, sizeof(szFullPath)-1, lpszSysDir, BUF_SIZ );
	_tcsncat_s( szFullPath, sizeof(szFullPath)-1, _T("\\"), BUF_SIZ );
	_tcsncat_s( szFullPath, sizeof(szFullPath)-1, lpFileName, BUF_SIZ );

	HMODULE hModule = LoadLibraryEx( szFullPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH );

	free( lpszSysDir );
	lpszSysDir = NULL;

	if( hModule ) {
		return hModule;
	}
	
	return NULL;
}
/*
DWORD CrashDump::GetDefaultModuleTempPath(OUT LPTSTR lpszPath, IN DWORD dwCchPath)
{
	DWORD	dwRtnValue	= ERROR_SUCCESS;
	LPTSTR	lpszFound	= NULL;

	if (NULL == lpszPath)
	{
		dwRtnValue = ERROR_INVALID_PARAMETER;
		goto FINAL;
	}

	// Clean Up
	ZeroMemory(lpszPath, sizeof(TCHAR) * dwCchPath);

	if (0 == ::GetModuleFileName(NULL, lpszPath, dwCchPath))
	{
		// Module Handle ���ϴµ� ����
		dwRtnValue = ::GetLastError();
		goto FINAL;
	}

	lpszFound = _tcsrchr(lpszPath, TEXT('\\'));
	if (NULL == lpszFound)
	{
		// Path ���� \�� ����?
		dwRtnValue = ERROR_PATH_NOT_FOUND;
		goto FINAL;
	}

	if (lpszFound <= lpszPath)
	{
		// Path ���� \�� ����?
		dwRtnValue = ERROR_PATH_NOT_FOUND;
		goto FINAL;
	}

	// Null ���ڷ� �������
	*lpszFound = TEXT('\0');

	// Temp ������ �߰�����
	//StringCchCat(lpszPath, dwCchPath, TEXT("\\Temp"));

	// ������� �Դٸ�, ����
	dwRtnValue = ERROR_SUCCESS;

FINAL:
	return dwRtnValue;
}
*/
/*
DWORD CrashDump::GetDefaultDumpPreFixName(OUT LPTSTR lpszDefaultDumpPreFixName, IN DWORD dwCchDefaultDumpPreFixName)
{
	DWORD	dwRtnValue						= ERROR_SUCCESS;
	TCHAR	szModulePath[eMAX_PATH_SELFDUMP]	= {0,};
	LPTSTR	lpszFound						= NULL;
	LPTSTR	lpszIter						= NULL;
	BOOL	bFound							= FALSE;
	INT		i								= 0;

	// ���� .exe�� �����̸� (Ȯ���� ����)�� dumpfile�� prefix�� �����Ѵ�.
	if (NULL == lpszDefaultDumpPreFixName)
	{
		dwRtnValue = ERROR_INVALID_PARAMETER;
		goto FINAL;
	}

	ZeroMemory(lpszDefaultDumpPreFixName, sizeof(TCHAR)*dwCchDefaultDumpPreFixName);

	if (0 == ::GetModuleFileName(NULL, szModulePath, eMAX_PATH_SELFDUMP))
	{
		dwRtnValue = ::GetLastError();
		StringCchPrintf(lpszDefaultDumpPreFixName, dwCchDefaultDumpPreFixName, TEXT("UnKnown"));
		goto FINAL;
	}

	lpszFound = _tcsrchr(szModulePath, TEXT('\\'));
	if ((NULL == lpszFound) || (lpszFound <= szModulePath) || (TEXT('\0') == *(lpszFound+1)))
	{
		dwRtnValue = ERROR_PATH_NOT_FOUND;
		StringCchPrintf(lpszDefaultDumpPreFixName, dwCchDefaultDumpPreFixName, TEXT("UnKnown"));
		goto FINAL;
	}

	// Ȯ���ڸ� �����Ѵ�.
	bFound = FALSE;
	for (i=0; i<MAX_PATH; i++)
	{
		if (TEXT('\0') == *(lpszFound+i))
		{
			break;
		}

		if (TEXT('.') == *(lpszFound+i))
		{
			bFound = TRUE;
			*(lpszFound+i) = TEXT('\0');
			break;
		}
	}

	if (FALSE == bFound)
	{
		dwRtnValue = ERROR_PATH_NOT_FOUND;
		StringCchPrintf(lpszDefaultDumpPreFixName, dwCchDefaultDumpPreFixName, TEXT("UnKnown"));
		goto FINAL;
	}

	dwRtnValue = ERROR_SUCCESS;
	StringCchCopy(lpszDefaultDumpPreFixName, dwCchDefaultDumpPreFixName, lpszFound + 1);

FINAL:
	return dwRtnValue;
}
*/