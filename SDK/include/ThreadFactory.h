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
#include <ThreadFunc_Args.h>

/** 
	쓰레드의 생성, 실행, 종료시키는 인터페이스를 제공하는 추상 클래스

	쓰레드 객체는 ThreadFactory 에서 제공하는 Create() 함수나 Start() 탬플릿 함수를 통해 생성할 수 있다.
	아래는 쓰레드 객체를 생성하는 방법을 보여준다.

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
struct Thread
{
	/**
		쓰레드를 정지상태로 생성하며 쓰레드에서 실행할 ThreadRunnable 객체를 쓰레드에 전달한다.
		@param pRunCore ThreadRunnable를 상속받아 구현된 객체의 주소값로 쓰레드가 실행되면 pRunCore->Running() 함수가 호출된다.
		@param iBindCPU 쓰레드를 실행한 CPU 번호
	*/
	virtual bool				Start( ThreadRunnable *pRunCore, unsigned short iBindCPU ) = 0;
	/**
		정지상태의 쓰레드를 실행하며 전달받은 ThreadRunnable 객체의 Running() 함수를 호출한다.
	*/
	virtual bool				Resume( void ) = 0;
	/**
		쓰레드를 강제로 종료합니다.
	*/
	virtual bool				Terminate( void ) = 0;
	/**
		쓰레드가 종료하기를 기다린다.
	*/
	virtual bool				Wait( void ) = 0;
	/** 
		쓰레드의 우선순위 설정한다.
		@param iPrio 쓰레드 우선순위
		@li THREAD_PRIORITY_TIME_CRITICAL (최우선)
		@li THREAD_PRIORITY_HIGHEST (높음)
		@li THREAD_PRIORITY_ABOVE_NORMAL (조금높음)
		@li THREAD_PRIORITY_NORMAL (보통)
		@li THREAD_PRIORITY_BELOW_NORMAL (조금낮음)
		@li THREAD_PRIORITY_LOWEST (매우낮음)
		@li THREAD_PRIORITY_IDLE (시스템 유휴 시간에만 동작)
	*/
	virtual bool				SetPriority( int iPrio ) = 0;
	/**
		쓰레드가 실행되고 있는 CPU 번호를 조회한다.
	*/
	virtual unsigned short		GetBindCPU( void ) = 0;
	/**
		쓰레드가 실행될 CPU 번호를 설정한다.
		@param 쓰레드를 실행할 CPU 번호
	*/
	virtual void				SetBindCPU( unsigned short iCPU ) = 0;
	/**
		쓰레드 선호도(실행될 CPU 번호) 마스크를 조회한다.
	*/
	virtual DWORD				GetAffinity( void ) = 0;
	/**
		쓰레드 선호도 마스크를 설정한다.
		@param dwMask 선호도 마스크
	*/
	virtual bool				SetAffinity( DWORD dwMask ) = 0;
	/**
		쓰레드 핸들을 조회한다.
	*/
	virtual HANDLE				GetHandle( void ) = 0;
	/**
		쓰레드 ID를 조회한다.
	*/
	virtual DWORD				GetThreadId( void ) = 0;
};

