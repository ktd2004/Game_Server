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
//<
#include <Errorcodes.h>
#include <TaskSchedulerFactory.h>
#include "ProcessImpl.h"
#include "ProcessManager.h"

ProcessManager::ProcessManager()
{
}

ProcessManager::~ProcessManager()
{

}

ProcessPtr ProcessManager::Launch(const ProcessOption& Opt, ProcessHandler* pHandler, interface_ref* pUserData)
{
	ProcessImpl* pProcess = new ProcessImpl;
	if ( pProcess )
	{
		pProcess->UserData( pUserData );
		if ( pProcess->Spawn(Opt, pHandler) )
		{
			Guard lock(m_Sync);
			m_ProcessMap.insert( ProcessMap::value_type(pProcess->GetProcessId(), pProcess) );
			if ( pHandler )
			{
				pHandler->OnLaunchProcess( pProcess );
			}
			return pProcess;
		}
	}
	safe_delete(pProcess);
	return nullptr;
}

bool ProcessManager::Terminate( DWORD iProcessId )
{
	Guard lock(m_Sync);
	ProcessMap::iterator it = m_ProcessMap.find( iProcessId );
	if ( it != m_ProcessMap.end() )
	{
		ProcessImpl* pProcess = (ProcessImpl*)it->second.get();
		pProcess->Terminate();
		if ( pProcess->m_pHandler )
		{
			pProcess->m_pHandler->OnExitProcess( pProcess );
		}
		m_ProcessMap.erase( it );
		return true;
	}
	
	SetLastError( PROCESS_NOT_FOUND_PROCESS_ID );
	return false;
}

ProcessPtr ProcessManager::GetProcess( DWORD iProcessId )
{
	Guard lock(m_Sync);
	found_map_ret( ProcessMap, it, m_ProcessMap, iProcessId, it->second );
	SetLastError( PROCESS_NOT_FOUND_PROCESS_ID );
	return nullptr;
}

ProcessPtr ProcessManager::Monitor(DWORD iProcessId, ProcessHandler* pHandler, interface_ref* pUserData)
{
	Guard lock(m_Sync);
	ProcessMap::iterator it = m_ProcessMap.find( iProcessId );
	if ( it != m_ProcessMap.end() )
	{
		ProcessImpl* pProcess = (ProcessImpl*)it->second.get();
		if ( pProcess->m_pHandler != pHandler )
		{
			pProcess->m_pHandler = pHandler;
		}
		if ( pProcess->UserData() != pUserData )
		{
			pProcess->UserData( pUserData );
		}
		return it->second;
	}

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, iProcessId);
	if (hProcess != INVALID_HANDLE_VALUE && hProcess != 0)
	{
		ProcessImpl* pProcess = new ProcessImpl;
		pProcess->m_pHandler = pHandler;
		pProcess->SetHandle(hProcess);
		pProcess->SetProcessId(iProcessId);
		pProcess->UserData(pUserData);
		m_ProcessMap.insert( ProcessMap::value_type(pProcess->GetProcessId(), pProcess) );
		return (Process*)pProcess;
	}

	return nullptr;
}

void ProcessManager::EventTick( void )
{
	std::list<ProcessPtr> spawn;
	ProcessImpl* pProcess = nullptr;
	Guard lock(m_Sync);
	for( ProcessMap::iterator it = m_ProcessMap.begin(); it != m_ProcessMap.end(); )
	{
		pProcess = (ProcessImpl*)it->second.get(); 
		if ( !pProcess->IsRunning() )
		{
			if ( pProcess->m_pHandler )
			{
				pProcess->m_pHandler->OnExitProcess( pProcess );
			}

			if ( pProcess->m_bRestart && ++(pProcess->m_iRestartCnt) <= pProcess->m_Opt.MaxRestartCnt() )
			{
				spawn.push_back( pProcess );
			}

			it = m_ProcessMap.erase( it );
			continue;
		}

		++it;
	}

	for ( std::list<ProcessPtr>::iterator it = spawn.begin(); it != spawn.end(); ++it )
	{
		pProcess = (ProcessImpl*)it->get();
		if ( pProcess->Spawn(pProcess->m_Opt, pProcess->m_pHandler) )
		{
			m_ProcessMap.insert( ProcessMap::value_type(pProcess->GetProcessId(),pProcess) );
			if ( pProcess->m_pHandler )
			{
				pProcess->m_pHandler->OnLaunchProcess( pProcess );
			}
		}
	}
	spawn.clear();
}