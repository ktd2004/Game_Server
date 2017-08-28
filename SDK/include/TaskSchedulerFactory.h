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

#include <Functional.h>
#include <Procedure.h>
#include <TaskEventMsg.h>

/** 
	�½�ũ�����ٷ��� �۾��̺�Ʈ�� �����Ҷ� ȣ��Ǵ� �ݹ� �Լ����� Ÿ�Լ���.
*/
typedef functional_v0<void>		TaskFn_V0;

/** 
	�½�ũ ���� ���� Ŭ����.

	�½�ũ�����ٷ��� ���� ȣ��Ǵ� Task���� ȣ�� ����, ��� ����ð����� ������ �����ϴ� Ŭ�����̴�.
*/
class TaskBase
{
public:
	TaskFn_V0			m_Fn;			///< �½�ũ�ݹ��Լ�
	unsigned long		m_iStart;		///< �½�ũ�ݹ��Լ�ȣ����۽ð�
	unsigned long		m_iDuration;	///< �½�ũ�ݹ��Լ�ȣ�Ⱓ��
	double				m_dAvg;			///< ��ռ���ð�

private:
	TaskBase( const TaskBase& r ) {}

public:
	TaskBase(unsigned long iDuration)
		: m_iDuration(iDuration)
		, m_dAvg(0)
		, m_iStart(0)
	{}
	virtual ~TaskBase()
	{}
};


/** 
	�½�ũ�����ٷ��� Ÿ�̸Ӹ� �����Ҷ� ȣ��Ǵ� �ݹ� �Լ����� Ÿ�Լ���.
*/
typedef functional_v1<bool, void*>		TimerFn_V1;

PTR(TimerBase);
/** 
	Ÿ�̸� ���� ���� Ŭ����.

	�½�ũ�����ٷ��� ���� ȣ��Ǵ� Ÿ�̸ӵ��� ȣ�� ����, ����ð����� ������ �����ϴ� Ŭ�����̴�.
*/
class TimerBase : public shared_ref
{
private:
	TimerBase(const TimerBase& r ) {}

public:
	TimerFn_V1			m_Fn;			///< Ÿ�̸��ݹ��Լ�
	void*				m_pUserData;	///< ����ڵ���Ÿ
	unsigned __int64	m_iTm;			///< ȣ��ð�
	unsigned long		m_iDuration;	///< Ÿ�̸��ݹ�ȣ�Ⱓ��
	bool				m_bCancel;		///< Ÿ�̸� ���
	TimerBase()
		: m_iTm(0)
		, m_iDuration(0)
		, m_pUserData(nullptr)
		, m_bCancel(false)
	{}
	virtual ~TimerBase()
	{}
};

/** 
	Ÿ�̸�Ű ����
*/
typedef TimerBasePtr		TimerHandle;

