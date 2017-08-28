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

/**
	RPC.NET 에서 발생되는 에러코드를 정의한 상수

	MS의 권고 사항에 따라 사용자 정의 에러코드 값은 DWORD의 29bit 가 1로 설정된 영역에 할당해야 한다.
	참고로 30 ~ 31 bit는 에러치명도를 나타내며 2로 설정되면 경고를 나타낸다.
	세부사항은 http://www.codesarang.com/code/content_46.html 링크를 참조한다.
*/
enum eUserDefinedErrorCodes
{
	BASE_USER_ERROR_NUMBER	= 0x30000000,

	THREAD_LIB				= BASE_USER_ERROR_NUMBER + 1,
		THREAD_CREATE_FAILED										= THREAD_LIB + 0,			///< 쓰레드 생성 실패함.
		THREAD_ALREADY_STARTED										= THREAD_LIB + 1,			///< 쓰레드 이미 실행됨.
		THREAD_NOT_YET_RUNNING										= THREAD_LIB + 2,			///< 쓰레드 아직 실행 안함
	THREAD_LIB_MAX,

	TASKSCHEDULER_LIB		= THREAD_LIB_MAX,
		TASKSCHEDULER_DUPLICATE_PROCEDURE_ID						= TASKSCHEDULER_LIB + 0,	///< 프로시져 ID 중복으로 등록 실패함.
		TASKSCHEDULER_DUPLICATE_SCHEDULER_ID						= TASKSCHEDULER_LIB + 1,	///< 태스크 스캐줄러 ID 중복으로 태크스 스캐쥴러 생성 실패함.
		TASKSCHEDULER_NOT_FOUND_SCHEDULER_ID						= TASKSCHEDULER_LIB + 2,	///< 태크스 스캐줄러 ID 를 못찾음.
		TASKSCHEDULER_NOT_FOUND_PROCEDURE_ID						= TASKSCHEDULER_LIB + 3,	///< 프로시져 ID 를 못찾음.
	TASKSCHEDULER_LIB_MAX,

	PROACTOR_LIB			= TASKSCHEDULER_LIB_MAX,
		PROACTOR_REMOTE_NETLINK_ACCEPT_FAILED						= PROACTOR_LIB + 0,			///< 원격 NetLink를 accept 할 수 없음.
		PROACTOR_REMOTE_NETLINK_ACCEPT_SUCCESS_BUT_RECEIVE_FAILED	= PROACTOR_LIB + 1,			///< 원격 NetLink를 accept 했으나, 패킷 receive 할 수 없음.
		PROACTOR_CLOSED_LISTEN_NETLINK								= PROACTOR_LIB + 2,			///< listen NetLink가 종료됬음.
		PROACTOR_CLOSED_NETLINK										= PROACTOR_LIB + 3,			///< NetLink 종료됬음.
		PROACTOR_SEND_PACKET_FAILED									= PROACTOR_LIB + 4,			///< 패킷 전송 실패함.
		PROACTOR_RECEIVE_PACKET_FAILED								= PROACTOR_LIB + 5,			///< 패킷 수신 실패함.
		PROACTOR_CALL_ACCEPTEX_FAILED								= PROACTOR_LIB + 6,			///< NetLink의 AcceptEx 호출 실패.
		PROACTOR_CONNECTING_REMOTE_HOST_FAILED						= PROACTOR_LIB + 7,			///< 원격 서버에 연결할 수 없음.
		PROACTOR_CONNECT_SUCCESS_BUT_RECEIVE_FAILED					= PROACTOR_LIB + 8,			///< 연결은 성공했으나 패킷 receive 할 수 없음.
		PROACTOR_DUPLICATE_REMOTE_PROCEDURE_ID						= PROACTOR_LIB + 9,			///< 원격 프로시져 ID 중복으로 등록 실패함.
		PROACTOR_PACKET_PARSE_FAILED								= PROACTOR_LIB + 10,		///< 패킷 분석 실패함.
		PROACTOR_NOT_FOUND_REMOTE_PROCEDURE_ID						= PROACTOR_LIB + 11,		///< 원격 프로시져 ID 를 찾지 못함.
	PROACTOR_LIB_MAX,

	PROCESS_LIB			= PROACTOR_LIB_MAX,
		PROCESS_ALREADY_STARTED										= PROCESS_LIB + 0,			///< 프로세스 이미 실행됨.
		PROCESS_NOT_YET_RUNNING										= PROCESS_LIB + 1,			///< 프로세스 아직 실행 안됨.
		PROCESS_NOT_FOUND_PROCESS_ID								= PROCESS_LIB + 2,			///< 프로세스 ID 를 못찾음.
	PROCESS_LIB_MAX,

	DBENGINE_LIB		= PROCESS_LIB_MAX,
		DBPROVIDER_ALREADY_STARTED									= DBENGINE_LIB + 0,			///< 데이타베이스 프로바이더 이미 실행됨.
		DBPROVIDER_CONNECT_FAILED									= DBENGINE_LIB + 1,			///< 데이타베이스 프로바이더에서 데이타베이스 연결 실패
		DBPROVIDER_METHOD_IS_NOT_SUPPORTED							= DBENGINE_LIB + 2,			///< 지원되지 않는 메소드	
	DBENGINE_LIB_MAX,
};


#if !defined(is_user_error)
#define is_user_error(x)		BASE_USER_ERROR_NUMBER & x
#endif

/** 
	에러코드에 해당하는 에러 메세지를 얻어온다.
	@param iErrorCode 에러코드
	@verbatim
	DWORD dwError = GetLastError();
	if ( dwError != NOERROR )
	{
		printf( "error: %s\n", GetLastErrorMessage(dwError) );
	}
	@endverbatim
*/
extern const char* GetLastErrorMessage( DWORD iErrorCode );
extern void SetLastErrorMessage( char* pMsg );