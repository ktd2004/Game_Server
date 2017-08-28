#pragma once

#include <DataTypes.h>
#include <Ptr.h>

//////////////////////////////////////////////////////////////////////////
// ���๰ ����Ÿ
//////////////////////////////////////////////////////////////////////////
PTR(BuildingData);
class BuildingData : public single_ref
{
public:
	enum { eHouse,	eStore,	eHotel,	eLandMark, eMaxUpgrade };
	std::string m_sName;			/// ���๰�̸�
	int32		m_iPrice;			/// ���๰����
	int32		m_iRoomCharge;		/// ���ڷ�
	short		m_iUpgradeLevel;	/// ���๰����
	int32		m_iBuildingIdx;		/// �����Ϸù�ȣ
public:
	BuildingData();
};

//////////////////////////////////////////////////////////////////////////
// ���� ����Ÿ
//////////////////////////////////////////////////////////////////////////
PTR(LandData);
class LandData : public single_ref
{
public:
	std::string		m_sName;		/// �����̸�
	int32			m_iPrice;		/// ��������
	int32			m_iPassageMoney;/// �����
	int32			m_iLandIdx;		/// �����Ϸù�ȣ

	typedef std::vector<BuildingDataPtr>	BuildingDataArray;
	BuildingDataArray	m_UpgradeBuilding;

public:
	LandData();
	BuildingDataPtr	SetBuildingData( short iLevel, short iBuildingType );
};

//////////////////////////////////////////////////////////////////////////
// ���Ӹ� ����Ÿ
//////////////////////////////////////////////////////////////////////////
PTR(GameMapData)
class GameMapData : public single_ref
{
public:
	std::string	m_sName;			/// ���̸�
	int32		m_iMapIdx;			/// ���ε���
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
// ���Ӹʵ���Ÿ ������
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