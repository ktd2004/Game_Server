#include "stdafx.h"
#include "MasterMgr.h"
#include <Logger.h>


Master::Master()
	: m_iMID(null_id)
	, m_bBlock(false)
{
}

Master::~Master()
{
}

MID Master::GetID()
{
	return m_iMID;
}

void Master::SetID(MID iMID)
{
	m_iMID = iMID;
}

void Master::SetLink(NetLinkPtr spLink)
{
	m_spMasterLink = spLink;
}

NetLinkPtr Master::GetLink(void)
{
	return m_spMasterLink;
}

void Master::SetBlock(bool bBlock)
{
	m_bBlock = bBlock;
}

bool Master::IsBlock(bool bBlock)
{
	return m_bBlock;
}

////////////////////////////////////////////////////////////////////////////////

MasterMgr::MasterMgr()
{
}

MasterMgr::~MasterMgr()
{
	m_MasterMap.clear();
}

MasterMgr* MasterMgr::instance()
{
	static MasterMgr sInstance;
	return &sInstance;
}

MasterPtr MasterMgr::Add( MID iMID, NetLinkPtr spLink )
{
	// 이미 등록된 마스터면 실패 처리해야 된다.
	MasterMap::iterator itMaster = m_MasterMap.find( iMID );
	if( itMaster != m_MasterMap.end() )
	{
		return itMaster->second;
	}

	MasterPtr spMaster = new Master;
	spMaster->m_iMID = iMID;
	spMaster->m_spMasterLink = spLink;
	m_MasterMap.insert( MasterMap::value_type( iMID, spMaster ) );

	return spMaster;
}

MasterPtr MasterMgr::Get( MID iMID )
{
	found_map_ret( MasterMap, itM, m_MasterMap, iMID, itM->second );
	return nullptr;
}

bool MasterMgr::Remove( MID iMID )
{
	notfound_map_ret( MasterMap, it, m_MasterMap, iMID, false );
	m_MasterMap.erase( it );
	return true;
}

void MasterMgr::RemoveAll()
{
	m_MasterMap.clear();
}

MasterMgr::MasterMap* MasterMgr::GetMasterMap()
{
	return &m_MasterMap;
}

int32 MasterMgr::GetMasterCount()
{
	return m_MasterMap.size();
}
