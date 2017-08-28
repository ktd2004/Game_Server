#pragma once

//
#include <Ptr.h>
#include <Query.h>
#include <Sync.h>
#include <Signal.h>

class Query;
class QueryQueue
{
private:
	Query::QType		m_Que;
	Sync				m_Mutex;
	volatile long32		m_Seq;
	Signal				m_Event;
public:
	QueryQueue();
	virtual ~QueryQueue();
	long				push_back( Query* pQry );
	Query*				pop(void);
	Query*				pop_front(int32 msec);
	bool				empty();
	size_t				size();
	Sync&				getmutex();
	Query::QType&		getque();
	void				signal();
};
