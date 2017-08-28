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

#pragma warning(disable: 4267)
#pragma warning(disable: 4996)

/** 
	@def 에러메세지 버퍼의 크기
*/
#define ERROR_MESSAGE_BUFFER_SIZE		4096
/** 
	@def P2P 패킷 응답시간을 담아 놓는 Array 의 크기
*/
#define MAX_LATENCY						10
/** 
	@def P2P reliable packet 을 재전송하는 최소 시간
*/
#define MIN_PACKET_RESEND_LATENCY		10
/** 
	@def P2P 패킷을 한번에 수신할 수 있는 시간(msec)
*/
#define MAX_PACKET_RECEIVE_TM			50
/** 
	@def 홀펀칭 패킷(Sync)을 전송하는 최대 횟수
*/
#define MAX_SYNC_CNT					3
/** 
	@def 연결 유지 패킷을 수신하지 못하면 세션을 종료 처리
*/
#define MAX_KEEP_ALIVE					1000

// relay test
//#define TEST_REALY
// sync timeout test
//#define TEST_SYNC_TIMEOUT

/** 
	@def DBEngine 내부에서 사용하는 Query Queue를 fastlist로 사용
*/
#define USE_UTIL_LIST_AT_QUERY_QUEUE

/** 
	@def google의 메모리 할당자를 사용
*/
#define _USE_TCMALLOC_

/**
	@def 서버구성 파일에서 사용하는 이미 정의된 섹션명
*/
#define			SAGENT		"SAgent"
#define			PAGENT		"PAgent"
#define			LOGS		"LogS"
#define			GTWS		"GtwS"
#define			MSTS		"MstS"
#define			CHNS		"ChnS"
#define			DBGW		"Dbgw"
#define			DB_SECTION	"DB_"

/** 
	사버프로세스의 상태를 정의한 상수
*/
enum eServerAppState
{
	eINIT,			///< 초기상태
	eRUNNING,		///< 실행상태
	eQUIT,			///< 종료상태
};

/** 
	사버프로세스의 종료 코드를 정의한 상수
*/
enum eServerAppExitCode
{
	eNOT_FOUND_INI_FILE = -3,	///< 구성파일 못찾음
	eABNORMAL_EXIT = -2,			///< 비정상 종료
	eARGUMENT_PARSE_ERROR = -1,	///< 아규먼트 분석 실패
	eNORMAL_EXIT = 0,			///< 정상종료
};
