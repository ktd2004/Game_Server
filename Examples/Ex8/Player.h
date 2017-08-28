#pragma once

#include <DataTypes.h>
#include <Ptr.h>
#include <Functional.h>
#include <FSM.h>

PTR(GameRoom);
PTR(Player);
class Player : public single_ref, public FSM::Machine
{
public:
	int32		m_iAccUID;
	int32		m_iLandIdx;
	GameRoomPtr	m_spGameRoom;
	int32		m_iSlotIdx;
	int32		m_iState;
	bool		m_bKonkrus;
	bool		m_bVictory;

	///
	int32		m_iGolds;
	typedef std::vector<int32>	MyLands;
	MyLands		m_MyLands;

	///
	int32		m_iFirstDice;
	int32		m_iSecondDice;

	int32		m_iUpgradeLevel;
	int32		m_iTakeOverPrice;
	int32		m_iPayRoomCharge;

public:
	Player( GameRoomPtr spGameRoom );
	void	OnReset( void );
	void	SetGameRoom( GameRoomPtr spGameRoom );
	void	SetID( int32 iAccUID );
	int32	GetID( void );
	int32	GetRoomNum( void );
	int32	GetSlotNum( void );
	bool	IsKonkrus( void );
	bool	IsVictory( void );

	///
	int32	DiceCast( void );			// 주사위를 던지다
	int32	Move( void );				// 이동하다
	int32	BuyEmptyLand( void );		// 빈땅을 구매하다
	int32	UpgradeBuilding( void );	// 업그레이드 하다
	int32	PayRoomCharge( void );		// 숙박료를 지불하다
	int32	TakeOverLand( void );		// 인수하다
	int32	Konkrus( void );			// 파산하다
	int32	SellLand( void );			// 토지를 팔다
};