/** 
	RPC.NET�� �½�ũ �����ٷ��� Application���� �񵿱������� ȣ��Ǵ� �۾� �� Ÿ�̸ӵ��� �����Ѵ�.
	�½�ũ�����ٷ��� ����Ǹ� 1���� �����尡 �����Ǹ� �� ������� �۾� �� Ÿ�̸� ��� ����Ʈ�� ������ �������� ��ȸ�ϸ�
	ȣ��ð��� ����� �۾��̳� Ÿ�̸��� �ݹ� �Լ��� ȣ���մϴ�. �׸��� �½�ũ �����ٷ��� ���Ѱ��� �߿��� ������ �ܺη�
	���� ������ �񵿱����ν����� ���� ��û�� ó���մϴ�.
*/
struct TaskScheduler
{
	/** 
		�½�ũ�����ٷ� ID �����Ѵ�.
		@param iTaskSchedulerID �½�ũ�����ٷ�ID
	*/
	virtual void			SetID( unsigned int iTaskSchedulerID ) = 0;
	/** 
		�½�ũ�����ٷ� ID ��ȸ�Ѵ�.
	*/
	virtual unsigned int	GetID( void ) = 0; 
	/** 
		Task ȣ�Ⱓ�� �����Ѵ�.
		@param iDuration Task ȣ�Ⱓ��(ms)
	*/
	virtual void			SetDuration( unsigned long iDuration ) = 0;
	/** 
		Task ȣ�Ⱓ�� ��ȸ�Ѵ�.
	*/
	virtual unsigned long	GetDuration( void ) = 0;
	/** 
		�½�ũ �����ٷ��� ����Ǵ� ������ ID�� ��ȸ�Ѵ�.
	*/
	virtual DWORD			GetThreadId( void ) = 0;
	/** 
		�½�ũ�����ٷ��� Task ����Ѵ�.
		@param pTask �½�ũ ��ü�� �ּҰ�
		@return �½�ũ ����ID
	*/
	virtual int				RegTask( TaskBase* pTask ) = 0;
	/** 
		Task ��ü�� ��ȸ�Ѵ�.
		@param Task ����ID
	*/
	virtual TaskBase*		GetTask( int iTaskID ) = 0;
	/** 
		�½�ũ �����ٷ��� �۾��̺�Ʈ ť�� �۾��̺�Ʈ�� �����Ѵ�.
		@param pMsg �۾��̺�Ʈ ��ü�� �ּҰ�( �۾��̺�Ʈ�� TaskEventMsg Ŭ������ ��ӹ޾� �����Ѵ� )
	*/
	virtual int				Post( TaskEventMsg* pMsg ) = 0;
	/** 
		�½�ũ �����ٷ��� ��ϵ� Task �� �����մϴ�.
		@param iTaskID �½�ũ ���� ID
	*/
	virtual int				Signal( int iTaskID ) = 0;
	/** 
		�½�ũ�����ٷ��� Ÿ�̸� ����մϴ�.
		@param pTimer Ÿ�̸� ��ü�� �ּҰ�
	*/
	virtual TimerHandle		RegTimer( TimerBase* pTimer ) = 0;
	/** 
		�½�ũ�����ٷ��� ��ϵ� Ÿ�̸Ӹ� ����մϴ�.
		@param Key Ÿ�̸�Ű
	*/
	virtual void			CancelTimer( TimerHandle& spTimer ) = 0;
	/** 
		�½�ũ �����췯�� �̺�Ʈ ó�� ����
	*/
	virtual void			RunEventLoop( void ) = 0;
	/**
		�׽�ũ �����ٷ� �̺�Ʈ ó�� ������ ���� ����� �����Ѵ�.
	*/
	virtual void			Finalize( void ) = 0;
};


