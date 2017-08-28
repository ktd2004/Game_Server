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
	Thread ��ü�� ThreadRunnable�� ��ӹ��� ��ü�� Running() �Լ��� �����忡�� �����Ѵ�.
	
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
	�����忡�� Ŭ������ �ɹ��Լ��� ȣ���ϱ� ���� ���Ǵ� ���ø�Ŭ����.
	
	ThreadFunc_Arg���� Ŭ������ �ɹ��Լ��� ȣ���ϴ� �ݹ��Լ��ڿ� �ݹ��Լ��ڿ� ������ �Ű������� �����ϰ� �ִ�.
	�����尡 ����Ǹ� ThreadFunc_Arg�� Running() �Լ��� ȣ��ǰ� Running() �Լ������� �ݹ��Լ��ڸ� ȣ���Ͽ�
	Ŭ������ �ɹ��Լ��� �����忡�� ����ǵ��� �����Ǿ� �ִ�. �Ʒ��� Thread �� ������ �ڵ带 �Ϻθ� �����ش�.
	
	@verbatim
	// �����带 ���� ���·� �����ϸ� pRunCore �� ������� �����մϴ�.
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

	// Thread ��ü���� pRunCore�� Running() �Լ��� ȣ���մϴ�.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	��뿹)

	ThreadFunc_Arg0* pRunCore = new ThreadFunc_Arg0;
	// �����忡�� ȣ���� �ݹ��Լ� ����Ѵ�.
	pRunCore->m_fn.assign(o, f);			
	// ������ ��ü�� �����Ѵ�.
	Thread* pThr = ThreadFactory::Create();
	// ������ 1���� �������·� �����Ѵ�.
	pThr->Start( pRunCore, 1 );
	// ������ �����Ѵ�.
	pThr->Resume();
	@endverbatim
*/
class ThreadFunc_Arg0 : public ThreadRunnable
{
public:
	functional_v0<void>	m_fn;	///< �����忡�� ������ �ݹ��Լ�

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
	�����忡�� Ŭ������ �ɹ��Լ��� ȣ���ϱ� ���� ���Ǵ� ���ø�Ŭ����.
	
	ThreadFunc_Arg���� Ŭ������ �ɹ��Լ��� ȣ���ϴ� �ݹ��Լ��ڿ� �ݹ��Լ��ڿ� ������ �Ű������� �����ϰ� �ִ�.
	�����尡 ����Ǹ� ThreadFunc_Arg�� Running() �Լ��� ȣ��ǰ� Running() �Լ������� �ݹ��Լ��ڸ� ȣ���Ͽ�
	Ŭ������ �ɹ��Լ��� �����忡�� ����ǵ��� �����Ǿ� �ִ�. �Ʒ��� Thread �� ������ �ڵ带 �Ϻθ� �����ش�.
	
	@verbatim
	// �����带 ���� ���·� �����ϸ� pRunCore �� ������� �����մϴ�.
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

	// Thread ��ü���� pRunCore�� Running() �Լ��� ȣ���մϴ�.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	��뿹)
	int arg1 = 3;

