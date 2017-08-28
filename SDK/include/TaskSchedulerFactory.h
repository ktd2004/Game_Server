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
	태스크스케줄러가 작업이벤트를 실행할때 호출되는 콜백 함수자의 타입선언.
*/
typedef functional_v0<void>		TaskFn_V0;

/** 
	태스크 정보 관리 클래스.

	태스크스케줄러에 의해 호출되는 Task들의 호출 간격, 평균 수행시간등의 정보를 관리하는 클래스이다.
*/
class TaskBase
{
public:
	TaskFn_V0			m_Fn;			///< 태스크콜백함수
	unsigned long		m_iStart;		///< 태스크콜백함수호출시작시간
	unsigned long		m_iDuration;	///< 태스크콜백함수호출간격
	double				m_dAvg;			///< 평균수행시간

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
	태스크스케줄러가 타이머를 실행할때 호출되는 콜백 함수자의 타입선언.
*/
typedef functional_v1<bool, void*>		TimerFn_V1;

PTR(TimerBase);
/** 
	타이머 정보 관리 클래스.

	태스크스케줄러에 의해 호출되는 타이머들의 호출 간격, 수행시간등의 정보를 관리하는 클래스이다.
*/
class TimerBase : public shared_ref
{
private:
	TimerBase(const TimerBase& r ) {}

public:
	TimerFn_V1			m_Fn;			///< 타이머콜백함수
	void*				m_pUserData;	///< 사용자데이타
	unsigned __int64	m_iTm;			///< 호출시간
	unsigned long		m_iDuration;	///< 타이머콜백호출간격
	bool				m_bCancel;		///< 타이머 취소
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
	타이머키 정의
*/
typedef TimerBasePtr		TimerHandle;

/** 
	RPC.NET의 태스크 스케줄러는 Application에서 비동기적으로 호출되는 작업 및 타이머들을 관리한다.
	태스크스케줄러가 실행되면 1개의 쓰레드가 생성되며 이 쓰레드는 작업 및 타이머 목록 리스트를 일정한 간격으로 순회하며
	호출시간이 만료된 작업이나 타이머의 콜백 함수를 호출합니다. 그리고 태스크 스케줄러의 또한가지 중요한 역할은 외부로
	부터 수신한 비동기프로시져의 실행 요청을 처리합니다.
*/
struct TaskScheduler
{
	/** 
		태스크스케줄러 ID 설정한다.
		@param iTaskSchedulerID 태스크스케줄러ID
	*/
	virtual void			SetID( unsigned int iTaskSchedulerID ) = 0;
	/** 
		태스크스케줄러 ID 조회한다.
	*/
	virtual unsigned int	GetID( void ) = 0; 
	/** 
		Task 호출간격 설정한다.
		@param iDuration Task 호출간격(ms)
	*/
	virtual void			SetDuration( unsigned long iDuration ) = 0;
	/** 
		Task 호출간격 조회한다.
	*/
	virtual unsigned long	GetDuration( void ) = 0;
	/** 
		태스크 스케줄러가 실행되는 쓰레드 ID를 조회한다.
	*/
	virtual DWORD			GetThreadId( void ) = 0;
	/** 
		태스크스케줄러에 Task 등록한다.
		@param pTask 태스크 객체의 주소값
		@return 태스크 고유ID
	*/
	virtual int				RegTask( TaskBase* pTask ) = 0;
	/** 
		Task 객체를 조회한다.
		@param Task 고유ID
	*/
	virtual TaskBase*		GetTask( int iTaskID ) = 0;
	/** 
		태스크 스케줄러의 작업이벤트 큐에 작업이벤트를 전달한다.
		@param pMsg 작업이벤트 객체의 주소값( 작업이벤트는 TaskEventMsg 클래스를 상속받아 구현한다 )
	*/
	virtual int				Post( TaskEventMsg* pMsg ) = 0;
	/** 
		태스크 스케줄러에 등록된 Task 에 실행합니다.
		@param iTaskID 태스크 고유 ID
	*/
	virtual int				Signal( int iTaskID ) = 0;
	/** 
		태스크스케줄러에 타이머 등록합니다.
		@param pTimer 타이머 객체의 주소값
	*/
	virtual TimerHandle		RegTimer( TimerBase* pTimer ) = 0;
	/** 
		태스크스케줄러에 등록된 타이머를 취소합니다.
		@param Key 타이머키
	*/
	virtual void			CancelTimer( TimerHandle& spTimer ) = 0;
	/** 
		태스크 스케쥴러의 이벤트 처리 루프
	*/
	virtual void			RunEventLoop( void ) = 0;
	/**
		테스크 스케줄러 이벤트 처리 루프에 종료 명령을 전송한다.
	*/
	virtual void			Finalize( void ) = 0;
};


/** 
	태스크스케줄러를 생성, 시작, 종료하는 전역 함수를 제공하는 네임스페이스.
*/
namespace TaskSchedulerFactory
{
	/** 
		태스크스케줄러 객체를 정지상태로 생성한다.
		@param iTaskSchedulerID 생성할 태스크 스케줄러의 고유한 ID 값으로 1 ~ 정수 최대값 설정한다.
		태스크스케줄러 객체를 검색하거나 태스크스케줄러에 태스크, 타이머, 비동기 프로시져를 등록할 태스크스케줄러ID을 사용한다.
		@param iDuration 태스크 스케줄러에 등록된 태스크나 타이머의 호출시간이 만료 됐는지 조사하는 간격(ms 단위)
		@param iBindCPU 태스크 스케줄러를 생성하면 쓰레드가 1개 생성되는데 이 쓰레드를 어떤 CPU 에서 실행하도록 할 것 인지를 설정
	*/
	TaskScheduler*	Create( unsigned int iTaskSchedulerID, unsigned long iDuration, unsigned short iBindCPU );
	/** 
		태스크스케줄러 객체를 조회합니다.
		@param iTaskSchedulerID 태스크스케줄러의 고유ID
	*/
	TaskScheduler*	Get( unsigned int iTaskSchedulerID );
	/** 
		정지 상태의 태스크스케줄러를 실행합니다.
		@param iTaskSchedulerID 태스크스케줄러의 고유ID, 만약 0을 설정하면 모든 테스크스케줄러를 나타낸다.
	*/
	bool			Start( unsigned int iTaskSchedulerID = 0 );
	/** 
		태스크스케줄러를 종료합니다.
		@param iTaskSchedulerID 태스크스케줄러의 고유ID, 만약 0을 설정하면 모든 테스크스케줄러를 나타낸다.
	*/
	bool			Stop( unsigned int iTaskSchedulerID = 0 );
	/** 
		태스크스케줄러에 비동기 프로시져를 등록한다.
		@param iTaskSchedulerID 태스크스케줄러의 고유ID
		@param iProcID 비동기프로시져 고유ID
		@param pProc 비동기프로시져 함수자
	*/
	bool			RegProcedure( unsigned int iTaskSchedulerID, unsigned int iProcID, procedure_base* pProc );
	/** 
		태스크스케줄러의 비동기 프로시져 테이블을 조회하여 매칭되는 비동기 프로시져를 실행한다.
		@param iProcID 비동기프로시져 고유ID
		@param pRef 비동기프로시져를 호출한 객체
		@param pBuf 전달할 매개변수를 저장하는 버퍼
	*/
	bool			CallProcedure( unsigned int iProcID, interface_ref* pRef, Buffer* pBuf );
	/** 
		태스크스케줄러에 태스크를 등록한다. 태스크 스케줄러에 비동기 프로시져로 등록된 객체를 삭제하면
		비동기 프로시져 테이블 목록의 유효성을 잃게 되므로 등록된 객체는 삭제하면 안된다.
		@param iTaskSchedulerID 태스크스케줄러의 고유ID
		@param o 쓰레드에서 호출할 함수가 선언된 클래스의 포인터
		@param f 클래스 맴버함수 포인터
		@param iDuration 호출간격(ms)
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
		태스크스케줄러에 타이머를 등록한다.
		@param iTaskSchedulerID 태스크스케줄러의 고유ID
		@param o 쓰레드에서 호출할 함수가 선언된 클래스의 포인터
		@param f 클래스 맴버함수 포인터
		@param iDuration 호출간격(ms)
		@param pUserData 타이머호출시 전달될 매개변수
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
		태스크스케줄러에 절대시간을 기준으로 호출될 타이머를 등록한다.
		@param iTaskSchedulerID 태스크스케줄러의 고유ID
		@param o 쓰레드에서 호출할 함수가 선언된 클래스의 포인터
		@param f 클래스 맴버함수 포인터
		@param iAbsTime 절대시간
		@param iDuration 호출간격(ms)
		@param pUserData 타이머호출시 전달될 매개변수
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
		태스크스케줄러에 등록된 타이머를 삭제합니다.
		@param iTaskSchedulerID 태스크스케줄러아이디
		@param spTimer 타이머를 생성할때 할당된 핸들
	*/
	void CancelTimer( unsigned int iTaskSchedulerID, TimerHandle& spTimer );
};

/** 
	@def 태스크스케줄러를 생성합니다.
	@param taskschedulerid 태스크스케줄러아이디
	@param duration 태스크 스케줄러에 등록된 태스크나 타이머의 호출시간이 만료 됐는지 조사하는 간격(ms 단위)
	@param bindcpu 태스크스케줄러 쓰레드의 선호도
*/
#define CREATE_TASKSCEDULER(taskschedulerid, duration, bindcpu)	\
	TaskSchedulerFactory::Create( taskschedulerid, duration, bindcpu )
/** 
	@def 태스크스케줄러를 조회합니다.
	@param taskschedulerid 태스크스케줄러아이디
*/
#define GET_TASKSCHEDULER(taskschedulerid)	\
	TaskSchedulerFactory::Get( taskschedulerid )
/**
	@def 태스크스케줄러를 실행합니다.
	@param taskschedulerid 태스크스케줄러아이디
*/
#define START_TASKSCHEDULER( taskschedulerid ) \
	TaskSchedulerFactory::Start( taskschedulerid )
/** 
	@def 태스크스케줄러를 종료합니다.
	@param taskschedulerid 태스크스케줄러아이디
*/
#define STOP_TASKSCHEDULER( taskschedulerid ) \
	TaskSchedulerFactory::Stop( taskschedulerid )
/** 
	@def 태스크스케줄러에 Task를 등록합니다.
	@param taskschedulerid 태스크스케줄러아이디
	@param o 쓰레드에서 호출할 함수가 선언된 클래스의 포인터
	@param func 클래스 맴버함수 포인터
	@param duration 태스크 스케줄러에 등록된 태스크나 타이머의 호출시간이 만료 됐는지 조사하는 간격(ms 단위)
*/
#define CREATE_TASK(taskschedulerid, obj, func, duration ) \
	TaskSchedulerFactory::Task( taskschedulerid, obj, func, duration )
/** 
	@def 태스크스케줄러에 반복 호출되는 타이머를 등록합니다.
	@param taskschedulerid 태스크스케줄러아이디
	@param o 쓰레드에서 호출할 함수가 선언된 클래스의 포인터
	@param func 클래스 맴버함수 포인터
	@param duration 태스크 스케줄러에 등록된 태스크나 타이머의 호출시간이 만료 됐는지 조사하는 간격(ms 단위)
	@param userdata 타이머가 호출될때 전달할 사용자 주소값
*/
#define CREATE_TIMER(taskschedulerid, obj, func, duration, userdata ) \
	TaskSchedulerFactory::Timer( taskschedulerid, obj, func, duration, userdata )
/** 
	@def 태스크스케줄러에 절대시간을 기준으로 반복호출되는 타이머를 등록합니다.
	@param taskschedulerid 태스크스케줄러아이디
	@param o 쓰레드에서 호출할 함수가 선언된 클래스의 포인터
	@param func 클래스 맴버함수 포인터
	@param duration 태스크 스케줄러에 등록된 태스크나 타이머의 호출시간이 만료 됐는지 조사하는 간격(ms 단위)
	@param userdata 타이머가 호출될때 전달할 사용자 주소값
	@return TimerHandler
*/
#define CREATE_ABSTIMER(taskschedulerid, obj, func, msec, duration, userdata ) \
	TaskSchedulerFactory::Timer( taskschedulerid, obj, func, msec, duration, userdata )
/** 
	@def 태스크스케줄러에 등록된 타이머를 삭제합니다.
	@param taskschedulerid 태스크스케줄러아이디
	@param timerhandle 타이머를 생성할때 할당된 핸들
*/
#define CANCEL_TIMER( taskschedulerid, timerhandle ) \
	TaskSchedulerFactory::CancelTimer( taskschedulerid, timerhandle )