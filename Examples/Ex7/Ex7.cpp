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

#include <TaskScheduler.h>
#include <ProcessEx.h>

Buffer& operator<<( Buffer& pk, const ProcessOption& r )
{
	pk << r.m_sExecPath
		<< r.m_sCommandLine
		<< r.m_sDirectory
		<< r.m_sEnvironment
		<< r.m_iCreationFlag
		<< r.m_iInheritHandle
		<< r.m_bBackground
		<< r.m_iMaxRestartCnt;

	return pk;
}

Buffer& operator>>( Buffer& pk, ProcessOption& r )
{
	pk >> r.m_sExecPath
		>> r.m_sCommandLine
		>> r.m_sDirectory
		>> r.m_sEnvironment
		>> r.m_iCreationFlag
		>> r.m_iInheritHandle
		>> r.m_bBackground
		>> r.m_iMaxRestartCnt;
	return pk;
}

class ProcessMonitor : public ProcessHandler
{
public:
	ProcessMonitor()
	{
	}

	~ProcessMonitor()
	{
	}

	void OnLaunchProcess( ProcessPtr pST ) override
	{

	}

	void OnExitProcess( ProcessPtr pST ) override
	{
		printf( "exit process = %d\n", pST->GetProcessId() );
	}

};

int _tmain(int argc, _TCHAR* argv[])
{
// 	TaskScheduler* sc = TaskSchedulerFactory::Create( 1, 10, 1 );
// 	sc->SetDuration( 50 );
// 	TaskSchedulerFactory::Start();
// 
// 
// 	ProcessMonitor mon;
// 	ProcessFactory::Start( 1, 10 );
// 
// 	ProcessOption	opt;
// 	opt.Exec( "C:\\WINDOWS\\system32\\notepad.exe" );
// 	opt.CommandLine( "" );
// 	opt.WorkingDirectory( ".\\" );
// 	opt.CreationFlag( CREATE_NEW_CONSOLE );
// 	opt.Environment( "this=is" );
// 	opt.MaxRestartCnt( 3 );
// 
// 	ProcessFactory::Spawn( opt, &mon );

	Buffer buf;
	buf << (int)20;
	buf << std::string("this is test");

	int val = 0;
	std::string val2 = "";

	Buffer* buf2 = new Buffer(2048);
	*buf2 << buf;

	*buf2 >> val >> val2;
	
	printf("val = %d, val2 = %s\n", val, cstr(val2));
	buf >> val >> val2;
	printf("val = %d, val2 = %s\n", val, cstr(val2));
	

	getchar();

//	TaskSchedulerFactory::Stop();

	return 0;
}

