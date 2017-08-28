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
	�������� ����, ����, �����Ű�� �������̽��� �����ϴ� �߻� Ŭ����

	������ ��ü�� ThreadFactory ���� �����ϴ� Create() �Լ��� Start() ���ø� �Լ��� ���� ������ �� �ִ�.
	�Ʒ��� ������ ��ü�� �����ϴ� ����� �����ش�.

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
		�����带 �������·� �����ϸ� �����忡�� ������ ThreadRunnable ��ü�� �����忡 �����Ѵ�.
		@param pRunCore ThreadRunnable�� ��ӹ޾� ������ ��ü�� �ּҰ��� �����尡 ����Ǹ� pRunCore->Running() �Լ��� ȣ��ȴ�.
		@param iBindCPU �����带 ������ CPU ��ȣ
	*/
	virtual bool				Start( ThreadRunnable *pRunCore, unsigned short iBindCPU ) = 0;
	/**
		���������� �����带 �����ϸ� ���޹��� ThreadRunnable ��ü�� Running() �Լ��� ȣ���Ѵ�.
	*/
	virtual bool				Resume( void ) = 0;
	/**
		�����带 ������ �����մϴ�.
	*/
	virtual bool				Terminate( void ) = 0;
	/**
		�����尡 �����ϱ⸦ ��ٸ���.
	*/
	virtual bool				Wait( void ) = 0;
	/** 
		�������� �켱���� �����Ѵ�.
		@param iPrio ������ �켱����
		@li THREAD_PRIORITY_TIME_CRITICAL (�ֿ켱)
		@li THREAD_PRIORITY_HIGHEST (����)
		@li THREAD_PRIORITY_ABOVE_NORMAL (���ݳ���)
		@li THREAD_PRIORITY_NORMAL (����)
		@li THREAD_PRIORITY_BELOW_NORMAL (���ݳ���)
		@li THREAD_PRIORITY_LOWEST (�ſ쳷��)
		@li THREAD_PRIORITY_IDLE (�ý��� ���� �ð����� ����)
	*/
	virtual bool				SetPriority( int iPrio ) = 0;
	/**
		�����尡 ����ǰ� �ִ� CPU ��ȣ�� ��ȸ�Ѵ�.
	*/
	virtual unsigned short		GetBindCPU( void ) = 0;
	/**
		�����尡 ����� CPU ��ȣ�� �����Ѵ�.
		@param �����带 ������ CPU ��ȣ
	*/
	virtual void				SetBindCPU( unsigned short iCPU ) = 0;
	/**
		������ ��ȣ��(����� CPU ��ȣ) ����ũ�� ��ȸ�Ѵ�.
	*/
	virtual DWORD				GetAffinity( void ) = 0;
	/**
		������ ��ȣ�� ����ũ�� �����Ѵ�.
		@param dwMask ��ȣ�� ����ũ
	*/
	virtual bool				SetAffinity( DWORD dwMask ) = 0;
	/**
		������ �ڵ��� ��ȸ�Ѵ�.
	*/
	virtual HANDLE				GetHandle( void ) = 0;
	/**
		������ ID�� ��ȸ�Ѵ�.
	*/
	virtual DWORD				GetThreadId( void ) = 0;
};

