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
#include <stdio.h>
#include <DataTypes.h>
#include <Errorcodes.h>
#include <MacroFunc.h>

//<
__declspec(thread) char g_szErrMsgBuf[ERROR_MESSAGE_BUFFER_SIZE];
__declspec(thread) char g_szErrUserMsgBuf[ERROR_MESSAGE_BUFFER_SIZE];

//<
namespace
{
	static char* g_pText[] = {
		"MEMPOOLER_LIB",
		"create thread failed",	// THREAD_CREATE_FAILED
		"thread is already started", // THREAD_ALREADY_STARTED
		"thread not yet running", // THREAD_NOT_YET_RUNNING

		"can't register procedure, because proceduer id is duplicated",	 // TASKSCHEDULER_DUPLICATE_PROCEDURE_ID
		"can't create taskscheduler, because taskscheduler id is duplicated", // TASKSCHEDULER_DUPLICATE_SCHEDULER_ID
		"taskscheduler is not found", // TASKSCHEDULER_NOT_FOUND_SCHEDULER_ID
		"procedure is not found", // TASKSCHEDULER_NOT_FOUND_PROCEDURE_ID
		"remote netlink accept failed", // PROACTOR_REMOTE_NETLINK_ACCEPT_FAILED
		"remote netlink accept success, but packet received failed", // PROACTOR_REMOTE_NETLINK_ACCEPT_SUCCESS_BUT_RECEIVE_FAILED
		"closed listen nelink", // PROACTOR_CLOSED_LISTEN_NETLINK
		"closed netlink", // PROACTOR_CLOSED_NETLINK
		"send packet failed, maybe netlink is closed", // PROACTOR_SEND_PACKET_FAILED
		"receive packet failed, maybe netlink is closed", // PROACTOR_RECEIVE_PACKET_FAILED
		"call acceptex failed",	// PROACTOR_CALL_ACCEPTEX_FAILED
		"connecting remote host failed, check network address(ip,port) or network status", // PROACTOR_CONNECTING_REMOTE_HOST_FAILED
		"connect success, but receive packet failed", // PROACTOR_CONNECT_SUCCESS_BUT_RECEIVE_FAILED
		"can't register remote proceduere id, because remote procedure id is duplicated", // PROACTOR_DUPLICATE_REMOTE_PROCEDURE_ID
		"received packet parsing failed, because invaild packet format", // PROACTOR_PACKET_PARSE_FAILED
		"not found remote procedure id", // PROACTOR_NOT_FOUND_REMOTE_PROCEDURE_ID
		"process is already started", // PROCESS_ALREADY_STARTED
		"process not yet running", // PROCESS_NOT_YET_RUNNING
		"process id is not found", // PROCESS_NOT_FOUND_PROCESS_ID
		"dbprovider is already started", //DBPROVIDER_ALREADY_STARTED
		NULL,
		"this method is not supported",
		"unknown error code",
	};
};

extern const char* GetLastErrorMessage( DWORD iErrorCode )
{
	g_szErrMsgBuf[0] = 0;

	if ( is_user_error(iErrorCode) )
	{
		unsigned int i = 0x0FFFFFFF & iErrorCode;
		if ( sizeof(g_pText)/sizeof(char*) <= i )
		{
			return g_pText[sizeof(g_pText)/sizeof(char*) -1];
		}

		sprintf_s( g_szErrMsgBuf, sizeof(g_szErrMsgBuf), "error %d, %s", iErrorCode, g_pText[i] ? g_pText[i] : g_szErrUserMsgBuf  );
	}
	else
	{
		HLOCAL hlocal = NULL; // 에러 메시지를 얻기 위한 버퍼 
		BOOL fOk = FormatMessage( 
			FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER, // 에러코드에 대한 문자열을 원하고 | 에러 텍스트 문자열을 메모리 블록에 할당받아 반환할것(LocalFree()로 해지할 것을 약속) 
			NULL,  
			iErrorCode, // 에러코드 
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // 사용할 언어 
			(LPTSTR)&hlocal, // 반환되는 문자열 
			0,  
			NULL); 
		if(hlocal != NULL) 
		{ 
			sprintf_s( g_szErrMsgBuf, sizeof(g_szErrMsgBuf), "%d, %s", iErrorCode, (char*)hlocal);
			// FormatMessage 처리 
			LocalFree(hlocal); // 반드시 해제시켜줄것 
		}
	}

	return g_szErrMsgBuf;
}

void SetLastErrorMessage( char* pMsg )
{
	strcpy_s( g_szErrUserMsgBuf, sizeof(g_szErrUserMsgBuf), pMsg );
}