/** 
	쓰레드 객체를 생성하고 쓰레드를 실행,종료하는 전역함수와 클래스의 맴버함수를 쓰레드에서 실행할 수 있도록 도와주는 템플릿 함수를
	제공하는 네임스페이스.

	아래는 Main클래스의 Func1, Func2 맴버함수를 쓰레드에서 실행하는 방법을 보여준다.
	@verbatim
	
	struct Main
	{
		void Func_1( void )
		{
		}
		void Func_2( int a )
		{
		}
	};

	int _tmain( int argc, char* argv[] )
	{
		Main obj;
		// 1 번 CPU 에서 쓰레드가 실행되게 설정한다.
		Thread* pThr1 = ThreadFactory::Start( &obj, &Main::Func_1, 1 );
		// 쓰레드 실행한다.
		pThr1->Resume();

		int arg1 = 23;
		// 파라미터로 arg1을 전달하고 2 번 CPU 에서 쓰레드가 실행되게 설정한다.
		Thread* pThr2 = ThreadFactory::Start( &obj, &Main::Func_2, a, 2 );
		// 쓰레드를 실행한다.
		pThr2->Resume();
		return 0;
	}

	@endverbatim
*/
namespace ThreadFactory
{
	/** 쓰레드 객체를 생성한다. */
	Thread* Create( void );
	/** 
		쓰레드 객체를 생성하고 쓰레드에서 호출할 클래스의 맴버함수 등록한다.
		@param o 쓰레드에서 호출할 클래스의 맴버 함수가 선언된 객체의 포인터
		@param f 클래스 맴버함수 포인터
		@param iBindCPU 쓰레드를 실행할 CPU 번호
	*/
	template<class O, class F>
	Thread* Start( const O& o, F f, unsigned short iBindCPU )
	{
		ThreadFunc_Arg0* pRunCore = new ThreadFunc_Arg0;
		pRunCore->m_fn.assign(o, f);
		Thread* pThr = Create();
		if ( pThr->Start(pRunCore, iBindCPU) )
		{
			return pThr;
		}
		delete pThr;
		delete pRunCore;

		return nullptr;
	}
	/** 
		쓰레드 객체를 생성하고 쓰레드에서 호출할 클래스의 맴버함수 등록한다.
		@param o 쓰레드에서 호출할 클래스의 맴버 함수가 선언된 객체의 포인터
		@param f 클래스 맴버함수 포인터
		@param arg1 맴버함수에 전달할 파리미터1
		@param iBindCPU 쓰레드를 실행할 CPU 번호
	*/
	template<class O, class F, class T1>
	Thread* Start( const O& o, F f, const T1& arg1, unsigned short iBindCPU )
	{
		ThreadFunc_Arg1<T1>* pRunCore = new ThreadFunc_Arg1<T1>(arg1);
		pRunCore->m_fn.assign(o, f);
		Thread* pThr = Create();
		if ( pThr->Start(pRunCore, iBindCPU) )
		{
			return pThr;
		}
		delete pThr;
		delete pRunCore;

		return pThr;
	}
	/** 
		쓰레드 객체를 생성하고 쓰레드에서 호출할 클래스의 맴버함수 등록한다.
		@param o 쓰레드에서 호출할 클래스의 맴버 함수가 선언된 객체의 포인터
		@param f 클래스 맴버함수 포인터
		@param arg1 맴버함수에 전달할 파리미터1
		@param arg2 맴버함수에 전달할 파리미터2
		@param iBindCPU 쓰레드를 실행할 CPU 번호
	*/
	template<class O, class F, class T1, class T2>
	Thread* Start( const O& o, F f, const T1& arg1, const T2& arg2, unsigned short iBindCPU )
	{
		ThreadFunc_Arg2<T1,T2>* pRunCore = new ThreadFunc_Arg2<T1,T2>(arg1, arg2);
		pRunCore->m_fn.assign(o, f);
		Thread* pThr = Create();
		if ( pThr->Start(pRunCore, iBindCPU) )
		{
			return pThr;
		}
		delete pThr;
		delete pRunCore;

		return pThr;
	}
	/** 
		쓰레드 객체를 생성하고 쓰레드에서 호출할 클래스의 맴버함수 등록한다.
		@param o 쓰레드에서 호출할 클래스의 맴버 함수가 선언된 객체의 포인터
		@param f 클래스 맴버함수 포인터
		@param arg1 맴버함수에 전달할 파리미터1
		@param arg2 맴버함수에 전달할 파리미터2
		@param arg3 맴버함수에 전달할 파리미터3
		@param iBindCPU 쓰레드를 실행할 CPU 번호
	*/
	template<class O, class F, class T1, class T2, class T3>
	Thread* Start( const O& o, F f, const T1& arg1, const T2& arg2, const T3& arg3, unsigned short iBindCPU )
	{
		ThreadFunc_Arg3<T1,T2>* pRunCore = new ThreadFunc_Arg3<T1,T2>(arg1, arg2, arg3);
		pRunCore->m_fn.assign(o, f);
		Thread* pThr = Create();
		if ( pThr->Start(pRunCore, iBindCPU) )
		{
			return pThr;
		}
		delete pThr;
		delete pRunCore;

		return pThr;
	}
	/** 
		쓰레드 객체를 생성하고 쓰레드에서 호출할 클래스의 맴버함수 등록한다.
		@param o 쓰레드에서 호출할 클래스의 맴버 함수가 선언된 객체의 포인터
		@param f 클래스 맴버함수 포인터
		@param arg1 쓰레드에서 호출되는 맴버함수에 전달할 파리미터1
		@param arg2 쓰레드에서 호출되는 맴버함수에 전달할 파리미터2
		@param arg3 쓰레드에서 호출되는 맴버함수에 전달할 파리미터3
		@param arg4 쓰레드에서 호출되는 맴버함수에 전달할 파리미터4
		@param iBindCPU 쓰레드를 실행할 CPU 번호
	*/
	template<class O, class F, class T1, class T2, class T3, class T4>
	Thread* Start( const O& o, F f, const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, unsigned short iBindCPU )
	{
		ThreadFunc_Arg4<T1,T2,T3,T4>* pRunCore = new ThreadFunc_Arg4<T1,T2,T3,T4>(arg1, arg2, arg3, arg4);
		pRunCore->m_fn.assign(o, f);
		Thread* pThr = Create();
		if ( pThr->Start(pRunCore, iBindCPU) )
		{
			return pThr;
		}
		delete pThr;
		delete pRunCore;

		return pThr;
	}
	/** 
		쓰레드 객체를 생성하고 쓰레드에서 호출할 클래스의 맴버함수 등록한다.
		@param o 쓰레드에서 호출할 클래스의 맴버 함수가 선언된 객체의 포인터
		@param f 클래스 맴버함수 포인터
		@param arg1 맴버함수에 전달할 파리미터1
		@param arg2 맴버함수에 전달할 파리미터2
		@param arg3 맴버함수에 전달할 파리미터3
		@param arg4 맴버함수에 전달할 파리미터4
		@param arg5 맴버함수에 전달할 파리미터5
		@param iBindCPU 쓰레드를 실행할 CPU 번호
	*/
	template<class O, class F, class T1, class T2, class T3, class T4, class T5>
	Thread* Start( const O& o, F f, const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, unsigned short iBindCPU )
	{
		// Task 등록
		ThreadFunc_Arg5<T1,T2,T3,T4,T5>* pRunCore = new ThreadFunc_Arg5<T1,T2,T3,T4,T5>(arg1, arg2, arg3, arg4, arg5);
		pRunCore->m_fn.assign(o, f);
		Thread* pThr = Create();
		if ( pThr->Start(pRunCore, iBindCPU) )
		{
			return pThr;
		}
		delete pThr;
		delete pRunCore;

		return pThr;
	}
	/** 
		쓰레드 객체를 생성하고 쓰레드에서 호출할 클래스의 맴버함수 등록한다.
		@param o 쓰레드에서 호출할 클래스의 맴버 함수가 선언된 객체의 포인터
		@param f 클래스 맴버함수 포인터
		@param arg1 맴버함수에 전달할 파리미터1
		@param arg2 맴버함수에 전달할 파리미터2
		@param arg3 맴버함수에 전달할 파리미터3
		@param arg4 맴버함수에 전달할 파리미터4
		@param arg5 맴버함수에 전달할 파리미터5
		@param arg6 맴버함수에 전달할 파리미터6
		@param iBindCPU 쓰레드를 실행할 CPU 번호
	*/
	template<class O, class F, class T1, class T2, class T3, class T4, class T5, class T6>
	Thread* Start( const O& o, F f, const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, unsigned short iBindCPU )
	{
		ThreadFunc_Arg6<T1,T2,T3,T4,T5,T6>* pRunCore = new ThreadFunc_Arg6<T1,T2,T3,T4,T5,T6>(arg1, arg2, arg3, arg4, arg5, arg6);
		pRunCore->m_fn.assign(o, f);
		Thread* pThr = Create();
		if ( pThr->Start(pRunCore, iBindCPU) )
		{
			return pThr;
		}
		delete pThr;
		delete pRunCore;

		return pThr;
	}
};

