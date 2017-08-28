#include "stdafx.h"

#include "GameMapData.h"

//////////////////////////////////////////////////////////////////////////

BuildingData::BuildingData()
{
}

//////////////////////////////////////////////////////////////////////////

LandData::LandData()
	: m_iPrice(50)
	, m_iPassageMoney(100)
	, m_sName("Unknown")
{
	m_UpgradeBuilding.resize(4);
}

BuildingDataPtr LandData::SetBuildingData( short iLevel, short iBuildingType )
{
	BuildingData* spBuilding = new BuildingData;
	spBuilding->m_iUpgradeLevel = iBuildingType;
	m_UpgradeBuilding[iLevel] = spBuilding;
	switch (iBuildingType)
	{
	case BuildingData::eHouse :
		spBuilding->m_sName = "House";
		break;
	case BuildingData::eStore :
		spBuilding->m_sName = "Store";
		break;
	case BuildingData::eHotel :
		spBuilding->m_sName = "Hotel";
		break;
	case BuildingData::eLandMark :
		spBuilding->m_sName = "LandMark";
		break;
	}
	return m_UpgradeBuilding[iLevel];
}

//////////////////////////////////////////////////////////////////////////

GameMapData::GameMapData()
{
	int32 iCol = 3;
	m_Lands.resize( iCol * 2 + (iCol-2) * 2 );
}

LandData* GameMapData::Land( int32 iLandIdx )
{
	return m_Lands[iLandIdx];
}

int32 GameMapData::LandPrice( int32 iLandIdx )
{
	return m_Lands[iLandIdx]->m_iPrice;
}

std::string& GameMapData::LandName( int32 iLandIdx )
{
	return m_Lands[iLandIdx]->m_sName;
}

BuildingData* GameMapData::Building( int32 iLandIdx, int32 iUpgradedLevel )
{
	return m_Lands[iLandIdx]->m_UpgradeBuilding[iUpgradedLevel];
}

int32 GameMapData::BuildingPrice( int32 iLandIdx, int32 iUpgradedLevel )
{
	return m_Lands[iLandIdx]->m_UpgradeBuilding[iUpgradedLevel]->m_iPrice;
}

std::string& GameMapData::BuildingName( int32 iLandIdx, int32 iUpgradedLevel )
{
	return m_Lands[iLandIdx]->m_UpgradeBuilding[iUpgradedLevel]->m_sName;
}

int32 GameMapData::BuildingRoomCharge( int32 iLandIdx, int32 iUpgradeLevel )
{
	return m_Lands[iLandIdx]->m_UpgradeBuilding[iUpgradeLevel]->m_iRoomCharge;
}

int32 GameMapData::TakeOverCharge( int32 iLandIdx, int32 iUpgradeLevel )
{
	int32 iSum = m_Lands[iLandIdx]->m_iPrice;
	for ( int32 i = 0; i < iUpgradeLevel; i++ )
	{
		iSum += m_Lands[iLandIdx]->m_UpgradeBuilding[i]->m_iPrice;
	}

	return iSum;
}

namespace {
	void TestGenBuildingData( LandData* pLand )
	{
		BuildingData* pBuilding = nullptr;

		for ( int32 i = 0; i < BuildingData::eMaxUpgrade; i++ )
		{
			pBuilding = pLand->SetBuildingData( i, i );
			pBuilding->m_iPrice = (int32)(pLand->m_iPrice * 0.5f + 100 * i * 0.3f);
			pBuilding->m_iRoomCharge = (int32)((pBuilding->m_iPrice + pLand->m_iPrice) * 0.4f);
		}
	}
};

int32 GameMapData::Load()
{
	for ( size_t i = 0; i < m_Lands.size(); i++ )
	{
		m_Lands[i] = new LandData;
	}

	m_Lands[0]->m_sName = "서울";
	m_Lands[0]->m_iLandIdx = 0;
	m_Lands[0]->m_iPrice = 1000;
	m_Lands[0]->m_iPassageMoney = (int32)(m_Lands[0]->m_iPrice * 0.1f);
	TestGenBuildingData( m_Lands[0] );

	m_Lands[1]->m_sName = "대전";
	m_Lands[1]->m_iLandIdx = 1;
	m_Lands[1]->m_iPrice = 700;
	m_Lands[1]->m_iPassageMoney = (int32)(m_Lands[1]->m_iPrice * 0.1f);
	TestGenBuildingData( m_Lands[1] );

	m_Lands[2]->m_sName = "대구";
	m_Lands[2]->m_iLandIdx = 2;
	m_Lands[2]->m_iPrice = 500;
	m_Lands[2]->m_iPassageMoney = (int32)(m_Lands[2]->m_iPrice * 0.1f);
	TestGenBuildingData( m_Lands[2] );

	m_Lands[3]->m_sName = "부산";
	m_Lands[3]->m_iLandIdx = 3;
	m_Lands[3]->m_iPrice = 800;
	m_Lands[3]->m_iPassageMoney = (int32)(m_Lands[3]->m_iPrice * 0.1f);
	TestGenBuildingData( m_Lands[3] );

	m_Lands[4]->m_sName = "목포";
	m_Lands[4]->m_iLandIdx = 4;
	m_Lands[4]->m_iPrice = 400;
	m_Lands[4]->m_iPassageMoney = (int32)(m_Lands[4]->m_iPrice * 0.1f);
	TestGenBuildingData( m_Lands[4] );

	m_Lands[5]->m_sName = "여수";
	m_Lands[5]->m_iLandIdx = 5;
	m_Lands[5]->m_iPrice = 400;
	m_Lands[5]->m_iPassageMoney = (int32)(m_Lands[5]->m_iPrice * 0.1f);
	TestGenBuildingData( m_Lands[5] );

	m_Lands[6]->m_sName = "대천";
	m_Lands[6]->m_iLandIdx = 6;
	m_Lands[6]->m_iPrice = 300;
	m_Lands[6]->m_iPassageMoney = (int32)(m_Lands[6]->m_iPrice * 0.1f);
	TestGenBuildingData( m_Lands[6] );

	m_Lands[7]->m_sName = "강릉";
	m_Lands[7]->m_iLandIdx = 7;
	m_Lands[7]->m_iPrice = 300;
	m_Lands[7]->m_iPassageMoney = (int32)(m_Lands[7]->m_iPrice * 0.1f);
	TestGenBuildingData( m_Lands[7] );

	return 1;
}

//////////////////////////////////////////////////////////////////////////


GameMapDataMgr::GameMapDataMgr()
{
	m_Maps.resize(1);
	m_Maps[0] = new GameMapData;
	m_Maps[0]->m_iMapIdx = 0;
	m_Maps[0]->m_sName = "테스트맵";
	m_Maps[0]->Load();
}

GameMapDataMgr::GameMapDataMgr( const GameMapDataMgr& copy )
{
}

GameMapDataMgr::~GameMapDataMgr()
{
}

GameMapDataMgr* GameMapDataMgr::instance()
{
	static GameMapDataMgr inst;
	return &inst;
}

GameMapData* GameMapDataMgr::GetMap( int32 iMapIdx )
{
	return m_Maps[iMapIdx];
}

