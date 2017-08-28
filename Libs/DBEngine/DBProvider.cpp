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

#include <DataTypes.h>
#include <Mmsystem.h>
#include <Query.h>
#include <IDBFactory.h>
#include <DBProvider.h>
#include <TaskSchedulerFactory.h>

DBProvider::DBProvider()
: m_iTaskID(-1)
, m_pTaskScheduler(nullptr)
{
}

DBProvider::~DBProvider()
{
	m_QueryQue.clear();
	m_ResultQue.clear();
	m_EngineQue.clear();
}

int32 DBProvider::Startup( const IDBProvider::Properties& Prop )
{
	if ( m_EngineQue.size() > 0 )
	{
		SetLastError( DBPROVIDER_ALREADY_STARTED );
		return 0;
	}

	for ( int32 i = 0; i < (int32)Prop.stDatabase.iConnectionCount; i++ )
	{
		m_QueryQue.push_back (new QueryQueue);
		m_ResultQue.push_back(new QueryQueue);
		m_EngineQue.push_back(IDBEngineFactory::CreateMySQL());

		if ( m_EngineQue[i]->Connect(Prop) != 0 )
		{
			SetLastError( DBPROVIDER_CONNECT_FAILED );
			SetLastErrorMessage( (char*)m_EngineQue[i]->ErrorMsg() );
			return 0;
		}

		Thread* s = CREATE_THREAD_V1( this, &DBProvider::Running, i,	0 );
		if ( !s )
		{
			return 0;
		}

		m_SpawnList.push_back( s );
	}

	m_pTaskScheduler = TaskSchedulerFactory::Get( Prop.iNotifyTaskSchedulerID );
	m_iTaskID =	CREATE_TASK( Prop.iNotifyTaskSchedulerID, this, &DBProvider::EventTick, Prop.iTaskDuration );

	foreach_do(ThreadSpawnList, it, m_SpawnList)
	{
		(*it)->Resume();
	}

	return 1;
}

void DBProvider::Shutdown( void )
{
	for ( int32 i = 0; i < (int32)m_QueryQue.size(); i++ )
	{ 
		m_QueryQue[i]->push_back((Query*)new QuitMsg(eDBPROVIDER_SHUTDOWN));
	}

	foreach_do(ThreadSpawnList, it, m_SpawnList)
	{ 
		(*it)->Wait();
	}
}

size_t DBProvider::Size( void )
{  
	size_t iSum = 0;
	size_t iQue = m_QueryQue.size();
	for ( size_t i = 0; i < iQue; i++ )
	{
		iSum += m_QueryQue[i]->size();
	}
	return iSum;
}


size_t DBProvider::LoadBalance( void )
{
	size_t iMin = 0;
	size_t iInx = 0;
	const size_t iSize = m_QueryQue.size();

	for ( size_t i = 0; i < iSize; i++ )
	{
		if ( m_QueryQue[i]->size() == 0 )
		{
			iInx = i;
			break;
		}
		else if ( iMin > m_QueryQue[i]->size()  )
		{
			iMin = m_QueryQue[i]->size();
			iInx = i;
		}
	}

	return iInx;
}


int32 DBProvider::Push( Query* pQry )
{ 
	QUID iQid = pQry->GetQid();
	QUID iInx = 0;
	
	if ( iQid == 0 )
	{
		iInx = LoadBalance();
	}
	else
	{
		iInx = iQid % (QUID)m_QueryQue.size();
	}
	
	m_QueryQue[iInx]->push_back(pQry);
	m_QueryQue[iInx]->signal();
	return 1;
}

int32 DBProvider::Push( Query::QType& QryList )
{
	int32 iQueueSize = (int32)m_QueryQue.size();
	while ( !QryList.empty() )
	{
		Query* pQry = QryList.front();
		QryList.pop_front();
		
		QUID iQid = pQry->GetQid();
		QUID iInx = 0;

		if ( iQid == 0 )
		{
			iInx = LoadBalance();
		}
		else
		{
			iInx = iQid % iQueueSize;
		}
		
		m_QueryQue[iInx]->push_back(pQry);
	}

	for ( int32 i = 0; i < iQueueSize; i++ )
	{ 
		if ( !m_QueryQue[i]->empty() )
		{
			m_QueryQue[i]->signal();
		}
	}

	return 1;
}

int32 DBProvider::Push( Query* pQry, uint32 iMSec )
{
	safe_delete(pQry);
	return -1;
}

/// 큐가 여러개 있을 수 있기 때문에 어떤 큐에서 Pop 해야 되는지 선택하기 힘들다.
/// 그래서 iQid 를 받도록 구현되어 있다.
Query* DBProvider::Pop( QUID iQid, uint32 iMSec )
{ 
	iQid = iQid % (QUID)m_ResultQue.size();
	return m_ResultQue[iQid]->pop_front(iMSec);
}

bool DBProvider::Pop( Query::QType& QryList )
{
	int32 iQueueSize = (int32)m_ResultQue.size();
	for ( int i = 0; i < iQueueSize; i++ )
	{
		if ( !m_ResultQue[i]->empty() )
		{
			Guard lock(m_ResultQue[i]->getmutex());
			QryList.merge( m_ResultQue[i]->getque() );
		}
	}
	return !QryList.empty();
}

//////////////////////////////////////////////////////////////////////////

void DBProvider::Running(int32 iInx)
{
	IDBEngine&  Db        = *m_EngineQue[iInx];
	QueryQueue& QueryQue  = *m_QueryQue[iInx];
	QueryQueue& ResultQue = *m_ResultQue[iInx];

	int32 iRet = 0;
	while ( 1 )
	{
		Query* pQry = QueryQue.pop_front( eDBPROVIDER_WAIT_ONE_SECOND );
		if (pQry == nullptr)
		{
			continue;
		}

		if ( pQry->GetQid() == eDBPROVIDER_SHUTDOWN )
		{
			//prn_inf( "database provider is terminated, index = %d", iInx );
			delete pQry;
			break;
		}

		uint64 iTick = timeGetTime();
		iRet = Db.Execute( pQry );
		iTick = timeGetTime() - iTick;

		ResultQue.push_back( pQry );
		if ( m_pTaskScheduler )
			m_pTaskScheduler->Signal( m_iTaskID );
	}
}

void DBProvider::EventTick( void )
{
	Query::QType rList;
	if ( this->Pop(rList) )
	{
		while (!rList.empty())
		{
			spRef<Query> spQry(rList.front());
			rList.pop_front();
			spQry->OnResult();
		}
	}
}

//////////////////////////////////////////////////////////////////////////

IDBProvider* IDBProviderFactory::CreateDirect( void )
{
	return new DBProvider;
}