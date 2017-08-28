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
//
#include <Query.h>
#include <IDBFactory.h>
#include <DBProvider.h>
#include <QueryQueue.h>

QueryQueue::QueryQueue() : m_Seq(0)
{ }

QueryQueue::~QueryQueue()
{
	while ( !m_Que.empty() )
	{ 
		Query* pQry = m_Que.front();
		m_Que.pop_front();
		safe_delete(pQry);
	}
	m_Que.clear();
}

long32 QueryQueue::push_back(Query* pQry)
{
	Guard lock(m_Mutex);
	m_Que.push_back(pQry);
	return m_Seq;
}

Query* QueryQueue::pop(void)
{
	Guard lock(m_Mutex);
	if ( !m_Que.empty() )
	{
		Query* pQry = m_Que.front();
		m_Que.pop_front();
		return pQry;
	}
	return nullptr;
}

/// msec == 0 은 큐에 데이타가 없으면 즉시 리턴한다.
Query* QueryQueue::pop_front(int32 msec)
{
label_retry:
	Query* pQry = this->pop();
	if (nullptr == pQry && msec)
	{
		if ( m_Event.Wait(msec) == false )
		{
			goto label_retry;
		}
		else
		{	// 뭔가 시그널을 받았다.
			goto label_retry;
		}
	}
	return pQry;
}

bool QueryQueue::empty()
{ return m_Que.empty(); }

size_t QueryQueue::size()
{ return m_Que.size(); }

Sync& QueryQueue::getmutex()
{ return m_Mutex; }

Query::QType& QueryQueue::getque()
{ return m_Que;	}

void QueryQueue::signal()
{ m_Event.Wakeup();	}