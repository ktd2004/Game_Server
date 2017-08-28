#include "stdafx.h"
#include <Logger.h>
#include "AAAConst.h"
#include "ChannelMgr.h"

Channel::Channel()
	: m_iCID(null_id)
	, m_iMaxClient(0)
	, m_iCurrentUser(0)
	, m_bBlock(false)
	, m_spChannelLink(nullptr)
{

}

Channel::~Channel()
{

}


ChannelMgr* ChannelMgr::instance()
{
	static ChannelMgr sInstance;
	return &sInstance;
}

void Channel::SetID( CID iCID )
{
	m_iCID = iCID;
}

CID Channel::GetID( void )
{
	return m_iCID;
}

void Channel::SetMaxClient( uint32 iMax )
{
	m_iMaxClient = iMax;
}

uint32 Channel::GetMaxClient( void )
{
	return m_iMaxClient;
}

void Channel::SetCurrentUser( uint32 iCurrent )
{
	m_iCurrentUser = iCurrent;
}

uint32 Channel::GetCurrentUser( void )
{
	return m_iCurrentUser;
}

void Channel::SetBlock( bool bBlock )
{
	m_bBlock = bBlock;
}

bool Channel::IsBlock( void )
{
	return m_bBlock;
}

void Channel::SetLink( NetLinkPtr spLink )
{
	m_spChannelLink = spLink;
}

NetLinkPtr Channel::GetLink()
{
	return m_spChannelLink;
}

ChannelMgr::ChannelMgr()
	: m_iTotalUser(0)
	, m_iFewCID(0)
{
}

ChannelMgr::~ChannelMgr()
{
}

ChannelMgr::ChannelMap* ChannelMgr::GetChannelMap()
{
	return &m_ChannelMap;
}

ChannelPtr ChannelMgr::Get( CID iCID )
{
	notfound_map_ret(ChannelMap, it, m_ChannelMap, iCID, nullptr);
	return it->second;
}

ChannelPtr ChannelMgr::Add( CID iCID, NetLinkPtr spLink )
{
	found_map_ret( ChannelMap, it, m_ChannelMap, iCID, false );
	ChannelPtr spChannel = new Channel();
	spChannel->SetID(iCID);
	spChannel->SetLink(spLink);
	m_ChannelMap.insert( ChannelMap::value_type(iCID, spChannel) );
	return spChannel;
}

CID ChannelMgr::UpdateLoadBalancing()
{
	uint32 iCount = 0, iMinUser = 99999;
	CID iFewCID = eLB_ALL_CHANNEL_BLOCKED;
	foreach_do(ChannelMap, it, m_ChannelMap)
	{
		ChannelPtr spChannel = it->second;
		iCount += spChannel->GetCurrentUser();
		if (!spChannel->IsBlock() &&
			spChannel->GetCurrentUser() < spChannel->GetMaxClient() &&
			spChannel->GetCurrentUser() < iMinUser)
		{
			iFewCID = it->first;
			iMinUser = spChannel->GetCurrentUser();
		}
	}

//	prn_dbg("FewCID = %u", iFewCID);

	SetFewChannelID(iFewCID);
	return iFewCID;
}

bool ChannelMgr::Remove( CID iCID )
{
	notfound_map_ret( ChannelMap, it, m_ChannelMap, iCID, false );
	m_ChannelMap.erase( it );
	return true;
}

void ChannelMgr::SetFewChannelID( CID iCID )
{
	m_iFewCID = iCID;
}

CID ChannelMgr::GetFewChannelID()
{
	return m_iFewCID;
}

int32 ChannelMgr::GetCount()
{
	return m_ChannelMap.size();
}

void ChannelMgr::RemoveAll()
{
	m_ChannelMap.clear();
}
