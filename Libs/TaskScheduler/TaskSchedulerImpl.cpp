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
#include "TaskSchedulerImpl.h"
#include <Performance.h>
#include <Errorcodes.h>
#include <TaskEventMsg.h>
#include <TimeUtil.h>
#include <MMSystem.h>
#include <Logger.h>

namespace
{
	TaskScheduler_Counter& g_taskscheduler = TASKSCHEDULER_COUNTER;
};


//////////////////////////////////////////////////////////////////////////
Timer::Timer()
{
}

Timer::~Timer()
{
	while( m_TimerQ.empty() )
	{
		m_TimerQ.pop();
	}
}

TimerHandle Timer::RegTimer(TimerBase* pTimer)
{
	m_TimerQ.push( pTimer );
	return pTimer;
}

void Timer::Expire(void)
{
	unsigned __int64 t = util::time2msec();
	while ( m_TimerQ.size() )
	{
		TimerBasePtr spTimer = m_TimerQ.top();
		if ( spTimer->m_bCancel )
		{
			m_TimerQ.pop();
			continue;
		}

		if ( spTimer->m_iTm > t )
		{
			break;
		}
		m_TimerQ.pop();

		bool bRepeat = false;
		unsigned __int64 tElapsed = util::time2msec();
		bRepeat = spTimer->m_Fn( spTimer->m_pUserData );
		tElapsed = (long)(util::time2msec() - tElapsed);

		if ( g_taskscheduler.iMaxTimerElapsedTime < (long)tElapsed )
		{
			locked_exchange( g_taskscheduler.iMaxTimerElapsedTime, tElapsed );
		}
		if ( g_taskscheduler.iMinTimerElapsedTime == 0 || g_taskscheduler.iMinTimerElapsedTime > (long)tElapsed )
		{
			locked_exchange( g_taskscheduler.iMinTimerElapsedTime, tElapsed );
		}
		if ( bRepeat )
		{
			spTimer->m_iTm = t + spTimer->m_iDuration;
			m_TimerQ.push( spTimer );
		}
	}
}

void Timer::Cancel( TimerHandle& spTimer )
{
	spTimer->m_bCancel = true;
}


//////////////////////////////////////////////////////////////////////////
TaskSchedulerImpl::TaskSchedulerImpl()
: m_iTaskSeq(0)
, m_iDuration(100)
, m_hIOCP(INVALID_HANDLE_VALUE)
, m_iTaskSchedulerID(0)
, m_iLast(timeGetTime())
, m_bResume(false)
{
	Open();
}

TaskSchedulerImpl::TaskSchedulerImpl( const TaskSchedulerImpl& r )
: m_iTaskSeq(0)
, m_iDuration(100)
, m_iTaskSchedulerID(0)
, m_iLast(timeGetTime())
, m_bResume(false)
{
	Open();
}

TaskSchedulerImpl::~TaskSchedulerImpl()
{
// 종료처리 하면서 삭제하면 메모리 오류가 발생한다. 어딘가에서 참조하고 하고 있는듯 하다.
// 어짜피 종료 처리이기 때문에 윈도우에서 메모리를 알아서 정리하겠지.
// 	for ( TaskList::iterator it = m_TaskList.begin(); it != m_TaskList.end(); ++it )
// 	{
// 		delete it->second;
// 	}
//	m_TaskList.clear();

	Close();
}

bool TaskSchedulerImpl::Open( void )
{
	m_hIOCP = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 1, 1 );
	return (this->m_hIOCP == 0 ? false : true);
}

bool TaskSchedulerImpl::Close( void )
{
	if ( m_hIOCP != INVALID_HANDLE_VALUE )
	{
		::CloseHandle(m_hIOCP);
		m_hIOCP = INVALID_HANDLE_VALUE;
		return true;
	}

	return false;
}