/** 
	�½�ũ�����ٷ��� ����, ����, �����ϴ� ���� �Լ��� �����ϴ� ���ӽ����̽�.
*/
namespace TaskSchedulerFactory
{
	/** 
		�½�ũ�����ٷ� ��ü�� �������·� �����Ѵ�.
		@param iTaskSchedulerID ������ �½�ũ �����ٷ��� ������ ID ������ 1 ~ ���� �ִ밪 �����Ѵ�.
		�½�ũ�����ٷ� ��ü�� �˻��ϰų� �½�ũ�����ٷ��� �½�ũ, Ÿ�̸�, �񵿱� ���ν����� ����� �½�ũ�����ٷ�ID�� ����Ѵ�.
		@param iDuration �½�ũ �����ٷ��� ��ϵ� �½�ũ�� Ÿ�̸��� ȣ��ð��� ���� �ƴ��� �����ϴ� ����(ms ����)
		@param iBindCPU �½�ũ �����ٷ��� �����ϸ� �����尡 1�� �����Ǵµ� �� �����带 � CPU ���� �����ϵ��� �� �� ������ ����
	*/
	TaskScheduler*	Create( unsigned int iTaskSchedulerID, unsigned long iDuration, unsigned short iBindCPU );
	/** 
		�½�ũ�����ٷ� ��ü�� ��ȸ�մϴ�.
		@param iTaskSchedulerID �½�ũ�����ٷ��� ����ID
	*/
	TaskScheduler*	Get( unsigned int iTaskSchedulerID );
	/** 
		���� ������ �½�ũ�����ٷ��� �����մϴ�.
		@param iTaskSchedulerID �½�ũ�����ٷ��� ����ID, ���� 0�� �����ϸ� ��� �׽�ũ�����ٷ��� ��Ÿ����.
	*/
	bool			Start( unsigned int iTaskSchedulerID = 0 );
	/** 
		�½�ũ�����ٷ��� �����մϴ�.
		@param iTaskSchedulerID �½�ũ�����ٷ��� ����ID, ���� 0�� �����ϸ� ��� �׽�ũ�����ٷ��� ��Ÿ����.
	*/
	bool			Stop( unsigned int iTaskSchedulerID = 0 );
	/** 
		�½�ũ�����ٷ��� �񵿱� ���ν����� ����Ѵ�.
		@param iTaskSchedulerID �½�ũ�����ٷ��� ����ID
		@param iProcID �񵿱����ν��� ����ID
		@param pProc �񵿱����ν��� �Լ���
	*/
	bool			RegProcedure( unsigned int iTaskSchedulerID, unsigned int iProcID, procedure_base* pProc );
	/** 
		�½�ũ�����ٷ��� �񵿱� ���ν��� ���̺��� ��ȸ�Ͽ� ��Ī�Ǵ� �񵿱� ���ν����� �����Ѵ�.
		@param iProcID �񵿱����ν��� ����ID
		@param pRef �񵿱����ν����� ȣ���� ��ü
		@param pBuf ������ �Ű������� �����ϴ� ����
	*/
	bool			CallProcedure( unsigned int iProcID, interface_ref* pRef, Buffer* pBuf );
	/** 
		�½�ũ�����ٷ��� �½�ũ�� ����Ѵ�. �½�ũ �����ٷ��� �񵿱� ���ν����� ��ϵ� ��ü�� �����ϸ�
		�񵿱� ���ν��� ���̺� ����� ��ȿ���� �Ұ� �ǹǷ� ��ϵ� ��ü�� �����ϸ� �ȵȴ�.
		@param iTaskSchedulerID �½�ũ�����ٷ��� ����ID
		@param o �����忡�� ȣ���� �Լ��� ����� Ŭ������ ������
		@param f Ŭ���� �ɹ��Լ� ������
		@param iDuration ȣ�Ⱓ��(ms)
	*/
	template<class O, class F>
	int Task( unsigned int iTaskSchedulerID, const O& o, F f, unsigned long iDuration )
	{
		TaskScheduler* pTSch = TaskSchedulerFactory::Get(iTaskSchedulerID);
		if ( pTSch )
		{
			TaskBase* pTB = new TaskBase(iDuration);
			pTB->m_Fn.assign(o,f);
			return pTSch->RegTask( pTB );
		}

		return -1;
	}
	/** 
		�½�ũ�����ٷ��� Ÿ�̸Ӹ� ����Ѵ�.
		@param iTaskSchedulerID �½�ũ�����ٷ��� ����ID
		@param o �����忡�� ȣ���� �Լ��� ����� Ŭ������ ������
		@param f Ŭ���� �ɹ��Լ� ������
		@param iDuration ȣ�Ⱓ��(ms)
		@param pUserData Ÿ�̸�ȣ��� ���޵� �Ű�����
	*/
	template<class O, class F>
	TimerHandle Timer( unsigned int iTaskSchedulerID, const O& o, F f, unsigned long iDuration, void* pUserData )
	{
		TaskScheduler* pTSch = TaskSchedulerFactory::Get(iTaskSchedulerID);
		if ( pTSch )
		{
			TimerBase* pTimer = new TimerBase;
			pTimer->m_iDuration = iDuration;
			pTimer->m_pUserData = pUserData;
			pTimer->m_Fn.assign(o,f);
			return pTSch->RegTimer( pTimer );
		}

		return nullptr;
	}

	/** 
		�½�ũ�����ٷ��� ����ð��� �������� ȣ��� Ÿ�̸Ӹ� ����Ѵ�.
		@param iTaskSchedulerID �½�ũ�����ٷ��� ����ID
		@param o �����忡�� ȣ���� �Լ��� ����� Ŭ������ ������
		@param f Ŭ���� �ɹ��Լ� ������
		@param iAbsTime ����ð�
		@param iDuration ȣ�Ⱓ��(ms)
		@param pUserData Ÿ�̸�ȣ��� ���޵� �Ű�����
	*/
	template<class O, class F>
	TimerHandle Timer( unsigned int iTaskSchedulerID, const O& o, F f, unsigned __int64 iAbsTime, unsigned long iDuration, void* pUserData )
	{
		TaskScheduler* pTSch = TaskSchedulerFactory::Get(iTaskSchedulerID);
		if ( pTSch )
		{
			TimerBase* pTimer = new TimerBase;
			pTimer->m_iTm = iAbsTime;
			pTimer->m_iDuration = iDuration;
			pTimer->m_pUserData = pUserData;
			pTimer->m_Fn.assign(o,f);
			return pTSch->RegTimer( pTimer );
		}

		return nullptr;
	}
	/** 
		�½�ũ�����ٷ��� ��ϵ� Ÿ�̸Ӹ� �����մϴ�.
		@param iTaskSchedulerID �½�ũ�����ٷ����̵�
		@param spTimer Ÿ�̸Ӹ� �����Ҷ� �Ҵ�� �ڵ�
	*/
	void CancelTimer( unsigned int iTaskSchedulerID, TimerHandle& spTimer );
};

