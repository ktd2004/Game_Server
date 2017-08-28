#include "stdafx.h"
#include "WholeChat.h"


WholeChat::~WholeChat()
{
	m_SessionList.clear();
}

WholeChat* WholeChat::Instance()
{
	static WholeChat sInstance;
	return &sInstance;
}

bool WholeChat::Add( PlayerSession* pSession )
{
	foreach_do( SessionList, it, m_SessionList )
	{
		if( (*it) && (*it)->GetUID() == pSession->GetUID() )
		{
			return false;
		}
	}

	m_SessionList.push_back( pSession );
	return true;
}

bool WholeChat::Remove( PlayerSession* pSession )
{
	foreach_do( SessionList, it, m_SessionList )
	{
		if( (*it) && (*it)->GetUID() == pSession->GetUID() )
		{
			m_SessionList.erase( it );
			return true;
		}
	}

	return false;
}
