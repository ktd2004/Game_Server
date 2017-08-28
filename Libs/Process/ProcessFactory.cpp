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
#include "ProcessImpl.h"
#include "ProcessManager.h"
#include <ProcessFactory.h>
#include <TaskSchedulerFactory.h>
#include <Logger.h>

//<
namespace ProcessFactory
{
	ProcessManager		g_ProcessMgr;

	bool Start( unsigned int iTaskSchedulerID, unsigned long iDuration )
	{
		return CREATE_TASK( iTaskSchedulerID, &g_ProcessMgr, &ProcessManager::EventTick, iDuration ) > 0;
	}

	ProcessPtr Spawn( const ProcessOption& Opt, ProcessHandler* pHandler, interface_ref* pUserData )
	{
		return g_ProcessMgr.Launch( Opt, pHandler, pUserData );
	}

	ProcessPtr Spawn( const ProcessOption& Opt )
	{
		ProcessImpl* pProcess = new ProcessImpl;
		if ( pProcess )
		{
			if (pProcess->Spawn(Opt, nullptr))
			{
				return pProcess;
			}
		}
		safe_delete(pProcess);
		return NULL;
	}

	bool Terminate( DWORD iProcessId )
	{
		return g_ProcessMgr.Terminate( iProcessId );
	}

	ProcessPtr GetProcess( DWORD iProcessId )
	{
		return g_ProcessMgr.GetProcess( iProcessId );
	}

	ProcessPtr Monitor(DWORD iProcessId, ProcessHandler* pHandler, interface_ref* pUserData)
	{
		return g_ProcessMgr.Monitor( iProcessId, pHandler, pUserData );
	}

	std::string GetCurrentProcessName( void )
	{
		TCHAR szDllName[_MAX_PATH] = { 0, };
		TCHAR szFilename[256] = { 0, };
		GetModuleFileName(0, szDllName, _MAX_PATH);
		_splitpath_s(szDllName, NULL, 0, NULL, 0, szFilename, sizeof(szFilename), NULL, 0);
		return szFilename;
	}

	std::string GetEnvVariable( const std::string& sKey )
	{
		enum { BUF_LEN = 2048 };
		char szBuf[BUF_LEN];

		DWORD n = ::GetEnvironmentVariable(sKey.c_str(), szBuf, BUF_LEN);
		if( n == 0 || n >= BUF_LEN )
		{
			return null_string;
		}

		return szBuf;
	}

	void SetEnvVariable( const std::string& sKey, const std::string& sVal )
	{
		::SetEnvironmentVariable(sKey.c_str(), sVal.c_str());
	}

	std::string GetEnvString( void )
	{
		std::string sEnv;
		LPVOID pEnv = GetEnvironmentStrings();
		for ( char* p = (char*)pEnv; *p; p++)
		{
			while( *p )
			{
				sEnv += *p;
				p++;
			}
			sEnv += '\0';
		}

		FreeEnvironmentStrings( (LPTSTR)pEnv );
		return sEnv;
	}

	void DumpEnvString( const std::string& sEnv )
	{
		for ( size_t i = 0; i < sEnv.size(); i++ )
		{
			if ( sEnv[i] )
				putchar( sEnv[i] );
			else
				putchar( '\n' );
		}
	}

	void DumpEnvString( char* pEnv )
	{
		for ( char* p = pEnv; *p; p++)
		{
			while( *p )
			{
				putchar( *p );
				p++;
			}
			putchar( '\n' );
		}
	}

};