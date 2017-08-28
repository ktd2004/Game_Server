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
#include <MacroFunc.h>
#include <TaskEventMsg.h>
#include <Errorcodes.h>
#include <Logger.h>
#include "ProcedureManager.h"


//<
ProcedureManager::ProcedureManager()
{
}

ProcedureManager::~ProcedureManager()
{
	foreach_do ( ProcedureMap, it, m_ProcedureMap )
	{ safe_delete(it->second.pProc); }
	m_ProcedureMap.clear();
}

bool ProcedureManager::Reg( unsigned int iTaskSchedulerID, TaskSchedulerImpl* pTSch, unsigned int iProcID, procedure_base* pProc )
{
	if ( m_ProcedureMap.count(iProcID) == 0 )
	{
		FuncType fn;
		fn.iTSID = iTaskSchedulerID;
		fn.pTSch = pTSch;
		fn.pProc = pProc;
		m_ProcedureMap.insert( ProcedureMap::value_type(iProcID,fn) );
		return true;
	}
	else
	{
		prn_err( "duplicated procedure id (%d)", iProcID );
		SetLastError( TASKSCHEDULER_DUPLICATE_PROCEDURE_ID );
		safe_delete(pProc);
		return false;
	}
}


class ProcedureCallMsg : public TaskEventMsg
{
public:
	interface_ref*	m_pRef;
	Buffer*			m_pBuf;
	procedure_base* m_pProc;
public:
	ProcedureCallMsg(interface_ref* pRef, procedure_base* pProc, Buffer* pBuf)
	{
		if ( pRef ) pRef->incref();
		m_pRef = pRef;
		m_pProc = pProc;
		m_pBuf = pBuf;
	}
	virtual ~ProcedureCallMsg()
	{
		if ( m_pRef && m_pRef->decref() == 0 )
			delete m_pRef;
		//safe_delete( m_pBuf );
		if ( m_pBuf ) m_pBuf->release();
	}
	void OnHandle( void )
	{
		m_pProc->stream( m_pBuf );
		m_pProc->call( m_pRef );
	}
};

bool ProcedureManager::Call( unsigned int iProcID, interface_ref* pRef, Buffer* pBuf )
{
	ProcedureMap::iterator it = m_ProcedureMap.find( iProcID );
	if ( it != m_ProcedureMap.end() )
	{
		ProcedureCallMsg* pMsg = new ProcedureCallMsg(pRef, it->second.pProc, pBuf);
		if (it->second.pTSch != nullptr)
		{
			return it->second.pTSch->Post(pMsg) > 0 ? true : false;
		}
		else
		{
			it->second.pTSch = (TaskSchedulerImpl*)TaskSchedulerFactory::Get(it->second.iTSID);
			if ( it->second.pTSch )
			{
				return it->second.pTSch->Post(pMsg) > 0 ? true : false;
			}
		}
	}
//	delete pBuf;
	pBuf->release();
	SetLastError( TASKSCHEDULER_NOT_FOUND_PROCEDURE_ID );
	return false;
}
