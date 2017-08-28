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
	타이머 클래스
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
	태스크 및 타이머등록, 태스크 및 타이머 콜백함수들을 주기적인 호출하는 클래스
*/
class TaskSchedulerImpl : public TaskScheduler
{
protected:
	typedef std::map<int, TaskBase*> TaskList;	///< 태스크맵 타입선언
	unsigned long			m_iLast;			///< 마지막호출시간
	unsigned long			m_iDuration;		///< 태스트호출간격
	TaskList				m_TaskList;			///< 태스크맵
	int						m_iTaskSeq;			///< 태스크일련번호
	HANDLE					m_hIOCP;			///< IOCP핸들
	unsigned int			m_iTaskSchedulerID;	///< 태스크스케줄러ID
	Timer					m_Timer;			///< 타이머
	spRef<Thread>			m_Thread;			///< 쓰레드
	bool					m_bResume;			///< 쓰레드가 시작 상태인지 여부

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
		태스크스케줄러 아이디를 설정한다.
		@param iTaskSchedulerID 태스크스케줄러 아이디
	*/
	void			SetID( unsigned int iTaskSchedulerID ) override;
	/** 
		태스크스케줄러 아이디를 조회한다.
	*/
	unsigned int	GetID( void ) override;
	/** 
		태스크 스케줄러에 등록된 태스크나 타이머의 호출시간이 만료 됐는지 조사하는 간격(ms 단위)을 설정한다.
		@param iDuration 조사하는 간격(ms 단위)
	*/
	void			SetDuration( unsigned long iDuration ) override;
	/** 
		태스크 스케줄러에 등록된 태스크나 타이머의 호출시간이 만료 됐는지 조사하는 간격(ms 단위)을 조회한다.
	*/
	unsigned long	GetDuration( void ) override;
	/** 
		태스크스케줄러 쓰레드의 아이디를 조회한다.
	*/
	DWORD			GetThreadId( void ) override;
	/** 
		태스크를 등록한다.
		@param pTask 태스크 오브젝트의 주소값
	*/
	int				RegTask( TaskBase* pTask ) override;
	/** 
		태스크를 조회한다.
		@param iTaskID 태스크 아이디
	*/
	TaskBase*		GetTask( int iTaskID ) override;
	/** 
		태스크스케줄러에게 작업을 전달한다.
		@param pMsg 작업 오브젝트의 주소값
	*/
	int				Post( TaskEventMsg* pMsg ) override;
	/** 
		태스크의 실행을 요청한다.
		@param iTaskID 태스크 고유아이디
	*/
	int				Signal( int iTaskID ) override;
	/** 
		타이머를 등록한다.
		@param pTimer 타이머 오브젝트의 주소값
	*/
	TimerHandle		RegTimer( TimerBase* pTimer ) override;
	/** 
		타이머 등록을 취소한다.
		@param spTimer 타이머 오브젝트의 주소값
	*/
	void			CancelTimer( TimerHandle& spTimer ) override;
	/**
		테스트 스케줄러의 이벤트 처리 루프
	*/
	void			RunEventLoop(void) override;
	/**
		테스크 스케줄러 이벤트 처리 루프에 종료 명령을 전송한다.
	*/
	void			Finalize(void) override;
	/** 
		태스크스케줄러를 정지 상태로 실행한다.
		@param iBindCPU 태스크스케줄러쓰레드의 선호도
	*/
	bool			Start( unsigned short iBindCPU );
	/** 
		태스크스케줄러를 실행한다.
	*/
	void			Resume( void );
	/** 
		태스크스케줄러 쓰레드를 강제로 종료한다.
	*/
	void			Terminate( void );
	/** 
		태스크스케줄러가 쓰레드가 종료되기를 기다린다
	*/
	void			Wait( void );
	/** 
		태스트를 태스크목록에 삽입한다.
		@param pTask 태스크 오브젝트의 주소값
	*/
	void			Insert( TaskBase* pTask );
	/** 
		타이머를 타이머목록에 삽입한다.
		@param pTimer 타이머 오브젝트의 주소값
	*/
	void			Insert( TimerBase* pTimer );
	
private:
	/** IOCP 큐 생성 */
	bool			Open( void );
	/* IOCP 큐 삭제 */
	bool			Close( void );
	/* 등록된 Task 콜백함수 호출 */
	unsigned long	DoTask( unsigned long iStart );
	/* 메세지를 얻어온다. */
//	int				ObtainMessage( OVERLAPPED*& pMsg, unsigned int iMSec );
};
