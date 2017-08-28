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
//<
#include <Functional.h>

/**
	Thread 객체는 ThreadRunnable을 상속받은 객체의 Running() 함수를 쓰레드에서 실행한다.
	
	@verbatim
	class MyObj : public ThreadRunnable
	{
	public:

		void Running( void )
		{
			// do something
		}
	};

	int _tmain( int argc, char* argv[] )
	{
		MyObj obj;

		Thread* pThr = ThreadFactory::Create();
		pThr->Start( &obj, 1 );
		pThr->Resume();

		getchar();
		return 0;
	}
	@endverbatim
*/
struct ThreadRunnable
{
	virtual void Running( void ) = 0;
};

/**
	쓰레드에서 클래스의 맴버함수를 호출하기 위해 사용되는 탬플릿클래스.
	
	ThreadFunc_Arg에는 클래스의 맴버함수를 호출하는 콜백함수자와 콜백함수자에 전달할 매개변수를 저장하고 있다.
	쓰레드가 실행되면 ThreadFunc_Arg의 Running() 함수가 호출되고 Running() 함수에서는 콜백함수자를 호출하여
	클래스의 맴버함수가 쓰레드에서 실행되도록 구현되어 있다. 아래는 Thread 를 구현한 코드를 일부를 보여준다.
	
	@verbatim
	// 쓰레드를 정지 상태로 시작하면 pRunCore 를 쓰레드로 전달합니다.
	bool ThreadImpl::Start( ThreadRunnable *pRunCore, unsigned short iBindCPU )
	{
		m_hThread = BeginThreadEx(
		nullptr,					// Pointer to thread security attributes 
		0,						// Initial thread stack size, in bytes 
		__ThreadEntryPoint,     // Pointer to thread function 
		pRunCore,				//
		CREATE_SUSPENDED,       // Creation flags 
		&m_iThreadId);			// Pointer to returned thread identifier 

		...
	}

	// Thread 객체에서 pRunCore의 Running() 함수를 호출합니다.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	사용예)

	ThreadFunc_Arg0* pRunCore = new ThreadFunc_Arg0;
	// 쓰레드에서 호출할 콜백함수 등록한다.
	pRunCore->m_fn.assign(o, f);			
	// 쓰레드 객체를 생성한다.
	Thread* pThr = ThreadFactory::Create();
	// 쓰레드 1개를 정지상태로 실행한다.
	pThr->Start( pRunCore, 1 );
	// 쓰레드 실행한다.
	pThr->Resume();
	@endverbatim
*/
class ThreadFunc_Arg0 : public ThreadRunnable
{
public:
	functional_v0<void>	m_fn;	///< 쓰레드에서 실행할 콜백함수

public:
	ThreadFunc_Arg0()
	{
	}
	virtual ~ThreadFunc_Arg0()
	{}

	void Running() override
	{
		if ( m_fn ) m_fn();
	}
};

