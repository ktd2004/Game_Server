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

#include <map>
#include <Procedure.h>
#include "TaskSchedulerImpl.h"

/** 
	비동기 프로시져 관리자 클래스
*/
class ProcedureManager
{
	struct FuncType
	{
		unsigned int		iTSID;		///< 비동기 프로시져를 실행할 테스크스케줄러 고유ID
		TaskSchedulerImpl*	pTSch;		///< 비동기 프로시져를 실행할 테스크스케줄러 포인터
		procedure_base*		pProc;		///< 비동기 프로시져
	};
	typedef std::map<unsigned int, FuncType > ProcedureMap;
	ProcedureMap		m_ProcedureMap;	///< 비동기 프로시져 컨테이너

public:
	ProcedureManager();
	virtual ~ProcedureManager();

public:
	/** 
		비동기 프로시져를 등록한다.
		@param iTaskSchedulerID 비동기프로시져를 실행할 테스크스케줄러 ID
		@param pTSch 비동기 프로시져를 실행할 테스크스케줄러 포인터
		@param iProcID 비동기 프로시져의 고유아이디
		@param pProc 비동기 프로시져의 주소값
	*/
	bool Reg(unsigned int iTaskSchedulerID, TaskSchedulerImpl* pTSch, unsigned int iProcID, procedure_base* pProc);
	/** 
		비동기 프로시져를 실행한다.
		@param iProcID 비동기 프로시져의 고유아이디
		@param pRef 비동기 프로시져를 호출한 호출자
		@param pBuf 비동기 프로시져에 전달될 매개변수를 담고있는 버퍼
	*/
	bool Call(unsigned iProcID, interface_ref* pRef, Buffer* pBuf);
};