/** 
	������ ��ü�� �����ϰ� �����带 ����,�����ϴ� �����Լ��� Ŭ������ �ɹ��Լ��� �����忡�� ������ �� �ֵ��� �����ִ� ���ø� �Լ���
	�����ϴ� ���ӽ����̽�.

	�Ʒ��� MainŬ������ Func1, Func2 �ɹ��Լ��� �����忡�� �����ϴ� ����� �����ش�.
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
		// 1 �� CPU ���� �����尡 ����ǰ� �����Ѵ�.
		Thread* pThr1 = ThreadFactory::Start( &obj, &Main::Func_1, 1 );
		// ������ �����Ѵ�.
		pThr1->Resume();

		int arg1 = 23;
		// �Ķ���ͷ� arg1�� �����ϰ� 2 �� CPU ���� �����尡 ����ǰ� �����Ѵ�.
		Thread* pThr2 = ThreadFactory::Start( &obj, &Main::Func_2, a, 2 );
		// �����带 �����Ѵ�.
		pThr2->Resume();
		return 0;
	}

	@endverbatim
*/
namespace ThreadFactory
{
	/** ������ ��ü�� �����Ѵ�. */
	Thread* Create( void );
	/** 
		������ ��ü�� �����ϰ� �����忡�� ȣ���� Ŭ������ �ɹ��Լ� ����Ѵ�.
		@param o �����忡�� ȣ���� Ŭ������ �ɹ� �Լ��� ����� ��ü�� ������
		@param f Ŭ���� �ɹ��Լ� ������
		@param iBindCPU �����带 ������ CPU ��ȣ
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
		������ ��ü�� �����ϰ� �����忡�� ȣ���� Ŭ������ �ɹ��Լ� ����Ѵ�.
		@param o �����忡�� ȣ���� Ŭ������ �ɹ� �Լ��� ����� ��ü�� ������
		@param f Ŭ���� �ɹ��Լ� ������
		@param arg1 �ɹ��Լ��� ������ �ĸ�����1
		@param iBindCPU �����带 ������ CPU ��ȣ
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
		������ ��ü�� �����ϰ� �����忡�� ȣ���� Ŭ������ �ɹ��Լ� ����Ѵ�.
		@param o �����忡�� ȣ���� Ŭ������ �ɹ� �Լ��� ����� ��ü�� ������
		@param f Ŭ���� �ɹ��Լ� ������
		@param arg1 �ɹ��Լ��� ������ �ĸ�����1
		@param arg2 �ɹ��Լ��� ������ �ĸ�����2
		@param iBindCPU �����带 ������ CPU ��ȣ
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
		������ ��ü�� �����ϰ� �����忡�� ȣ���� Ŭ������ �ɹ��Լ� ����Ѵ�.
		@param o �����忡�� ȣ���� Ŭ������ �ɹ� �Լ��� ����� ��ü�� ������
		@param f Ŭ���� �ɹ��Լ� ������
		@param arg1 �ɹ��Լ��� ������ �ĸ�����1
		@param arg2 �ɹ��Լ��� ������ �ĸ�����2
		@param arg3 �ɹ��Լ��� ������ �ĸ�����3
		@param iBindCPU �����带 ������ CPU ��ȣ
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
		������ ��ü�� �����ϰ� �����忡�� ȣ���� Ŭ������ �ɹ��Լ� ����Ѵ�.
		@param o �����忡�� ȣ���� Ŭ������ �ɹ� �Լ��� ����� ��ü�� ������
		@param f Ŭ���� �ɹ��Լ� ������
		@param arg1 �����忡�� ȣ��Ǵ� �ɹ��Լ��� ������ �ĸ�����1
		@param arg2 �����忡�� ȣ��Ǵ� �ɹ��Լ��� ������ �ĸ�����2
		@param arg3 �����忡�� ȣ��Ǵ� �ɹ��Լ��� ������ �ĸ�����3
		@param arg4 �����忡�� ȣ��Ǵ� �ɹ��Լ��� ������ �ĸ�����4
		@param iBindCPU �����带 ������ CPU ��ȣ
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
		������ ��ü�� �����ϰ� �����忡�� ȣ���� Ŭ������ �ɹ��Լ� ����Ѵ�.
		@param o �����忡�� ȣ���� Ŭ������ �ɹ� �Լ��� ����� ��ü�� ������
		@param f Ŭ���� �ɹ��Լ� ������
		@param arg1 �ɹ��Լ��� ������ �ĸ�����1
		@param arg2 �ɹ��Լ��� ������ �ĸ�����2
		@param arg3 �ɹ��Լ��� ������ �ĸ�����3
		@param arg4 �ɹ��Լ��� ������ �ĸ�����4
		@param arg5 �ɹ��Լ��� ������ �ĸ�����5
		@param iBindCPU �����带 ������ CPU ��ȣ
	*/
	template<class O, class F, class T1, class T2, class T3, class T4, class T5>
	Thread* Start( const O& o, F f, const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, unsigned short iBindCPU )
	{
		// Task ���
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
		������ ��ü�� �����ϰ� �����忡�� ȣ���� Ŭ������ �ɹ��Լ� ����Ѵ�.
		@param o �����忡�� ȣ���� Ŭ������ �ɹ� �Լ��� ����� ��ü�� ������
		@param f Ŭ���� �ɹ��Լ� ������
		@param arg1 �ɹ��Լ��� ������ �ĸ�����1
		@param arg2 �ɹ��Լ��� ������ �ĸ�����2
		@param arg3 �ɹ��Լ��� ������ �ĸ�����3
		@param arg4 �ɹ��Լ��� ������ �ĸ�����4
		@param arg5 �ɹ��Լ��� ������ �ĸ�����5
		@param arg6 �ɹ��Լ��� ������ �ĸ�����6
		@param iBindCPU �����带 ������ CPU ��ȣ
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
	@def ������ ���� ��ũ��
*/
#define CREATE_THREAD_V0(o, func, bindcpu) \
	ThreadFactory::Start( o, func, bindcpu )
/**
	@def ������ ���� ��ũ��
*/
#define CREATE_THREAD_V1(o, func, arg1, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, bindcpu )
/**
	@def ������ ���� ��ũ��
*/
#define CREATE_THREAD_V2(o, func, arg1, arg2, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, arg2, bindcpu )
/**
	@def ������ ���� ��ũ��
*/
#define CREATE_THREAD_V3(o, func, arg1, arg2, arg3, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, arg2, arg3, bindcpu )
/**
	@def ������ ���� ��ũ��
*/
#define CREATE_THREAD_V4(o, func, arg1, arg2, arg3, arg4, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, arg2, arg3, arg4, bindcpu )
/**
	@def ������ ���� ��ũ��
*/
#define CREATE_THREAD_V5(o, func, arg1, arg2, arg3, arg4, arg5, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, arg2, arg3, arg4, arg5, bindcpu )
/**
	@def ������ ���� ��ũ��
*/
#define CREATE_THREAD_V6(o, func, arg1, arg2, arg3, arg4, arg5, arg6, bindcpu ) \
	ThreadFactory::Start( o, func, arg1, arg2, arg3, arg4, arg5, arg6, bindcpu )