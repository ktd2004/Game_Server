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
#include <ThreadFactory.h>
#include <TaskSchedulerFactory.h>

/** 
	Ÿ�̸� Ŭ����
*/
class Timer
{
private:
	struct timer_order
	{
		int operator() ( const TimerBase* a, const TimerBase* b )
		{
			return a->m_iTm > b->m_iTm;
		}
	};

	typedef std::priority_queue<TimerBase*, std::vector<TimerBasePtr>, timer_order>	TimerQ;
	TimerQ		m_TimerQ;

public:
	Timer();
	virtual ~Timer();

	TimerHandle		RegTimer( TimerBase* pTimer );
	void			Expire( void );
	void			Cancel( TimerHandle& spTimer );
};

/** 
	�½�ũ �� Ÿ�̸ӵ��, �½�ũ �� Ÿ�̸� �ݹ��Լ����� �ֱ����� ȣ���ϴ� Ŭ����
*/
class TaskSchedulerImpl : public TaskScheduler
{
protected:
	typedef std::map<int, TaskBase*> TaskList;	///< �½�ũ�� Ÿ�Լ���
	unsigned long			m_iLast;			///< ������ȣ��ð�
	unsigned long			m_iDuration;		///< �½�Ʈȣ�Ⱓ��
	TaskList				m_TaskList;			///< �½�ũ��
	int						m_iTaskSeq;			///< �½�ũ�Ϸù�ȣ
	HANDLE					m_hIOCP;			///< IOCP�ڵ�
	unsigned int			m_iTaskSchedulerID;	///< �½�ũ�����ٷ�ID
	Timer					m_Timer;			///< Ÿ�̸�
	spRef<Thread>			m_Thread;			///< ������
	bool					m_bResume;			///< �����尡 ���� �������� ����

	enum PostMsg
	{
		eTask_FIN = 0xFFFFFFFF,
		eTask_DAT = 0xFFFFFFFE,
	};

public:
	TaskSchedulerImpl();
	TaskSchedulerImpl(const TaskSchedulerImpl& r);
	virtual ~TaskSchedulerImpl();

public:
	/** 
		�½�ũ�����ٷ� ���̵� �����Ѵ�.
		@param iTaskSchedulerID �½�ũ�����ٷ� ���̵�
	*/
	void			SetID( unsigned int iTaskSchedulerID ) override;
	/** 
		�½�ũ�����ٷ� ���̵� ��ȸ�Ѵ�.
	*/
	unsigned int	GetID( void ) override;
	/** 
		�½�ũ �����ٷ��� ��ϵ� �½�ũ�� Ÿ�̸��� ȣ��ð��� ���� �ƴ��� �����ϴ� ����(ms ����)�� �����Ѵ�.
		@param iDuration �����ϴ� ����(ms ����)
	*/
	void			SetDuration( unsigned long iDuration ) override;
	/** 
		�½�ũ �����ٷ��� ��ϵ� �½�ũ�� Ÿ�̸��� ȣ��ð��� ���� �ƴ��� �����ϴ� ����(ms ����)�� ��ȸ�Ѵ�.
	*/
	unsigned long	GetDuration( void ) override;
	/** 
		�½�ũ�����ٷ� �������� ���̵� ��ȸ�Ѵ�.
	*/
	DWORD			GetThreadId( void ) override;
	/** 
		�½�ũ�� ����Ѵ�.
		@param pTask �½�ũ ������Ʈ�� �ּҰ�
	*/
	int				RegTask( TaskBase* pTask ) override;
	/** 
		�½�ũ�� ��ȸ�Ѵ�.
		@param iTaskID �½�ũ ���̵�
	*/
	TaskBase*		GetTask( int iTaskID ) override;
	/** 
		�½�ũ�����ٷ����� �۾��� �����Ѵ�.
		@param pMsg �۾� ������Ʈ�� �ּҰ�
	*/
	int				Post( TaskEventMsg* pMsg ) override;
	/** 
		�½�ũ�� ������ ��û�Ѵ�.
		@param iTaskID �½�ũ �������̵�
	*/
	int				Signal( int iTaskID ) override;
	/** 
		Ÿ�̸Ӹ� ����Ѵ�.
		@param pTimer Ÿ�̸� ������Ʈ�� �ּҰ�
	*/
	TimerHandle		RegTimer( TimerBase* pTimer ) override;
	/** 
		Ÿ�̸� ����� ����Ѵ�.
		@param spTimer Ÿ�̸� ������Ʈ�� �ּҰ�
	*/
	void			CancelTimer( TimerHandle& spTimer ) override;
	/**
		�׽�Ʈ �����ٷ��� �̺�Ʈ ó�� ����
	*/
	void			RunEventLoop(void) override;
	/**
		�׽�ũ �����ٷ� �̺�Ʈ ó�� ������ ���� ����� �����Ѵ�.
	*/
	void			Finalize(void) override;
	/** 
		�½�ũ�����ٷ��� ���� ���·� �����Ѵ�.
		@param iBindCPU �½�ũ�����ٷ��������� ��ȣ��
	*/
	bool			Start( unsigned short iBindCPU );
	/** 
		�½�ũ�����ٷ��� �����Ѵ�.
	*/
	void			Resume( void );
	/** 
		�½�ũ�����ٷ� �����带 ������ �����Ѵ�.
	*/
	void			Terminate( void );
	/** 
		�½�ũ�����ٷ��� �����尡 ����Ǳ⸦ ��ٸ���
	*/
	void			Wait( void );
	/** 
		�½�Ʈ�� �½�ũ��Ͽ� �����Ѵ�.
		@param pTask �½�ũ ������Ʈ�� �ּҰ�
	*/
	void			Insert( TaskBase* pTask );
	/** 
		Ÿ�̸Ӹ� Ÿ�̸Ӹ�Ͽ� �����Ѵ�.
		@param pTimer Ÿ�̸� ������Ʈ�� �ּҰ�
	*/
	void			Insert( TimerBase* pTimer );
	
private:
	/** IOCP ť ���� */
	bool			Open( void );
	/* IOCP ť ���� */
	bool			Close( void );
	/* ��ϵ� Task �ݹ��Լ� ȣ�� */
	unsigned long	DoTask( unsigned long iStart );
	/* �޼����� ���´�. */
//	int				ObtainMessage( OVERLAPPED*& pMsg, unsigned int iMSec );
};
