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
	@def �����޼��� ������ ũ��
*/
#define ERROR_MESSAGE_BUFFER_SIZE		4096
/** 
	@def P2P ��Ŷ ����ð��� ��� ���� Array �� ũ��
*/
#define MAX_LATENCY						10
/** 
	@def P2P reliable packet �� �������ϴ� �ּ� �ð�
*/
#define MIN_PACKET_RESEND_LATENCY		10
/** 
	@def P2P ��Ŷ�� �ѹ��� ������ �� �ִ� �ð�(msec)
*/
#define MAX_PACKET_RECEIVE_TM			50
/** 
	@def Ȧ��Ī ��Ŷ(Sync)�� �����ϴ� �ִ� Ƚ��
*/
#define MAX_SYNC_CNT					3
/** 
	@def ���� ���� ��Ŷ�� �������� ���ϸ� ������ ���� ó��
*/
#define MAX_KEEP_ALIVE					1000

// relay test
//#define TEST_REALY
// sync timeout test
//#define TEST_SYNC_TIMEOUT

/** 
	@def DBEngine ���ο��� ����ϴ� Query Queue�� fastlist�� ���
*/
#define USE_UTIL_LIST_AT_QUERY_QUEUE

/** 
	@def google�� �޸� �Ҵ��ڸ� ���
*/
#define _USE_TCMALLOC_

/**
	@def �������� ���Ͽ��� ����ϴ� �̹� ���ǵ� ���Ǹ�
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
	������μ����� ���¸� ������ ���
*/
enum eServerAppState
{
	eINIT,			///< �ʱ����
	eRUNNING,		///< �������
	eQUIT,			///< �������
};

/** 
	������μ����� ���� �ڵ带 ������ ���
*/
enum eServerAppExitCode
{
	eNOT_FOUND_INI_FILE = -3,	///< �������� ��ã��
	eABNORMAL_EXIT = -2,			///< ������ ����
	eARGUMENT_PARSE_ERROR = -1,	///< �ƱԸ�Ʈ �м� ����
	eNORMAL_EXIT = 0,			///< ��������
};
