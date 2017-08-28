#include "stdafx.h"

#include "Player.h"
#include "GameRoom.h"
#include "GameMap.h"

//////////////////////////////////////////////////////////////////////////

Land::Land()
{
	m_Players.resize(4);
}

Land::Land( LandData* spLand )
{
	m_Players.resize(4);
	m_spLand = spLand;
}

void Land::Debug( void )
{
	printf( "%s : owner(%d) building(%s) players(", cstr(m_spLand->m_sName), m_Owner ? m_Owner->GetID() : -1, m_spBuilding ? cstr(m_spBuilding->m_sName) : "" );
	for ( size_t i = 0; i < m_Players.size(); i++ )
	{
		if ( m_Players[i] )
		{
			printf( "%d ", m_Players[i]->GetID() );
		}
	}

	printf( ")\n" );
}

int32 Land::Add( PlayerPtr spPlayer )
{
	m_Players[spPlayer->GetSlotNum()] = spPlayer;
	spPlayer->m_iLandIdx = m_spLand->m_iLandIdx;
	return 1;
}

int32 Land::Remove( PlayerPtr spPlayer )
{
	if ( spPlayer->m_iLandIdx == m_spLand->m_iLandIdx )
	{
		m_Players[spPlayer->GetSlotNum()] = nullptr;
		spPlayer->m_iLandIdx = -1;
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////

GameMap::GameMap()
{
	int32 iCol = 3;
	m_Lands.resize( iCol * 2 + (iCol-2) * 2 );
}


void GameMap::Debug( void )
{
	printf( "map(%s)\n", cstr(m_GameMapData->m_sName) );
	for ( size_t i = 0; i < m_GameMapData->m_Lands.size(); i++ )
	{
		printf( "%s(%d) : passage money(%d), price(%d)\n", 
			cstr(m_GameMapData->m_Lands[i]->m_sName), 
			m_GameMapData->m_Lands[i]->m_iLandIdx,
			m_GameMapData->m_Lands[i]->m_iPassageMoney,
			m_GameMapData->m_Lands[i]->m_iPrice );
		for ( size_t j = 0; j < m_GameMapData->m_Lands[i]->m_UpgradeBuilding.size(); j++ )
		{
			printf( "\t%s : level(%d), room charge(%d), price(%d)\n", 
				cstr(m_GameMapData->m_Lands[i]->m_UpgradeBuilding[j]->m_sName),
				m_GameMapData->m_Lands[i]->m_UpgradeBuilding[j]->m_iUpgradeLevel,
				m_GameMapData->m_Lands[i]->m_UpgradeBuilding[j]->m_iRoomCharge,
				m_GameMapData->m_Lands[i]->m_UpgradeBuilding[j]->m_iPrice);
		}
	}
}


void GameMap::DebugLand( void )
{
	for ( size_t i = 0; i < m_GameMapData->m_Lands.size(); i++ )
	{
		m_Lands[i]->Debug();
	}
}


int32 GameMap::Load()
{
	GameMapDataMgr::instance();
	m_GameMapData = GameMapDataMgr::instance()->GetMap( 0 );
	m_Lands.clear();
	m_Lands.resize(m_GameMapData->m_Lands.size());
	for ( size_t i = 0; i < m_GameMapData->m_Lands.size(); i++ )
	{
		m_Lands[i] = new Land( m_GameMapData->m_Lands[i] );
	}

	Debug();
	return 1;
}

size_t GameMap::Size()
{
	return m_Lands.size();
}

bool GameMap::IsEmptyLand( int32 iLandIdx )
{
	if (m_Lands[iLandIdx]->m_Owner == nullptr)
		return true;
	else
		return false;
}

bool GameMap::IsOwner( int32 iLandIdx, PlayerPtr spPlayer )
{
	if ( m_Lands[iLandIdx]->m_Owner == spPlayer )
		return true;
	else
		return false;
}

void GameMap::Move( int32 iLandIdx, PlayerPtr spPlayer )
{
	m_Lands[spPlayer->m_iLandIdx]->Remove( spPlayer );
	m_Lands[iLandIdx]->Add( spPlayer );
}

int32 GameMap::ContructBuilding( int32 iLandIdx )
{
	if ( m_Lands[iLandIdx]->m_spBuilding )
	{
		int32 iUpgradeLevel = m_Lands[iLandIdx]->m_spBuilding->m_iUpgradeLevel;
		if ( iUpgradeLevel + 1 < BuildingData::eMaxUpgrade )
		{
			m_Lands[iLandIdx]->m_spBuilding = m_GameMapData->Building( iLandIdx, iUpgradeLevel+1);
		}
		else
		{
			return -1;
		}
		
	}
	
	return 1;
}

int32 GameMap::GetEmptyLandPrice( int32 iLandIdx )
{
	return m_GameMapData->LandPrice( iLandIdx ) + m_GameMapData->BuildingPrice( iLandIdx, 0 );
}

int32 GameMap::SetBuyEmptyLand( int32 iLandIdx, PlayerPtr spPlayer )
{
	m_Lands[iLandIdx]->m_spBuilding = m_Lands[iLandIdx]->m_spLand->m_UpgradeBuilding[0];
	m_Lands[iLandIdx]->m_Owner = spPlayer;
	return 1;
}

int32 GameMap::GetUpgradeBuildingPrice( int32 iLandIdx )
{
	int32 iUpgradeLevel = m_Lands[iLandIdx]->m_spBuilding->m_iUpgradeLevel + 1;
	return m_GameMapData->BuildingPrice( iLandIdx, iUpgradeLevel );
}

int32 GameMap::SetUpgradeBuilding( int32 iLandIdx, PlayerPtr spPlayer )
{
	int32 iUpgradeLevel = m_Lands[iLandIdx]->m_spBuilding->m_iUpgradeLevel + 1;
	m_Lands[iLandIdx]->m_spBuilding = m_Lands[iLandIdx]->m_spLand->m_UpgradeBuilding[iUpgradeLevel];
	return 1;
}

int32 GameMap::GetTakeOverLandPrice( int32 iLandIdx )
{
	int32 iUpgradeLevel = m_Lands[iLandIdx]->m_spBuilding->m_iUpgradeLevel;
	return m_GameMapData->TakeOverCharge( iLandIdx, iUpgradeLevel );
}

int32 GameMap::SetTakeOverLand( int32 iLandIdx, PlayerPtr spPlayer )
{
	m_Lands[iLandIdx]->m_Owner = spPlayer;
	return 1;
}

int32 GameMap::GetRoomCharge( int32 iLandIdx )
{
	int32 iUpgradeLevel = m_Lands[iLandIdx]->m_spBuilding->m_iUpgradeLevel;
	return m_GameMapData->BuildingRoomCharge( iLandIdx, iUpgradeLevel );
}

int32 GameMap::GetUpgradeLevel( int32 iLandIdx )
{
	int32 iUpgradeLevel = m_Lands[iLandIdx]->m_spBuilding->m_iUpgradeLevel;
	return iUpgradeLevel;
}
