#pragma once

#include "GameMapData.h"

//////////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////////
PTR(Land);
class Land : public single_ref
{
public:
	LandData*		m_spLand;
	BuildingData*	m_spBuilding;	/// ������ ���� �����๰
	PlayerPtr		m_Owner;		/// ����������
	
	typedef std::vector<PlayerPtr>	PlayerArray;
	PlayerArray		m_Players;	/// ������ ���� �÷��̾�

public:
	Land();
	Land( LandData* spLand );

	void	Debug( void );

	int32	Add( PlayerPtr spPlayer );
	int32	Remove( PlayerPtr spPlayer );
};

//////////////////////////////////////////////////////////////////////////
// ���Ӹ�
//////////////////////////////////////////////////////////////////////////
PTR(GameMap)
class GameMap : public single_ref
{
public:
	GameMapData*	m_GameMapData;
	typedef std::vector<LandPtr>	Lands;
	Lands			m_Lands;
public:
	GameMap();
	void	Debug( void );
	void	DebugLand( void );

	int32	Load( void );

	size_t	Size( void );
	bool	IsEmptyLand( int32 iLandIdx );
	bool	IsOwner( int32 iLandIdx, PlayerPtr spPlayer );
	void	Move( int32 iLandIdx, PlayerPtr spPlayer );
	int32	ContructBuilding( int32 iLandIdx );
	int32	GetEmptyLandPrice( int32 iLandIdx );
	int32	SetBuyEmptyLand( int32 iLandIdx, PlayerPtr spPlayer );
	int32	GetUpgradeBuildingPrice( int32 iLandIdx );
	int32	SetUpgradeBuilding( int32 iLandIdx, PlayerPtr spPlayer );
	int32	GetRoomCharge( int32 iLandIdx );
	int32	GetTakeOverLandPrice( int32 iLandIdx );
	int32	SetTakeOverLand( int32 iLandIdx, PlayerPtr spPlayer );
	int32	GetUpgradeLevel( int32 iLandIdx );
};