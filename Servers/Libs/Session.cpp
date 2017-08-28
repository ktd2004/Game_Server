#include "stdafx.h"
#include "Session.h"
#include "AAAConst.h"


Session::Session()
	: m_iUID(0)
	, m_iAuthKey(0)
	, m_tAuthTime(0)
	, m_iGID(0)
	, m_iMasterID(0)
	, m_iChannelID(0)
	, m_iState(eAUTHENTICATE)
	, m_tLastTick(0)
	, m_tLastSave(0)
	, m_pUserData(nullptr)
{
}

Session::Session( UID iUID )
	: m_iUID(iUID)
	, m_iAuthKey(0)
	, m_tAuthTime(0)
	, m_iGID(0)
	, m_iMasterID(0)
	, m_iChannelID(0)
	, m_iState(eAUTHENTICATE)
	, m_tLastTick(0)
	, m_tLastSave(0)
	, m_pUserData(nullptr)
{
}

Session::Session(const Session& r)
	: m_iUID(r.m_iUID)
	, m_iAuthKey(r.m_iAuthKey)
	, m_tAuthTime(r.m_tAuthTime)
	, m_iGID(r.m_iGID)
	, m_iChannelID(r.m_iChannelID)
	, m_iState(r.m_iState)
	, m_tLastTick(r.m_tLastTick)
	, m_tLastSave(r.m_tLastSave)
	, m_spLink(r.m_spLink)
	, m_sAccount(r.m_sAccount)
	, m_pUserData(nullptr)
{
	UserData(r.m_pUserData);
}

Session::Session( const SessionPtr& r )
	: m_iUID(r->m_iUID)
	, m_iAuthKey(r->m_iAuthKey)
	, m_tAuthTime(r->m_tAuthTime)
	, m_iGID(r->m_iGID)
	, m_iChannelID(r->m_iChannelID)
	, m_iState(r->m_iState)
	, m_tLastTick(r->m_tLastTick)
	, m_tLastSave(r->m_tLastSave)
	, m_spLink(r->m_spLink)
	, m_sAccount(r->m_sAccount)
	, m_pUserData(nullptr)
{
	UserData(r->m_pUserData);
}

Session::~Session()
{
	if (m_pUserData && 0 == m_pUserData->decref())
	{
		interface_ref* pTmpUserData = (interface_ref*)locked_exchange_pointer(m_pUserData, nullptr);
		safe_delete(pTmpUserData);
	}
}

void Session::SetLoginedInfo( GID iGID, MID iMID, CID iCID )
{
	m_iGID = iGID;
	m_iMasterID = iMID;
	m_iChannelID = iCID;
}

UID Session::GetUID()
{
	return m_iUID;
}

void Session::SetLink( NetLinkPtr spLink )
{
	m_spLink = spLink;
}

NetLinkPtr Session::GetLink()
{
	return m_spLink;
}

void Session::SetAuthKey( uint32 iAuthKey )
{
	m_iAuthKey = iAuthKey;
}

uint32 Session::GetAuthKey()
{
	return m_iAuthKey;
}

void Session::SetAccount( const std::string& sAccount )
{
	m_sAccount = sAccount;
}

const std::string& Session::GetAccount()
{
	return m_sAccount;
}

void Session::SetAuthTime()
{
	m_tAuthTime = time( NULL );
}

time_t Session::GetAuthTime()
{
	return m_tAuthTime;
}

GID Session::GetGID()
{
	return m_iGID;
}

MID Session::GetMasterID()
{
	return m_iMasterID;
}

CID Session::GetChannelID()
{
	return m_iChannelID;
}

void Session::SetState( int32 iState )
{
	m_iState = iState;
}

int32 Session::GetState()
{
	return m_iState;
}

bool Session::IsUpdated()
{
	return false;
}

bool Session::Save( bool bClosed )
{
	if( IsUpdated() )
	{
		return true;
	}

	return true;
}

void Session::SetLastTick()
{
	m_tLastTick = util::time2msec();
}

time_t Session::GetLastTick()
{
	return m_tLastTick;
}

time_t Session::GetLastSave()
{
	return m_tLastSave;
}

void Session::SetLastSave()
{
	m_tLastSave = util::time2msec();
}

void Session::SetSessionData( const Session& r )
{
	m_spLink = r.m_spLink;
	m_iAuthKey = r.m_iAuthKey;
	m_sAccount = r.m_sAccount;
	m_tAuthTime = r.m_tAuthTime;
	m_iState = r.m_iState;
	m_iGID = r.m_iGID;
	m_iMasterID = r.m_iMasterID;
	m_iChannelID = r.m_iChannelID;
	UserData(r.m_pUserData);
}

bool Session::IsAuthencation()
{
	return GetState() == eVERIFICATION;
}

void Session::UserData(interface_ref* pUserData)
{
	if (pUserData)
	{
		pUserData->incref();
	}

	if (m_pUserData && 0 == m_pUserData->decref())
	{
		interface_ref* pTmpUserData = (interface_ref*)locked_exchange_pointer(m_pUserData, nullptr);
		safe_delete(pTmpUserData);
	}

	locked_exchange_pointer(m_pUserData, pUserData);
}

interface_ref* Session::UserData(void)
{
	return m_pUserData;
}

Buffer& operator<<( Buffer& pk, const Session& r )
{
	pk << r.m_iUID << r.m_iAuthKey << r.m_sAccount
		<< r.m_tAuthTime << r.m_iGID
		<< r.m_iMasterID << r.m_iChannelID << r.m_iState;
	return pk;
}

Buffer& operator>>( Buffer& pk, Session& r )
{
	pk >> r.m_iUID >> r.m_iAuthKey >> r.m_sAccount
		>> r.m_tAuthTime >> r.m_iGID
		>> r.m_iMasterID >> r.m_iChannelID >> r.m_iState;
	r.m_pUserData = nullptr;
	return pk;
}
