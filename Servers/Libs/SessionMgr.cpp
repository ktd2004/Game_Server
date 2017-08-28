#include "stdafx.h"
#include "Session.h"
#include "AuthKey.h"
#include <MacroFunc.h>
#include "SessionMgr.h"
#include <TaskSchedulerFactory.h>
#include <Logger.h>

SessionManager::SessionManager()
{
	m_SessionMap.clear();
	m_AuthMap.clear();
	m_KickList.clear();
}

SessionManager::SessionManager( const SessionManager& r )
{
	m_SessionMap.clear();
	m_AuthMap.clear();
	m_KickList.clear();
}

SessionManager::~SessionManager()
{
	m_SessionMap.clear();
	m_AuthMap.clear();
	m_KickList.clear();
}

SessionManager* SessionManager::instance()
{
	static SessionManager sInstance;
	return &sInstance;
}

SessionPtr SessionManager::Reg( UID iUID, NetLinkPtr spLink, uint32 iAuthKey )
{
	found_map_ret( SessionMap, it, m_SessionMap, iUID, it->second );

	SessionPtr spSession = new Session( iUID );
	spSession->SetState( Session::eAUTHENTICATE );
	spSession->SetLink( spLink );
	iAuthKey == 0 ? spSession->SetAuthKey( AuthKey::Create(iUID) ) : spSession->SetAuthKey( iAuthKey );

	m_SessionMap.insert( SessionMap::value_type(iUID, spSession) );
	return spSession;
}

SessionPtr SessionManager::Reg( SessionPtr spSession )
{
	isnull_ret(spSession, nullptr);
	found_map_ret( SessionMap, it, m_SessionMap, spSession->GetUID(), it->second );
	m_SessionMap.insert( SessionMap::value_type(spSession->GetUID(), spSession) );
	return spSession;
}

SessionPtr SessionManager::Reg(const Session& r, NetLinkPtr spLink)
{
	SessionPtr spSession;
	auto it = m_SessionMap.find(r.m_iUID);
	if (it == m_SessionMap.end())
	{
		spSession = new Session(r);
	}

	spSession->SetLink(spLink);

	m_SessionMap.insert(SessionMap::value_type(r.m_iUID, spSession));
	return spSession;
}

bool SessionManager::Unreg( UID iUID )
{
	notfound_map_ret( SessionMap, it, m_SessionMap, iUID, false );
	m_SessionMap.erase( it );
	return true;
}

SessionPtr SessionManager::Get( UID iUID )
{
	notfound_map_ret(SessionMap, it, m_SessionMap, iUID, nullptr);
	return it->second;
}

void SessionManager::SetLink( UID iUID, NetLinkPtr spLink )
{
	SessionMap::iterator it = m_SessionMap.find( iUID );
	if( it == m_SessionMap.end() || !it->second ) { return; }
	it->second->SetLink( spLink );
}

int32 SessionManager::GetCount()
{
	return m_SessionMap.size();
}

int32 SessionManager::GetCount3A()
{
	return m_AuthMap.size();
}

SessionPtr SessionManager::Reg3A( UID iUID, NetLinkPtr spLink, uint32 iAuthKey )
{
	found_map_ret(SessionMap, it, m_AuthMap, iUID, it->second);

	SessionPtr spSession = new Session;
	spSession->SetState( Session::eAUTHENTICATE );
	spSession->m_iUID = iUID;
	spSession->m_spLink = spLink;
	iAuthKey == 0 ? spSession->SetAuthKey( AuthKey::Create(iUID) ) : spSession->SetAuthKey( iAuthKey );

	m_AuthMap.insert( SessionMap::value_type(iUID, spSession) );
	return spSession;
}

SessionPtr SessionManager::Reg3A( SessionPtr spSession )
{
	isnull_ret(spSession, nullptr);
	found_map_ret(SessionMap, it, m_AuthMap, spSession->GetUID(), it->second);
	m_AuthMap.insert(SessionMap::value_type(spSession->GetUID(), spSession));
	return spSession;
}

bool SessionManager::Unreg3A( UID iUID )
{
	notfound_map_ret(SessionMap, it, m_AuthMap, iUID, false);
	m_AuthMap.erase(it);
	return true;
}


bool SessionManager::CompareAuthKeyUnregAll(UID iUID, uint32 iAuthKey, int32 iReason, const char* pFunc)
{
	auto it = m_SessionMap.find(iUID);
	if (it != m_SessionMap.end())
	{
		if (it->second->m_iAuthKey == iAuthKey)
		{
			NetLinkPtr spLink = it->second->GetLink();
			if (spLink)
			{
				spLink->UserData(nullptr);
				Kick(spLink, iReason, pFunc);
			}
			m_SessionMap.erase(it);
			return true;
		}
	}

	it = m_AuthMap.find(iUID);
	if (it != m_AuthMap.end())
	{
		if (it->second->m_iAuthKey == iAuthKey)
		{
			NetLinkPtr spLink = it->second->GetLink();
			if (spLink)
			{
				spLink->UserData(nullptr);
				Kick(spLink, iReason, pFunc);
			}
			m_AuthMap.erase(it);
			return true;
		}
	}

	return false;
}