/** 
	@def �½�ũ�����ٷ��� �����մϴ�.
	@param taskschedulerid �½�ũ�����ٷ����̵�
	@param duration �½�ũ �����ٷ��� ��ϵ� �½�ũ�� Ÿ�̸��� ȣ��ð��� ���� �ƴ��� �����ϴ� ����(ms ����)
	@param bindcpu �½�ũ�����ٷ� �������� ��ȣ��
*/
#define CREATE_TASKSCEDULER(taskschedulerid, duration, bindcpu)	\
	TaskSchedulerFactory::Create( taskschedulerid, duration, bindcpu )
/** 
	@def �½�ũ�����ٷ��� ��ȸ�մϴ�.
	@param taskschedulerid �½�ũ�����ٷ����̵�
*/
#define GET_TASKSCHEDULER(taskschedulerid)	\
	TaskSchedulerFactory::Get( taskschedulerid )
/**
	@def �½�ũ�����ٷ��� �����մϴ�.
	@param taskschedulerid �½�ũ�����ٷ����̵�
*/
#define START_TASKSCHEDULER( taskschedulerid ) \
	TaskSchedulerFactory::Start( taskschedulerid )
/** 
	@def �½�ũ�����ٷ��� �����մϴ�.
	@param taskschedulerid �½�ũ�����ٷ����̵�
*/
#define STOP_TASKSCHEDULER( taskschedulerid ) \
	TaskSchedulerFactory::Stop( taskschedulerid )
/** 
	@def �½�ũ�����ٷ��� Task�� ����մϴ�.
	@param taskschedulerid �½�ũ�����ٷ����̵�
	@param o �����忡�� ȣ���� �Լ��� ����� Ŭ������ ������
	@param func Ŭ���� �ɹ��Լ� ������
	@param duration �½�ũ �����ٷ��� ��ϵ� �½�ũ�� Ÿ�̸��� ȣ��ð��� ���� �ƴ��� �����ϴ� ����(ms ����)
*/
#define CREATE_TASK(taskschedulerid, obj, func, duration ) \
	TaskSchedulerFactory::Task( taskschedulerid, obj, func, duration )
/** 
	@def �½�ũ�����ٷ��� �ݺ� ȣ��Ǵ� Ÿ�̸Ӹ� ����մϴ�.
	@param taskschedulerid �½�ũ�����ٷ����̵�
	@param o �����忡�� ȣ���� �Լ��� ����� Ŭ������ ������
	@param func Ŭ���� �ɹ��Լ� ������
	@param duration �½�ũ �����ٷ��� ��ϵ� �½�ũ�� Ÿ�̸��� ȣ��ð��� ���� �ƴ��� �����ϴ� ����(ms ����)
	@param userdata Ÿ�̸Ӱ� ȣ��ɶ� ������ ����� �ּҰ�
*/
#define CREATE_TIMER(taskschedulerid, obj, func, duration, userdata ) \
	TaskSchedulerFactory::Timer( taskschedulerid, obj, func, duration, userdata )
/** 
	@def �½�ũ�����ٷ��� ����ð��� �������� �ݺ�ȣ��Ǵ� Ÿ�̸Ӹ� ����մϴ�.
	@param taskschedulerid �½�ũ�����ٷ����̵�
	@param o �����忡�� ȣ���� �Լ��� ����� Ŭ������ ������
	@param func Ŭ���� �ɹ��Լ� ������
	@param duration �½�ũ �����ٷ��� ��ϵ� �½�ũ�� Ÿ�̸��� ȣ��ð��� ���� �ƴ��� �����ϴ� ����(ms ����)
	@param userdata Ÿ�̸Ӱ� ȣ��ɶ� ������ ����� �ּҰ�
	@return TimerHandler
*/
#define CREATE_ABSTIMER(taskschedulerid, obj, func, msec, duration, userdata ) \
	TaskSchedulerFactory::Timer( taskschedulerid, obj, func, msec, duration, userdata )
/** 
	@def �½�ũ�����ٷ��� ��ϵ� Ÿ�̸Ӹ� �����մϴ�.
	@param taskschedulerid �½�ũ�����ٷ����̵�
	@param timerhandle Ÿ�̸Ӹ� �����Ҷ� �Ҵ�� �ڵ�
*/
#define CANCEL_TIMER( taskschedulerid, timerhandle ) \
	TaskSchedulerFactory::CancelTimer( taskschedulerid, timerhandle )