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
//<
#include <map>
//<
#include <ProcessHandler.h>
#include <Sync.h>
#include "ProcessManager.h"

class ProcessManager
{
	typedef std::map<DWORD, ProcessPtr>	ProcessMap;
	ProcessMap		m_ProcessMap;		///< 프로세스 관리맵
	unsigned int	m_iTaskSchedulerID;	///< 프로세스 시작, 종료시 이벤트를 전달할 테스크스케줄러ID
	Sync			m_Sync;				///< 동기화

	ProcessManager( const ProcessManager& r ) {}

public:
	ProcessManager();
	virtual ~ProcessManager();

	ProcessPtr		Launch( const ProcessOption& Opt, ProcessHandler* pHandler, interface_ref* pUserData );
	bool			Terminate( DWORD iProcessId );
	ProcessPtr		GetProcess( DWORD iProcessId );
	ProcessPtr		Monitor( DWORD iProcessId, ProcessHandler* pHandler, interface_ref* pUserData );
	void			EventTick( void );
};