SessionPtr SessionManager::Get3A( UID iUID )
{
	notfound_map_ret(SessionMap, it, m_AuthMap, iUID, nullptr);
	return it->second;
}

SessionPtr SessionManager::Authentication( UID iUID )
{
	notfound_map_ret(SessionMap, itA, m_AuthMap, iUID, nullptr);

	SessionPtr spSession = itA->second;
	m_AuthMap.erase(itA);

	SessionMap::iterator itS = m_SessionMap.find( iUID );
	if( itS != m_SessionMap.end() )
	{
		prn_err( "uid(%I64u) is present in both session and auth map", iUID );
		m_SessionMap.erase( itS );
		return nullptr;
	}

	spSession->SetState(Session::eVERIFICATION);
	m_SessionMap.insert( SessionMap::value_type(iUID, spSession) );
	return spSession;
}

SessionPtr SessionManager::Unauthentication( UID iUID )
{
	notfound_map_ret(SessionMap, itS, m_SessionMap, iUID, nullptr);

	SessionPtr spSession = itS->second;
	m_SessionMap.erase( itS );

	SessionMap::iterator itD = m_AuthMap.find( iUID );
	if (itD != m_AuthMap.end())
	{
		prn_err( "uid(%I64u) is present in both session and auth map", iUID );
		m_AuthMap.erase(itD);
		return nullptr;
	}

	spSession->SetState( Session::eAUTHENTICATE );
	m_AuthMap.insert(SessionMap::value_type(iUID, spSession));
	return spSession;
}

bool SessionManager::UnregAll( UID iUID )
{
	bool b1 = Unreg(iUID);
	bool b2 = Unreg3A(iUID);
	return (b1 || b2);
}

SessionManager::SessionMap& SessionManager::GetSessionMap()
{
	return m_SessionMap;
}

SessionManager::SessionMap& SessionManager::GetAuthMap()
{
	return m_AuthMap;
}

SessionPtr SessionManager::IsExists( UID iUID )
{
	SessionMap::iterator it = m_SessionMap.find( iUID );
	if( it != m_SessionMap.end() )
	{
		return it->second;
	}

	it = m_AuthMap.find( iUID );
	if (it != m_AuthMap.end())
	{
		return it->second;
	}

	return nullptr;
}

void SessionManager::Kick( NetLinkPtr spLink, int32 iReason, const std::string& sFunc )
{
	time_t tTime = util::time2msec() + 1000;	// 1초 후에 끊기도록 설정
	m_KickList.push_back( KickReasonInfo(tTime, spLink, iReason, sFunc) );
}

void SessionManager::Kick(UID iUID, int32 iReason, const std::string& sFunc)
{
	auto it = m_SessionMap.find(iUID);
	if (it != m_SessionMap.end())
	{
		SessionPtr spSession = it->second;
		m_SessionMap.erase(it);
		Kick(spSession->GetLink(), iReason, sFunc);
	}
}

bool SessionManager::OnKickTimer( void* pInst )
{
	time_t t = util::time2msec();
	for( KickList::iterator it = m_KickList.begin(); it != m_KickList.end(); )
	{
		// 정렬 되어 있으므로 break 하면 된다.
		KickReasonInfo& s = *it;
		if( s.m_tTime > t )
		{
			break;
		}

		if( s.m_spLink )
		{
			s.m_spLink->Kick( s.m_iReason, cstr(s.m_sFunc) );
		}

		it = m_KickList.erase( it );
	}

	return true;
}

size_t SessionManager::Broadcast( Buffer* pBuffer )
{
	if( m_SessionMap.size() == 0 )
	{
		pBuffer->release();
		return 0;
	}

	Buffer* pClone = nullptr;
	SessionMap::iterator itCheck;
	foreach_do( SessionMap, it, m_SessionMap )
	{
		if( it->second->GetLink() )
		{
			pClone = new Buffer( pBuffer->capacity() );
			pClone->copy( pBuffer->rd_ptr(), pBuffer->length() );
			it->second->GetLink()->Send( pClone );
		}
	}
	pBuffer->release();
	return m_SessionMap.size();
}

void SessionManager::StartKickTimer( int32 iTaskSchedulerID, uint32 iMsec )
{
	m_iTaskSchedulerID = iTaskSchedulerID;
	CREATE_TIMER(m_iTaskSchedulerID, this, &SessionManager::OnKickTimer, iMsec, nullptr);
}