/**
	쓰레드에서 클래스의 맴버함수를 호출하기 위해 사용되는 탬플릿클래스.
	
	ThreadFunc_Arg에는 클래스의 맴버함수를 호출하는 콜백함수자와 콜백함수자에 전달할 매개변수를 저장하고 있다.
	쓰레드가 실행되면 ThreadFunc_Arg의 Running() 함수가 호출되고 Running() 함수에서는 콜백함수자를 호출하여
	클래스의 맴버함수가 쓰레드에서 실행되도록 구현되어 있다. 아래는 Thread 를 구현한 코드를 일부를 보여준다.
	
	@verbatim
	// 쓰레드를 정지 상태로 시작하면 pRunCore 를 쓰레드로 전달합니다.
	bool ThreadImpl::Start( ThreadRunnable *pRunCore, unsigned short iBindCPU )
	{
		m_hThread = BeginThreadEx(
		nullptr,					// Pointer to thread security attributes 
		0,						// Initial thread stack size, in bytes 
		__ThreadEntryPoint,     // Pointer to thread function 
		pRunCore,				//
		CREATE_SUSPENDED,       // Creation flags 
		&m_iThreadId);			// Pointer to returned thread identifier 

		...
	}

	// Thread 객체에서 pRunCore의 Running() 함수를 호출합니다.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	사용예)
	int arg1 = 3;

	ThreadFunc_Arg1* pRunCore = new ThreadFunc_Arg1<int>(arg1);
	// 쓰레드에서 호출할 콜백함수 등록한다.
	pRunCore->m_fn.assign(o, f);			
	// 쓰레드 객체를 생성한다.
	Thread* pThr = ThreadFactory::Create();
	// 쓰레드 1개를 정지상태로 실행한다.
	pThr->Start( pRunCore, 1 );
	// 쓰레드 실행한다.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1>
class ThreadFunc_Arg1 : public ThreadRunnable
{
public:
	functional_v1<void, Param1>	m_fn;	///< 쓰레드에서 실행할 콜백함수

public:
	ThreadFunc_Arg1(Param1 p1)
		:m_Param1(p1)
	{}
	virtual ~ThreadFunc_Arg1()
	{}

	virtual void Running() override
	{
		if ( m_fn ) m_fn( m_Param1 );
	}

protected:
	Param1	m_Param1;	///< 콜백함수에 전달될 파라미터 1
};

/**
	쓰레드에서 클래스의 맴버함수를 호출하기 위해 사용되는 탬플릿클래스.
	
	ThreadFunc_Arg에는 클래스의 맴버함수를 호출하는 콜백함수자와 콜백함수자에 전달할 매개변수를 저장하고 있다.
	쓰레드가 실행되면 ThreadFunc_Arg의 Running() 함수가 호출되고 Running() 함수에서는 콜백함수자를 호출하여
	클래스의 맴버함수가 쓰레드에서 실행되도록 구현되어 있다. 아래는 Thread 를 구현한 코드를 일부를 보여준다.
	
	@verbatim
	// 쓰레드를 정지 상태로 시작하면 pRunCore 를 쓰레드로 전달합니다.
	bool ThreadImpl::Start( ThreadRunnable *pRunCore, unsigned short iBindCPU )
	{
		m_hThread = BeginThreadEx(
		nullptr,					// Pointer to thread security attributes 
		0,						// Initial thread stack size, in bytes 
		__ThreadEntryPoint,     // Pointer to thread function 
		pRunCore,				//
		CREATE_SUSPENDED,       // Creation flags 
		&m_iThreadId);			// Pointer to returned thread identifier 

		...
	}

	// Thread 객체에서 pRunCore의 Running() 함수를 호출합니다.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	사용예)
	int arg1 = 3, arg2 = 4;

	ThreadFunc_Arg2* pRunCore = new ThreadFunc_Arg2<int,int>(arg1,arg2);
	// 쓰레드에서 호출할 콜백함수 등록한다.
	pRunCore->m_fn.assign(o, f);			
	// 쓰레드 객체를 생성한다.
	Thread* pThr = ThreadFactory::Create();
	// 쓰레드 1개를 정지상태로 실행한다.
	pThr->Start( pRunCore, 1 );
	// 쓰레드 실행한다.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1, typename Param2>
class ThreadFunc_Arg2 : public ThreadRunnable
{
public:
	functional_v2<void, Param1, Param2>	m_fn;	///< 쓰레드에서 실행할 콜백함수

public:
	ThreadFunc_Arg2(Param1 p1, Param2 p2)
		: m_Param1(p1)
		, m_Param2(p2)
	{}
	virtual ~ThreadFunc_Arg2()
	{}

	void Running() override
	{
		if ( m_fn ) m_fn( m_Param1, m_Param2 );
	}

protected:
	Param1	m_Param1;	///< 콜백함수에 전달될 파라미터 1
	Param2	m_Param2;	///< 콜백함수에 전달될 파라미터 2
};

/**
	쓰레드에서 클래스의 맴버함수를 호출하기 위해 사용되는 탬플릿클래스.
	
	ThreadFunc_Arg에는 클래스의 맴버함수를 호출하는 콜백함수자와 콜백함수자에 전달할 매개변수를 저장하고 있다.
	쓰레드가 실행되면 ThreadFunc_Arg의 Running() 함수가 호출되고 Running() 함수에서는 콜백함수자를 호출하여
	클래스의 맴버함수가 쓰레드에서 실행되도록 구현되어 있다. 아래는 Thread 를 구현한 코드를 일부를 보여준다.
	
	@verbatim
	// 쓰레드를 정지 상태로 시작하면 pRunCore 를 쓰레드로 전달합니다.
	bool ThreadImpl::Start( ThreadRunnable *pRunCore, unsigned short iBindCPU )
	{
		m_hThread = BeginThreadEx(
		nullptr,					// Pointer to thread security attributes 
		0,						// Initial thread stack size, in bytes 
		__ThreadEntryPoint,     // Pointer to thread function 
		pRunCore,				//
		CREATE_SUSPENDED,       // Creation flags 
		&m_iThreadId);			// Pointer to returned thread identifier 

		...
	}

	// Thread 객체에서 pRunCore의 Running() 함수를 호출합니다.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	사용예)
	int arg1 = 3, arg2 = 4, arg3 = 3;

	ThreadFunc_Arg3* pRunCore = new ThreadFunc_Arg3<int,int,int>(arg1,arg2,arg3);
	// 쓰레드에서 호출할 콜백함수 등록한다.
	pRunCore->m_fn.assign(o, f);			
	// 쓰레드 객체를 생성한다.
	Thread* pThr = ThreadFactory::Create();
	// 쓰레드 1개를 정지상태로 실행한다.
	pThr->Start( pRunCore, 1 );
	// 쓰레드 실행한다.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1, typename Param2, typename Param3>
class ThreadFunc_Arg3 : public ThreadRunnable
{
public:
	functional_v3<void, Param1, Param2, Param3>	m_fn;	///< 쓰레드에서 실행할 콜백함수

public:
	ThreadFunc_Arg3(Param1 p1, Param2 p2, Param3 p3)
		: m_Param1(p1)
		, m_Param2(p2)
		, m_Param3(p3)
	{}
	virtual ~ThreadFunc_Arg3()
	{}

	void Running() override
	{
		if ( m_fn ) m_fn( m_Param1, m_Param2, m_Param3 );
	}

protected:
	Param1	m_Param1;	///< 콜백함수에 전달될 파라미터 1
	Param2	m_Param2;	///< 콜백함수에 전달될 파라미터 2
	Param3	m_Param3;	///< 콜백함수에 전달될 파라미터 3
};

/**
	쓰레드에서 클래스의 맴버함수를 호출하기 위해 사용되는 탬플릿클래스.
	
	ThreadFunc_Arg에는 클래스의 맴버함수를 호출하는 콜백함수자와 콜백함수자에 전달할 매개변수를 저장하고 있다.
	쓰레드가 실행되면 ThreadFunc_Arg의 Running() 함수가 호출되고 Running() 함수에서는 콜백함수자를 호출하여
	클래스의 맴버함수가 쓰레드에서 실행되도록 구현되어 있다. 아래는 Thread 를 구현한 코드를 일부를 보여준다.
	
	@verbatim
	// 쓰레드를 정지 상태로 시작하면 pRunCore 를 쓰레드로 전달합니다.
	bool ThreadImpl::Start( ThreadRunnable *pRunCore, unsigned short iBindCPU )
	{
		m_hThread = BeginThreadEx(
		nullptr,					// Pointer to thread security attributes 
		0,						// Initial thread stack size, in bytes 
		__ThreadEntryPoint,     // Pointer to thread function 
		pRunCore,				//
		CREATE_SUSPENDED,       // Creation flags 
		&m_iThreadId);			// Pointer to returned thread identifier 

		...
	}

	// Thread 객체에서 pRunCore의 Running() 함수를 호출합니다.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	사용예)
	int arg1 = 3, arg2 = 4, arg3 = 3, arg4 = 4;

	ThreadFunc_Arg4* pRunCore = new ThreadFunc_Arg4<int,int,int,int>(arg1,arg2,arg3,arg4);
	// 쓰레드에서 호출할 콜백함수 등록한다.
	pRunCore->m_fn.assign(o, f);			
	// 쓰레드 객체를 생성한다.
	Thread* pThr = ThreadFactory::Create();
	// 쓰레드 1개를 정지상태로 실행한다.
	pThr->Start( pRunCore, 1 );
	// 쓰레드 실행한다.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1, typename Param2, typename Param3, typename Param4>
class ThreadFunc_Arg4 : public ThreadRunnable
{
public:
	functional_v4<void, Param1, Param2, Param3, Param4>	m_fn;	///< 쓰레드에서 실행할 콜백함수

public:
	ThreadFunc_Arg4(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
		: m_Param1(p1)
		, m_Param2(p2)
		, m_Param3(p3)
		, m_Param4(p4)
	{}
	virtual ~ThreadFunc_Arg4()
	{}

	void Running() override
	{
		if ( m_fn ) m_fn( m_Param1, m_Param2, m_Param3, m_Param4 );
	}

protected:
	Param1	m_Param1;	///< 콜백함수에 전달될 파라미터 1
	Param2	m_Param2;	///< 콜백함수에 전달될 파라미터 2
	Param3	m_Param3;	///< 콜백함수에 전달될 파라미터 3
	Param4	m_Param4;	///< 콜백함수에 전달될 파라미터 4
};

/**
	쓰레드에서 클래스의 맴버함수를 호출하기 위해 사용되는 탬플릿클래스.
	
	ThreadFunc_Arg에는 클래스의 맴버함수를 호출하는 콜백함수자와 콜백함수자에 전달할 매개변수를 저장하고 있다.
	쓰레드가 실행되면 ThreadFunc_Arg의 Running() 함수가 호출되고 Running() 함수에서는 콜백함수자를 호출하여
	클래스의 맴버함수가 쓰레드에서 실행되도록 구현되어 있다. 아래는 Thread 를 구현한 코드를 일부를 보여준다.
	
	@verbatim
	// 쓰레드를 정지 상태로 시작하면 pRunCore 를 쓰레드로 전달합니다.
	bool ThreadImpl::Start( ThreadRunnable *pRunCore, unsigned short iBindCPU )
	{
		m_hThread = BeginThreadEx(
		nullptr,					// Pointer to thread security attributes 
		0,						// Initial thread stack size, in bytes 
		__ThreadEntryPoint,     // Pointer to thread function 
		pRunCore,				//
		CREATE_SUSPENDED,       // Creation flags 
		&m_iThreadId);			// Pointer to returned thread identifier 

		...
	}

	// Thread 객체에서 pRunCore의 Running() 함수를 호출합니다.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	사용예)
	int arg1 = 3, arg2 = 4, arg3 = 3, arg4 = 4, arg5 = 5;

	ThreadFunc_Arg5* pRunCore = new ThreadFunc_Arg5<int,int,int,int,int>(arg1,arg2,arg3,arg4,arg5);
	// 쓰레드에서 호출할 콜백함수 등록한다.
	pRunCore->m_fn.assign(o, f);			
	// 쓰레드 객체를 생성한다.
	Thread* pThr = ThreadFactory::Create();
	// 쓰레드 1개를 정지상태로 실행한다.
	pThr->Start( pRunCore, 1 );
	// 쓰레드 실행한다.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
class ThreadFunc_Arg5 : public ThreadRunnable
{
public:
	functional_v5<void, Param1, Param2, Param3, Param4, Param5>	m_fn;	///< 쓰레드에서 실행할 콜백함수

public:
	ThreadFunc_Arg5(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
		: m_Param1(p1)
		, m_Param2(p2)
		, m_Param3(p3)
		, m_Param4(p4)
		, m_Param5(p5)
	{}
	virtual ~ThreadFunc_Arg5()
	{}

	void Running() override
	{
		if ( m_fn ) m_fn( m_Param1, m_Param2, m_Param3, m_Param4, m_Param5 );
	}

protected:
	Param1	m_Param1;	///< 콜백함수에 전달될 파라미터 1
	Param2	m_Param2;	///< 콜백함수에 전달될 파라미터 2
	Param3	m_Param3;	///< 콜백함수에 전달될 파라미터 3
	Param4	m_Param4;	///< 콜백함수에 전달될 파라미터 4
	Param5	m_Param5;	///< 콜백함수에 전달될 파라미터 5
};

/**
	쓰레드에서 클래스의 맴버함수를 호출하기 위해 사용되는 탬플릿클래스.
	
	ThreadFunc_Arg에는 클래스의 맴버함수를 호출하는 콜백함수자와 콜백함수자에 전달할 매개변수를 저장하고 있다.
	쓰레드가 실행되면 ThreadFunc_Arg의 Running() 함수가 호출되고 Running() 함수에서는 콜백함수자를 호출하여
	클래스의 맴버함수가 쓰레드에서 실행되도록 구현되어 있다. 아래는 Thread 를 구현한 코드를 일부를 보여준다.
	
	@verbatim
	// 쓰레드를 정지 상태로 시작하면 pRunCore 를 쓰레드로 전달합니다.
	bool ThreadImpl::Start( ThreadRunnable *pRunCore, unsigned short iBindCPU )
	{
		m_hThread = BeginThreadEx(
		nullptr,					// Pointer to thread security attributes 
		0,						// Initial thread stack size, in bytes 
		__ThreadEntryPoint,     // Pointer to thread function 
		pRunCore,				//
		CREATE_SUSPENDED,       // Creation flags 
		&m_iThreadId);			// Pointer to returned thread identifier 

		...
	}

	// Thread 객체에서 pRunCore의 Running() 함수를 호출합니다.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	사용예)
	int arg1 = 3, arg2 = 4, arg3 = 3, arg4 = 4, arg5 = 5, arg6 = 6;

	ThreadFunc_Arg6* pRunCore = new ThreadFunc_Arg6<int,int,int,int,int,int>(arg1,arg2,arg3,arg4,arg5,arg6);
	// 쓰레드에서 호출할 콜백함수 등록한다.
	pRunCore->m_fn.assign(o, f);			
	// 쓰레드 객체를 생성한다.
	Thread* pThr = ThreadFactory::Create();
	// 쓰레드 1개를 정지상태로 실행한다.
	pThr->Start( pRunCore, 1 );
	// 쓰레드 실행한다.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
class ThreadFunc_Arg6 : public ThreadRunnable
{
public:
	functional_v6<void, Param1, Param2, Param3, Param4, Param5, Param6>	m_fn;	///< 쓰레드에서 실행할 콜백함수

public:
	ThreadFunc_Arg6(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6)
		: m_Param1(p1)
		, m_Param2(p2)
		, m_Param3(p3)
		, m_Param4(p4)
		, m_Param5(p5)
		, m_Param6(p6)
	{}
	virtual ~ThreadFunc_Arg6()
	{}

	void Running() override
	{
		if ( m_fn ) m_fn( m_Param1, m_Param2, m_Param3, m_Param4, m_Param5, m_Param6 );
	}

protected:
	Param1	m_Param1;	///< 콜백함수에 전달될 파라미터 1
	Param2	m_Param2;	///< 콜백함수에 전달될 파라미터 2
	Param3	m_Param3;	///< 콜백함수에 전달될 파라미터 3
	Param4	m_Param4;	///< 콜백함수에 전달될 파라미터 4
	Param5	m_Param5;	///< 콜백함수에 전달될 파라미터 5
	Param6	m_Param6;	///< 콜백함수에 전달될 파라미터 6
};