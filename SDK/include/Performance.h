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

/** 
	Proactor 의 성능 정보를 저장하는 구조체.

	패킷 송수신 횟수, 패킷 송수신 바이트, 연결 개수와 같은 네트웍 성능정보를 관리한다.
	@verbatim
	Proacotor_Counter p = performance::proactor_counter();

	@endverbatim
*/
struct Proactor_Counter
{
	//< 누적 통계
	volatile long	iTCPReceiveBytes;		///< TCP/IP 수신 바이트
	volatile long	iTCPReceives;			///< TCP/IP 수신 횟수
	volatile long	iTCPSentBytes;			///< TCP/IP 전송 바이트
	volatile long	iTCPSents;				///< TCP/IP 전송 횟수
	volatile long	iInvalidLengths;		///< 패킷 길이 오류 횟수
	volatile long	iInsufficentLengths;	///< 부족한 패킷 횟수
	volatile long	iResizeBufferLengths;	///< 패킷 수신 버퍼 크기 부족에 따른 버퍼 재할당 횟수

	//< Proactor 통계
	volatile long	iBuffers;				///< Buffer 개수
	volatile long	iTCPNetLinks;			///< TCP/IP NetLink 개수
	volatile long	iTCPAcceptCtxs;			///< TCP/IP 클라이언트 접속 대기 요청 횟수
	volatile long	iTCPReceiveCtxs;		///< TCP/IP 패킷 수신 요청 횟수
	volatile long	iTCPSentCtxs;			///< TCP/IP 패킷 전송 요청 횟수
	volatile long	iTCPUnlinkCtxs;			///< TCP/IP 클라이언트 종료 요청 횟수
	volatile long	iTCPConnectCtxs;		///< TCP/IP 서버 연결 요청 횟수

	//< 누적 통계
	volatile long	iUDPReceiveBytes;		///< UDP 수신 바이트
	volatile long	iUDPReceives;			///< UDP 수신 횟수
	volatile long	iUDPSentBytes;			///< UDP 전송 바이트
	volatile long	iUDPSents;				///< UDP 전송 횟수

	//< Proactor 통계
	volatile long	iUDPNetLinks;			///< UDP NetLink 개수
	volatile long	iUDPReceiveCtxs;		///< UDP 패킷 수신 요청 횟수
	volatile long	iUDPSentCtxs;			///< UDP 패킷 전송 요청 횟수

	//< 태스크 스케줄러 통계
	volatile long	iNetAcceptEvents;		///< 클라이언트 접속 작업이벤트 개수
	volatile long	iNetConnectEvents;		///< 서버 연결 작업이벤트 개수
	volatile long	iNetReceiveEvents;		///< 패킷 수신 작업이벤트 개수
	volatile long	iNetFunctorEvents;		///< 네트웍 함수 호출 작업이벤트 개수
	volatile long	iNetClosedEvents;		///< 연결 종료 작업이벤트 개수
	volatile long	iNetRemoteCallEvents;	///< RPC 호출 작업이벤트 개수

	//< RPC 통계
	volatile long	iRemoteProcedures;		///< RPC 등록 개수
	Proactor_Counter();
};

/** 
	TaskScheduler 의 성능 정보를 저장하는 구조체.

	@verbatim
	TaskScheduler_Counter p = performance::taskscheduler_counter();

	@endverbatim
*/
struct TaskScheduler_Counter
{
	volatile long	iMaxTaskElapsedTime;		///< 최대 작업완료 소요시간
	volatile long	iMinTaskElapsedTime;		///< 최소 작업완료 소요시간
	volatile long	iMaxTimerElapsedTime;		///< 최대 타이머완료 소요시간
	volatile long	iMinTimerElapsedTime;		///< 최소 타이머완료 소요시간
	volatile long	iMaxEventElapsedTime;		///< 최대 이벤트완료 소요시간
	volatile long	iMinEventElapsedTime;		///< 최소 이벤트완료 소요시간

	TaskScheduler_Counter();
};

/** 
	RPC.NET의 성능정보 구조체
*/
struct Performance_Counter
{
	Proactor_Counter		proactor;
	TaskScheduler_Counter	taskscheduler;
};

/** 
	RPC.NET 의 성능정보를 얻어오는 인터페이스를 제공하는 네임스페이스.
*/
namespace performance
{
	/** 
		무자금메모리풀, Proactor, 태스크스케줄러의 성능정보 구조체를 얻어온다.
	*/
	Performance_Counter&	counter( void );
	/** 
		Proactor의 성능정보 구조체를 복사한다.
	*/
	Proactor_Counter		proactor_counter();

	/** 
		태스크스케줄러의 성능정보 구조체를 복사한다.
	*/
	TaskScheduler_Counter	taskscheduler_counter();
};

#define PROACTOR_COUNTER		performance::counter().proactor
#define TASKSCHEDULER_COUNTER	performance::counter().taskscheduler