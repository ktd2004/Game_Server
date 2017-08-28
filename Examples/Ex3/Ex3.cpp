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
#include <Compile.h>
#include <process.h>
#include <MemoryEx.h>
#include <FastList.h>
#include <Generic.h>
#include <tcmalloc.h>
#include <TaskScheduler.h>

static unsigned long __stdcall __ThreadEntryPoint( void *arg )
{
	int i = 0;
	TaskScheduler* pSch = (TaskScheduler*)arg;
	while ( true )
	{
		Sleep(1000);
		i++;
		if (i > 5)
			break;
	}

	pSch->Finalize();

	return 0;
}

#include <functional>
//using namespace placeholders;

void LamdaTest(std::function<void(int)>* func)
{
	if (func)
	{
		(*func)(10);
	}
}

class TestClass
{
public:
	TestClass() {}
	int Prn(int n) { printf("%d\n", n); return 1; }
};

int Prn(int n) { printf("kkkk %d\n", n); return 1; }

#include <functional>

int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc == 1 )
	{
		printf( "usage)\n" );
		printf( "ex3 count\n" );
		printf( "ex3 10\n" );
		return 0;
	}


	auto lambaFunc = [](int n)
	{
		std::cout << "Number : " << n << std::endl;
	};

//	lambaFunc(100);

	std::function<void(int)>* func = nullptr;
	func = new std::function<void(int)>();
	*func = lambaFunc;

	LamdaTest(func);

	TestClass cls;
	std::function<int(int)> func3;
	func3 = std::bind(&Prn, std::placeholders::_1);
	func3(20);
	


	int iThreadCnt = atoi(argv[1]);

	TaskScheduler* pSch = CREATE_TASKSCEDULER(1, 100, 0);

	// 쓰레드를 정지상태로 생성한다.
	std::list<HANDLE>	handles;
	for ( int i = 0; i < iThreadCnt; i++ )
	{
		DWORD ThreadId;
		HANDLE handle = CreateThread(
		nullptr,				// Pointer to thread security attributes 
		0,						// Initial thread stack size, in bytes 
		__ThreadEntryPoint,     // Pointer to thread function 
		pSch,					// Argument
		CREATE_SUSPENDED,       // Creation flags 
		&ThreadId);				// Pointer to returned thread identifier 

		handles.push_back(handle);
	}

	// 쓰레드를 실행한다.
	while ( !handles.empty() )
	{
		ResumeThread( handles.front() );
		handles.pop_front();
		Sleep( 10 );
	}

	pSch->RunEventLoop();

	return 0;
}