unsigned long TaskSchedulerImpl::DoTask( unsigned long iStart )
{
	//< task
	for( TaskList::iterator it = m_TaskList.begin(); it != m_TaskList.end(); ++it )
	{
		TaskBase& t = *(it->second);
		if ( t.m_iStart <= iStart )
		{
			t.m_Fn();
			t.m_iStart = iStart + t.m_iDuration;
			t.m_dAvg = t.m_dAvg ? (t.m_dAvg + (timeGetTime() - iStart)) * 0.5f : timeGetTime() - iStart ;
			if ( g_taskscheduler.iMaxTaskElapsedTime < t.m_dAvg )
			{
				locked_exchange(g_taskscheduler.iMaxTaskElapsedTime, t.m_dAvg );
			}
			if ( g_taskscheduler.iMinTaskElapsedTime == 0 || g_taskscheduler.iMinTaskElapsedTime > t.m_dAvg )
			{
				locked_exchange(g_taskscheduler.iMinTaskElapsedTime, t.m_dAvg );
			}
		}
	}

	m_Timer.Expire();

	unsigned long iDone = timeGetTime() - iStart;
	if ( iDone > m_iDuration )
	{
		m_iLast = iStart;
		return 0;
	}
	else
	{
		m_iLast = iStart + m_iDuration - iDone;
		return m_iDuration - iDone;
	}
}

void TaskSchedulerImpl::RunEventLoop( void )
{
	OVERLAPPED*		pMsg;
	unsigned long	iStart;
	unsigned long	iDuration;

//	prn_sys( ": start" );
	while (true)
	{
		pMsg		= nullptr;
		iStart		= timeGetTime();
		iDuration	= m_iDuration;

		if ( iStart > m_iLast )
			iDuration = DoTask( iStart );

//		int iRet = ObtainMessage( pMsg, iDuration );
		ULONG_PTR key = NULL;
		DWORD dwSize = 0;

		int iRet = ::GetQueuedCompletionStatus(
			m_hIOCP,
			&dwSize,
			&key,
			&pMsg,
			iDuration);

		if (iRet == 0 && ::GetLastError() == WAIT_TIMEOUT)
		{	// time out
			continue;
		}

		if (key == eTask_FIN)
		{
			prn_sys("task scheduler loop break");
			break;
		}

		if ( iRet && pMsg )
		{
			unsigned long iEvtStart = timeGetTime();
			TaskEventMsg* pEvt = static_cast<TaskEventMsg*>(pMsg);
			if ( pEvt )
			{
				if ( pEvt->Task() != TaskEventMsg::eInvalid_Task )
				{
					TaskList::iterator it = m_TaskList.find(pEvt->Task());
					if ( it != m_TaskList.end() )
					{
						TaskBase& t = *(it->second);
						t.m_Fn();
						t.m_iStart = iEvtStart + t.m_iDuration;
						t.m_dAvg = t.m_dAvg ? (t.m_dAvg + (timeGetTime() - iEvtStart)) * 0.5f : timeGetTime() - iEvtStart ;
						if ( g_taskscheduler.iMaxEventElapsedTime < t.m_dAvg )
						{
							locked_exchange( g_taskscheduler.iMaxEventElapsedTime, t.m_dAvg );
						}
						if ( g_taskscheduler.iMinEventElapsedTime == 0 || g_taskscheduler.iMinEventElapsedTime > t.m_dAvg )
						{
							locked_exchange( g_taskscheduler.iMinEventElapsedTime, t.m_dAvg );
						}
					}
				}
				pEvt->OnHandle();
				delete pEvt;
			}
		}
	}
//	prn_sys( "exit" );
}

void TaskSchedulerImpl::SetID( unsigned int iTaskSchedulerID )
{
	m_iTaskSchedulerID = iTaskSchedulerID;
}

unsigned int TaskSchedulerImpl::GetID( void )
{
	return m_iTaskSchedulerID;
}

void TaskSchedulerImpl::SetDuration( unsigned long iDuration )
{
	m_iDuration = iDuration;
}

unsigned long TaskSchedulerImpl::GetDuration( void )
{
	return m_iDuration;
}

class RegTaskEvent : public TaskEventMsg
{
public:
	TaskSchedulerImpl*	m_pTSch;
	TaskBase*			m_pTask;
	RegTaskEvent(TaskSchedulerImpl* pTSch, TaskBase* pTask)
		: m_pTSch(pTSch)
		, m_pTask(pTask)
	{
	}
	virtual ~RegTaskEvent() {}
	void OnHandle()
	{
		m_pTSch->Insert( m_pTask );
	}
};

int TaskSchedulerImpl::RegTask( TaskBase* pTask )
{
	pTask->m_iStart = timeGetTime() + pTask->m_iDuration;
	++m_iTaskSeq;

	if ( !m_bResume )
	{
		m_TaskList.insert( std::make_pair(m_iTaskSeq, pTask) );
		return m_iTaskSeq;
	}
	else
	{
		if ( Post( new RegTaskEvent(this, pTask) ) == -1 )
			return -1;
	}

	return m_iTaskSeq;
}

