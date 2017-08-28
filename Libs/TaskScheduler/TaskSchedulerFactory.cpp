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
#include <DataTypes.h>
#include <TaskSchedulerFactory.h>
#include <Errorcodes.h>
#include <ThreadFactory.h>
//<
#include "TaskSchedulerImpl.h"
#include "ProcedureManager.h"

//<
namespace TaskSchedulerFactory
{
	typedef std::map<unsigned int, TaskSchedulerImpl* > TaskSchedulreMap;
	TaskSchedulreMap	g_TaskSchedulerMap;
	ProcedureManager	g_ProcedureMgr;

	TaskScheduler* Create( unsigned int iTaskSchedulerID, unsigned long iDuration, unsigned short iBindCPU )
	{
		if ( !TaskSchedulerFactory::Get(iTaskSchedulerID) )
		{
			TaskSchedulerImpl* pTSch = new TaskSchedulerImpl;
			pTSch->SetID( iTaskSchedulerID );
			pTSch->SetDuration( iDuration );
			pTSch->Start( iBindCPU );
			g_TaskSchedulerMap.insert( TaskSchedulreMap::value_type(iTaskSchedulerID, pTSch) );
			SetLastError( NOERROR );
			return (TaskScheduler*)pTSch;
		}
		SetLastError( TASKSCHEDULER_DUPLICATE_SCHEDULER_ID );
		return nullptr;
	}

	TaskScheduler* Get( unsigned int iTaskSchedulerID )
	{
		TaskSchedulreMap::iterator it = g_TaskSchedulerMap.find( iTaskSchedulerID );
		if ( it != g_TaskSchedulerMap.end() )
		{
			return it->second;
		}
		SetLastError( TASKSCHEDULER_NOT_FOUND_SCHEDULER_ID );		
		return nullptr;
	}

	bool Start( unsigned int iTaskSchedulerID )
	{
		TaskSchedulreMap::iterator it;
		if ( iTaskSchedulerID != 0 )
		{
			it = g_TaskSchedulerMap.find( iTaskSchedulerID );
			if ( it != g_TaskSchedulerMap.end() )
			{
				it->second->Resume();
				return true;
			}
			SetLastError( TASKSCHEDULER_NOT_FOUND_SCHEDULER_ID );
			return false;
		}
		else
		{
			for ( it = g_TaskSchedulerMap.begin(); it != g_TaskSchedulerMap.end(); ++it )
			{
				it->second->Resume();
			}

			return true;
		}
	}

	bool Stop( unsigned int iTaskSchedulerID )
	{
		TaskSchedulreMap::iterator it;
		if ( iTaskSchedulerID != 0 )
		{
			it = g_TaskSchedulerMap.find( iTaskSchedulerID );
			if ( it != g_TaskSchedulerMap.end() )
			{
 				it->second->Terminate();
 				it->second->Wait();
				return true;
			}
			SetLastError( TASKSCHEDULER_NOT_FOUND_SCHEDULER_ID );
			return true;
		}
		else
		{
			for ( it = g_TaskSchedulerMap.begin(); it != g_TaskSchedulerMap.end(); ++it )
			{
				it->second->Terminate();
				it->second->Wait();
			}
		}

		return true;
	}

	bool RegProcedure( unsigned int iTaskSchedulerID, unsigned int iProcID, procedure_base* pProc)
	{
		TaskSchedulerImpl* pTSch = (TaskSchedulerImpl*)Get( iTaskSchedulerID );
		return g_ProcedureMgr.Reg( iTaskSchedulerID, pTSch, iProcID, pProc );
	}

	bool CallProcedure( unsigned int iProcID, interface_ref* pRef, Buffer* pBuf )
	{
		return g_ProcedureMgr.Call( iProcID, pRef, pBuf );
	}

	void CancelTimer(unsigned int iTaskSchedulerID, TimerHandle& spTimer)
	{
		TaskScheduler* pTSch = TaskSchedulerFactory::Get(iTaskSchedulerID);
		if ( pTSch )
		{
			return pTSch->CancelTimer( spTimer );
		}
	}
};