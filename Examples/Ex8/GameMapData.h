#pragma once

#include <DataTypes.h>
#include <Ptr.h>

//////////////////////////////////////////////////////////////////////////
// 건축물 데이타
//////////////////////////////////////////////////////////////////////////
PTR(BuildingData);
class BuildingData : public single_ref
{
public:
	enum { eHouse,	eStore,	eHotel,	eLandMark, eMaxUpgrade };
	std::string m_sName;			/// 건축물이름
	int32		m_iPrice;			/// 건축물가격
	int32		m_iRoomCharge;		/// 숙박료
	short		m_iUpgradeLevel;	/// 건축물종류
	int32		m_iBuildingIdx;		/// 부지일련번호
public:
	BuildingData();
};

//////////////////////////////////////////////////////////////////////////
// 부지 데이타
//////////////////////////////////////////////////////////////////////////
PTR(LandData);
class LandData : public single_ref
{
public:
	std::string		m_sName;		/// 부지이름
	int32			m_iPrice;		/// 부지가격
	int32			m_iPassageMoney;/// 통행료
	int32			m_iLandIdx;		/// 부지일련번호

	typedef std::vector<BuildingDataPtr>	BuildingDataArray;
	BuildingDataArray	m_UpgradeBuilding;

public:
	LandData();
	BuildingDataPtr	SetBuildingData( short iLevel, short iBuildingType );
};

//////////////////////////////////////////////////////////////////////////
// 게임맵 데이타
//////////////////////////////////////////////////////////////////////////
PTR(GameMapData)
class GameMapData : public single_ref
{
public:
	std::string	m_sName;			/// 맵이름
	int32		m_iMapIdx;			/// 맵인덱스
	typedef std::vector<LandDataPtr>	LandDataArray;
	LandDataArray	m_Lands;
public:
	GameMapData();
	int32	Load( void );

	LandData*		Land( int32 iLandIdx );
	int32			LandPrice( int32 iLandIdx );
	std::string&	LandName( int32 iLandIdx );
	BuildingData*	Building( int32 iLandIdx, int32 iUpgradeLevel );
	int32			BuildingPrice( int32 iLandIdx, int32 iUpgradeLevel );
	std::string&	BuildingName( int32 iLandIdx, int32 iUpgradeLevel );
	int32			BuildingRoomCharge( int32 iLandIdx, int32 iUpgradeLevel );
	int32			TakeOverCharge( int32 iLandIdx, int32 iUpgradeLevel );
};

//////////////////////////////////////////////////////////////////////////
// 게임맵데이타 관리자
//////////////////////////////////////////////////////////////////////////
class GameMapDataMgr
{
public:
	typedef std::vector<GameMapDataPtr>		GameMapDataArray;
	GameMapDataArray		m_Maps;

private:
	GameMapDataMgr();
	GameMapDataMgr( const GameMapDataMgr& copy );
	virtual ~GameMapDataMgr();

public:
	static GameMapDataMgr* instance();
	GameMapData*	GetMap( int32 iMapIdx );

};