TaskBase* TaskSchedulerImpl::GetTask( int iTaskID )
{
	TaskList::iterator it = m_TaskList.find(iTaskID);
	if ( it != m_TaskList.end() )
	{
		return it->second;
	}
	return nullptr;
}

int TaskSchedulerImpl::Post( TaskEventMsg* pMsg )
{
// 	if ( ::PostQueuedCompletionStatus(
// 		m_hIOCP,
// 		sizeof(pMsg),
// 		1,
// 		(LPOVERLAPPED)pMsg) )
// 	{
// 		// Update the states once I succeed.
// 		return 1;
// 	}
// 
// 	delete pMsg;
// 	return -1;
	int iRet = ::PostQueuedCompletionStatus(
		m_hIOCP,
		sizeof(pMsg),
		eTask_DAT,
		(LPOVERLAPPED)pMsg);
	if (!iRet)
	{
		delete pMsg;
		return -1;
	}

	return 1;
}

// 
// int TaskSchedulerImpl::ObtainMessage( OVERLAPPED*& pMsg, unsigned int iMSec )
// {
// 	ULONG_PTR pCompletedKey = NULL;
// 	DWORD dwSize = 0;
// 
// 	int iRet = ::GetQueuedCompletionStatus(
// 		m_hIOCP,
// 		&dwSize,
// 		&pCompletedKey,
// 		&pMsg,
// 		iMSec );
// 
// 	if ( iRet && pMsg )
// 	{	// success
// 		return 1;
// 	}
// 	else if ( iRet == 0 && ::GetLastError() == WAIT_TIMEOUT)
// 	{	// time out
// 		return 0;
// 	}
// 
// 	return -1;
// }

class Wakeup : public TaskEventMsg
{
public:
	Wakeup(int iTask) : TaskEventMsg(iTask) {}
	~Wakeup() {}
	void OnHandle() {}
};

int TaskSchedulerImpl::Signal( int iTaskID )
{
	return Post( new Wakeup(iTaskID) );
}

class RegTimerEvent : public TaskEventMsg
{
public:
	TaskSchedulerImpl*	m_pTSch;
	TimerHandle			m_spTimer;
	RegTimerEvent(TaskSchedulerImpl* pTSch, TimerBase* pTimer)
		: m_pTSch(pTSch)
		, m_spTimer(pTimer)
	{
	}
	virtual ~RegTimerEvent() {}
	void OnHandle()
	{
		m_pTSch->Insert( m_spTimer );
	}
};

TimerBasePtr TaskSchedulerImpl::RegTimer( TimerBase* pTimer )
{
	if ( pTimer->m_iTm == 0 )
	{
		pTimer->m_iTm = util::time2msec() + pTimer->m_iDuration;
	}

	if ( !m_bResume || m_Thread->GetThreadId() == ::GetCurrentThreadId() )
	{
		return m_Timer.RegTimer( pTimer );
	}
	else
	{
		if ( Post( new RegTimerEvent(this, pTimer) ) > 0 )
		{
			return pTimer;
		}
		else
		{
			return nullptr;
		}
	}
}


void TaskSchedulerImpl::Insert( TaskBase* pTask )
{
	m_TaskList.insert(std::make_pair(m_iTaskSeq, pTask));
}

void TaskSchedulerImpl::Insert( TimerBase* pTimer )
{
	m_Timer.RegTimer( pTimer );
}


bool TaskSchedulerImpl::Start( unsigned short iBindCPU )
{
	m_Thread = ThreadFactory::Start( this, &TaskSchedulerImpl::RunEventLoop, iBindCPU );
	if ( m_Thread )
	{
		return true;
	}

	return false;
}

void TaskSchedulerImpl::Resume( void )
{
	if ( m_Thread )
	{
		m_bResume = true;
		m_Thread->Resume();
	}
}


void TaskSchedulerImpl::Terminate( void )
{
	if ( m_Thread )
		m_Thread->Terminate();
}

void TaskSchedulerImpl::Wait( void )
{
	if ( m_Thread )
		m_Thread->Wait();
}

DWORD TaskSchedulerImpl::GetThreadId( void )
{
	return m_Thread->GetThreadId();
}

void TaskSchedulerImpl::CancelTimer( TimerHandle& spTimer )
{
	m_Timer.Cancel( spTimer );
}

void TaskSchedulerImpl::Finalize(void)
{
	PostQueuedCompletionStatus(m_hIOCP, 0, eTask_FIN, 0);
}