	ThreadFunc_Arg1* pRunCore = new ThreadFunc_Arg1<int>(arg1);
	// �����忡�� ȣ���� �ݹ��Լ� ����Ѵ�.
	pRunCore->m_fn.assign(o, f);			
	// ������ ��ü�� �����Ѵ�.
	Thread* pThr = ThreadFactory::Create();
	// ������ 1���� �������·� �����Ѵ�.
	pThr->Start( pRunCore, 1 );
	// ������ �����Ѵ�.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1>
class ThreadFunc_Arg1 : public ThreadRunnable
{
public:
	functional_v1<void, Param1>	m_fn;	///< �����忡�� ������ �ݹ��Լ�

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
	Param1	m_Param1;	///< �ݹ��Լ��� ���޵� �Ķ���� 1
};

/**
	�����忡�� Ŭ������ �ɹ��Լ��� ȣ���ϱ� ���� ���Ǵ� ���ø�Ŭ����.
	
	ThreadFunc_Arg���� Ŭ������ �ɹ��Լ��� ȣ���ϴ� �ݹ��Լ��ڿ� �ݹ��Լ��ڿ� ������ �Ű������� �����ϰ� �ִ�.
	�����尡 ����Ǹ� ThreadFunc_Arg�� Running() �Լ��� ȣ��ǰ� Running() �Լ������� �ݹ��Լ��ڸ� ȣ���Ͽ�
	Ŭ������ �ɹ��Լ��� �����忡�� ����ǵ��� �����Ǿ� �ִ�. �Ʒ��� Thread �� ������ �ڵ带 �Ϻθ� �����ش�.
	
	@verbatim
	// �����带 ���� ���·� �����ϸ� pRunCore �� ������� �����մϴ�.
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

	// Thread ��ü���� pRunCore�� Running() �Լ��� ȣ���մϴ�.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	��뿹)
	int arg1 = 3, arg2 = 4;

	ThreadFunc_Arg2* pRunCore = new ThreadFunc_Arg2<int,int>(arg1,arg2);
	// �����忡�� ȣ���� �ݹ��Լ� ����Ѵ�.
	pRunCore->m_fn.assign(o, f);			
	// ������ ��ü�� �����Ѵ�.
	Thread* pThr = ThreadFactory::Create();
	// ������ 1���� �������·� �����Ѵ�.
	pThr->Start( pRunCore, 1 );
	// ������ �����Ѵ�.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1, typename Param2>
class ThreadFunc_Arg2 : public ThreadRunnable
{
public:
	functional_v2<void, Param1, Param2>	m_fn;	///< �����忡�� ������ �ݹ��Լ�

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
	Param1	m_Param1;	///< �ݹ��Լ��� ���޵� �Ķ���� 1
	Param2	m_Param2;	///< �ݹ��Լ��� ���޵� �Ķ���� 2
};

/**
	�����忡�� Ŭ������ �ɹ��Լ��� ȣ���ϱ� ���� ���Ǵ� ���ø�Ŭ����.
	
	ThreadFunc_Arg���� Ŭ������ �ɹ��Լ��� ȣ���ϴ� �ݹ��Լ��ڿ� �ݹ��Լ��ڿ� ������ �Ű������� �����ϰ� �ִ�.
	�����尡 ����Ǹ� ThreadFunc_Arg�� Running() �Լ��� ȣ��ǰ� Running() �Լ������� �ݹ��Լ��ڸ� ȣ���Ͽ�
	Ŭ������ �ɹ��Լ��� �����忡�� ����ǵ��� �����Ǿ� �ִ�. �Ʒ��� Thread �� ������ �ڵ带 �Ϻθ� �����ش�.
	
	@verbatim
	// �����带 ���� ���·� �����ϸ� pRunCore �� ������� �����մϴ�.
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

	// Thread ��ü���� pRunCore�� Running() �Լ��� ȣ���մϴ�.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	��뿹)
	int arg1 = 3, arg2 = 4, arg3 = 3;

	ThreadFunc_Arg3* pRunCore = new ThreadFunc_Arg3<int,int,int>(arg1,arg2,arg3);
	// �����忡�� ȣ���� �ݹ��Լ� ����Ѵ�.
	pRunCore->m_fn.assign(o, f);			
	// ������ ��ü�� �����Ѵ�.
	Thread* pThr = ThreadFactory::Create();
	// ������ 1���� �������·� �����Ѵ�.
	pThr->Start( pRunCore, 1 );
	// ������ �����Ѵ�.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1, typename Param2, typename Param3>
class ThreadFunc_Arg3 : public ThreadRunnable
{
public:
	functional_v3<void, Param1, Param2, Param3>	m_fn;	///< �����忡�� ������ �ݹ��Լ�

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
	Param1	m_Param1;	///< �ݹ��Լ��� ���޵� �Ķ���� 1
	Param2	m_Param2;	///< �ݹ��Լ��� ���޵� �Ķ���� 2
	Param3	m_Param3;	///< �ݹ��Լ��� ���޵� �Ķ���� 3
};

/**
	�����忡�� Ŭ������ �ɹ��Լ��� ȣ���ϱ� ���� ���Ǵ� ���ø�Ŭ����.
	
	ThreadFunc_Arg���� Ŭ������ �ɹ��Լ��� ȣ���ϴ� �ݹ��Լ��ڿ� �ݹ��Լ��ڿ� ������ �Ű������� �����ϰ� �ִ�.
	�����尡 ����Ǹ� ThreadFunc_Arg�� Running() �Լ��� ȣ��ǰ� Running() �Լ������� �ݹ��Լ��ڸ� ȣ���Ͽ�
	Ŭ������ �ɹ��Լ��� �����忡�� ����ǵ��� �����Ǿ� �ִ�. �Ʒ��� Thread �� ������ �ڵ带 �Ϻθ� �����ش�.
	
	@verbatim
	// �����带 ���� ���·� �����ϸ� pRunCore �� ������� �����մϴ�.
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

	// Thread ��ü���� pRunCore�� Running() �Լ��� ȣ���մϴ�.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	��뿹)
	int arg1 = 3, arg2 = 4, arg3 = 3, arg4 = 4;

	ThreadFunc_Arg4* pRunCore = new ThreadFunc_Arg4<int,int,int,int>(arg1,arg2,arg3,arg4);
	// �����忡�� ȣ���� �ݹ��Լ� ����Ѵ�.
	pRunCore->m_fn.assign(o, f);			
	// ������ ��ü�� �����Ѵ�.
	Thread* pThr = ThreadFactory::Create();
	// ������ 1���� �������·� �����Ѵ�.
	pThr->Start( pRunCore, 1 );
	// ������ �����Ѵ�.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1, typename Param2, typename Param3, typename Param4>
class ThreadFunc_Arg4 : public ThreadRunnable
{
public:
	functional_v4<void, Param1, Param2, Param3, Param4>	m_fn;	///< �����忡�� ������ �ݹ��Լ�

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
	Param1	m_Param1;	///< �ݹ��Լ��� ���޵� �Ķ���� 1
	Param2	m_Param2;	///< �ݹ��Լ��� ���޵� �Ķ���� 2
	Param3	m_Param3;	///< �ݹ��Լ��� ���޵� �Ķ���� 3
	Param4	m_Param4;	///< �ݹ��Լ��� ���޵� �Ķ���� 4
};

/**
	�����忡�� Ŭ������ �ɹ��Լ��� ȣ���ϱ� ���� ���Ǵ� ���ø�Ŭ����.
	
	ThreadFunc_Arg���� Ŭ������ �ɹ��Լ��� ȣ���ϴ� �ݹ��Լ��ڿ� �ݹ��Լ��ڿ� ������ �Ű������� �����ϰ� �ִ�.
	�����尡 ����Ǹ� ThreadFunc_Arg�� Running() �Լ��� ȣ��ǰ� Running() �Լ������� �ݹ��Լ��ڸ� ȣ���Ͽ�
	Ŭ������ �ɹ��Լ��� �����忡�� ����ǵ��� �����Ǿ� �ִ�. �Ʒ��� Thread �� ������ �ڵ带 �Ϻθ� �����ش�.
	
	@verbatim
	// �����带 ���� ���·� �����ϸ� pRunCore �� ������� �����մϴ�.
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

	// Thread ��ü���� pRunCore�� Running() �Լ��� ȣ���մϴ�.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	��뿹)
	int arg1 = 3, arg2 = 4, arg3 = 3, arg4 = 4, arg5 = 5;

	ThreadFunc_Arg5* pRunCore = new ThreadFunc_Arg5<int,int,int,int,int>(arg1,arg2,arg3,arg4,arg5);
	// �����忡�� ȣ���� �ݹ��Լ� ����Ѵ�.
	pRunCore->m_fn.assign(o, f);			
	// ������ ��ü�� �����Ѵ�.
	Thread* pThr = ThreadFactory::Create();
	// ������ 1���� �������·� �����Ѵ�.
	pThr->Start( pRunCore, 1 );
	// ������ �����Ѵ�.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
class ThreadFunc_Arg5 : public ThreadRunnable
{
public:
	functional_v5<void, Param1, Param2, Param3, Param4, Param5>	m_fn;	///< �����忡�� ������ �ݹ��Լ�

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
	Param1	m_Param1;	///< �ݹ��Լ��� ���޵� �Ķ���� 1
	Param2	m_Param2;	///< �ݹ��Լ��� ���޵� �Ķ���� 2
	Param3	m_Param3;	///< �ݹ��Լ��� ���޵� �Ķ���� 3
	Param4	m_Param4;	///< �ݹ��Լ��� ���޵� �Ķ���� 4
	Param5	m_Param5;	///< �ݹ��Լ��� ���޵� �Ķ���� 5
};

/**
	�����忡�� Ŭ������ �ɹ��Լ��� ȣ���ϱ� ���� ���Ǵ� ���ø�Ŭ����.
	
	ThreadFunc_Arg���� Ŭ������ �ɹ��Լ��� ȣ���ϴ� �ݹ��Լ��ڿ� �ݹ��Լ��ڿ� ������ �Ű������� �����ϰ� �ִ�.
	�����尡 ����Ǹ� ThreadFunc_Arg�� Running() �Լ��� ȣ��ǰ� Running() �Լ������� �ݹ��Լ��ڸ� ȣ���Ͽ�
	Ŭ������ �ɹ��Լ��� �����忡�� ����ǵ��� �����Ǿ� �ִ�. �Ʒ��� Thread �� ������ �ڵ带 �Ϻθ� �����ش�.
	
	@verbatim
	// �����带 ���� ���·� �����ϸ� pRunCore �� ������� �����մϴ�.
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

	// Thread ��ü���� pRunCore�� Running() �Լ��� ȣ���մϴ�.
	unsigned int __stdcall ThreadImpl::__ThreadEntryPoint(void *arg)
	{
		ThreadRunnable* pSelf = (ThreadRunnable*)arg;
		pSelf->Running();
		delete pSelf;
		return 0;
	}

	��뿹)
	int arg1 = 3, arg2 = 4, arg3 = 3, arg4 = 4, arg5 = 5, arg6 = 6;

	ThreadFunc_Arg6* pRunCore = new ThreadFunc_Arg6<int,int,int,int,int,int>(arg1,arg2,arg3,arg4,arg5,arg6);
	// �����忡�� ȣ���� �ݹ��Լ� ����Ѵ�.
	pRunCore->m_fn.assign(o, f);			
	// ������ ��ü�� �����Ѵ�.
	Thread* pThr = ThreadFactory::Create();
	// ������ 1���� �������·� �����Ѵ�.
	pThr->Start( pRunCore, 1 );
	// ������ �����Ѵ�.
	pThr->Resume();
	@endverbatim
*/
template<typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
class ThreadFunc_Arg6 : public ThreadRunnable
{
public:
	functional_v6<void, Param1, Param2, Param3, Param4, Param5, Param6>	m_fn;	///< �����忡�� ������ �ݹ��Լ�

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
	Param1	m_Param1;	///< �ݹ��Լ��� ���޵� �Ķ���� 1
	Param2	m_Param2;	///< �ݹ��Լ��� ���޵� �Ķ���� 2
	Param3	m_Param3;	///< �ݹ��Լ��� ���޵� �Ķ���� 3
	Param4	m_Param4;	///< �ݹ��Լ��� ���޵� �Ķ���� 4
	Param5	m_Param5;	///< �ݹ��Լ��� ���޵� �Ķ���� 5
	Param6	m_Param6;	///< �ݹ��Լ��� ���޵� �Ķ���� 6
};