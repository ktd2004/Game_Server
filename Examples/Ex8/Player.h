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
	int32	DiceCast( void );			// �ֻ����� ������
	int32	Move( void );				// �̵��ϴ�
	int32	BuyEmptyLand( void );		// ���� �����ϴ�
	int32	UpgradeBuilding( void );	// ���׷��̵� �ϴ�
	int32	PayRoomCharge( void );		// ���ڷḦ �����ϴ�
	int32	TakeOverLand( void );		// �μ��ϴ�
	int32	Konkrus( void );			// �Ļ��ϴ�
	int32	SellLand( void );			// ������ �ȴ�
};