/**
	@def 쓰레드 생성 매크로
*/
#define CREATE_THREAD_V0(o, func, bindcpu) \
	ThreadFactory::Start( o, func, bindcpu )
/**
	@def 쓰레드 생성 매크로
*/
#define CREATE_THREAD_V1(o, func, arg1, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, bindcpu )
/**
	@def 쓰레드 생성 매크로
*/
#define CREATE_THREAD_V2(o, func, arg1, arg2, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, arg2, bindcpu )
/**
	@def 쓰레드 생성 매크로
*/
#define CREATE_THREAD_V3(o, func, arg1, arg2, arg3, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, arg2, arg3, bindcpu )
/**
	@def 쓰레드 생성 매크로
*/
#define CREATE_THREAD_V4(o, func, arg1, arg2, arg3, arg4, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, arg2, arg3, arg4, bindcpu )
/**
	@def 쓰레드 생성 매크로
*/
#define CREATE_THREAD_V5(o, func, arg1, arg2, arg3, arg4, arg5, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, arg2, arg3, arg4, arg5, bindcpu )
/**
	@def 쓰레드 생성 매크로
*/
#define CREATE_THREAD_V6(o, func, arg1, arg2, arg3, arg4, arg5, arg6, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, arg2, arg3, arg4, arg5, arg6, bindcpu )