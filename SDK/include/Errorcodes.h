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
	RPC.NET ���� �߻��Ǵ� �����ڵ带 ������ ���

	MS�� �ǰ� ���׿� ���� ����� ���� �����ڵ� ���� DWORD�� 29bit �� 1�� ������ ������ �Ҵ��ؾ� �Ѵ�.
	����� 30 ~ 31 bit�� ����ġ���� ��Ÿ���� 2�� �����Ǹ� ��� ��Ÿ����.
	���λ����� http://www.codesarang.com/code/content_46.html ��ũ�� �����Ѵ�.
*/
enum eUserDefinedErrorCodes
{
	BASE_USER_ERROR_NUMBER	= 0x30000000,

	THREAD_LIB				= BASE_USER_ERROR_NUMBER + 1,
		THREAD_CREATE_FAILED										= THREAD_LIB + 0,			///< ������ ���� ������.
		THREAD_ALREADY_STARTED										= THREAD_LIB + 1,			///< ������ �̹� �����.
		THREAD_NOT_YET_RUNNING										= THREAD_LIB + 2,			///< ������ ���� ���� ����
	THREAD_LIB_MAX,

	TASKSCHEDULER_LIB		= THREAD_LIB_MAX,
		TASKSCHEDULER_DUPLICATE_PROCEDURE_ID						= TASKSCHEDULER_LIB + 0,	///< ���ν��� ID �ߺ����� ��� ������.
		TASKSCHEDULER_DUPLICATE_SCHEDULER_ID						= TASKSCHEDULER_LIB + 1,	///< �½�ũ ��ĳ�ٷ� ID �ߺ����� ��ũ�� ��ĳ�췯 ���� ������.
		TASKSCHEDULER_NOT_FOUND_SCHEDULER_ID						= TASKSCHEDULER_LIB + 2,	///< ��ũ�� ��ĳ�ٷ� ID �� ��ã��.
		TASKSCHEDULER_NOT_FOUND_PROCEDURE_ID						= TASKSCHEDULER_LIB + 3,	///< ���ν��� ID �� ��ã��.
	TASKSCHEDULER_LIB_MAX,

	PROACTOR_LIB			= TASKSCHEDULER_LIB_MAX,
		PROACTOR_REMOTE_NETLINK_ACCEPT_FAILED						= PROACTOR_LIB + 0,			///< ���� NetLink�� accept �� �� ����.
		PROACTOR_REMOTE_NETLINK_ACCEPT_SUCCESS_BUT_RECEIVE_FAILED	= PROACTOR_LIB + 1,			///< ���� NetLink�� accept ������, ��Ŷ receive �� �� ����.
		PROACTOR_CLOSED_LISTEN_NETLINK								= PROACTOR_LIB + 2,			///< listen NetLink�� �������.
		PROACTOR_CLOSED_NETLINK										= PROACTOR_LIB + 3,			///< NetLink �������.
		PROACTOR_SEND_PACKET_FAILED									= PROACTOR_LIB + 4,			///< ��Ŷ ���� ������.
		PROACTOR_RECEIVE_PACKET_FAILED								= PROACTOR_LIB + 5,			///< ��Ŷ ���� ������.
		PROACTOR_CALL_ACCEPTEX_FAILED								= PROACTOR_LIB + 6,			///< NetLink�� AcceptEx ȣ�� ����.
		PROACTOR_CONNECTING_REMOTE_HOST_FAILED						= PROACTOR_LIB + 7,			///< ���� ������ ������ �� ����.
		PROACTOR_CONNECT_SUCCESS_BUT_RECEIVE_FAILED					= PROACTOR_LIB + 8,			///< ������ ���������� ��Ŷ receive �� �� ����.
		PROACTOR_DUPLICATE_REMOTE_PROCEDURE_ID						= PROACTOR_LIB + 9,			///< ���� ���ν��� ID �ߺ����� ��� ������.
		PROACTOR_PACKET_PARSE_FAILED								= PROACTOR_LIB + 10,		///< ��Ŷ �м� ������.
		PROACTOR_NOT_FOUND_REMOTE_PROCEDURE_ID						= PROACTOR_LIB + 11,		///< ���� ���ν��� ID �� ã�� ����.
	PROACTOR_LIB_MAX,

	PROCESS_LIB			= PROACTOR_LIB_MAX,
		PROCESS_ALREADY_STARTED										= PROCESS_LIB + 0,			///< ���μ��� �̹� �����.
		PROCESS_NOT_YET_RUNNING										= PROCESS_LIB + 1,			///< ���μ��� ���� ���� �ȵ�.
		PROCESS_NOT_FOUND_PROCESS_ID								= PROCESS_LIB + 2,			///< ���μ��� ID �� ��ã��.
	PROCESS_LIB_MAX,

	DBENGINE_LIB		= PROCESS_LIB_MAX,
		DBPROVIDER_ALREADY_STARTED									= DBENGINE_LIB + 0,			///< ����Ÿ���̽� ���ι��̴� �̹� �����.
		DBPROVIDER_CONNECT_FAILED									= DBENGINE_LIB + 1,			///< ����Ÿ���̽� ���ι��̴����� ����Ÿ���̽� ���� ����
		DBPROVIDER_METHOD_IS_NOT_SUPPORTED							= DBENGINE_LIB + 2,			///< �������� �ʴ� �޼ҵ�	
	DBENGINE_LIB_MAX,
};


#if !defined(is_user_error)
#define is_user_error(x)		BASE_USER_ERROR_NUMBER & x
#endif

/** 
	�����ڵ忡 �ش��ϴ� ���� �޼����� ���´�.
	@param iErrorCode �����ڵ�
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