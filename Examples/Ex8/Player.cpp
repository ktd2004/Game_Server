#include "stdafx.h"

#include <Random.h>
#include "Player.h"
#include "GameMap.h"
#include "GameRoom.h"
#include "BlueMarbleRule.h"

Player::Player( GameRoomPtr spGameRoom )
	: m_spGameRoom(spGameRoom)
{
	OnReset();
}

void Player::OnReset( void )
{
	m_iLandIdx = -1;
	m_iSlotIdx = -1;
	m_iState = 0;
	m_bKonkrus = false;
	m_bVictory = false;

	m_iGolds = 10000;

	m_iTakeOverPrice = 0;
	m_iPayRoomCharge = 0;
	m_iUpgradeLevel = 0;
}

void Player::SetGameRoom( GameRoomPtr spGameRoom )
{
	m_spGameRoom = spGameRoom;
}

void Player::SetID( int32 iAccUID )
{
	m_iAccUID = iAccUID;
}

int32 Player::GetID( void )
{
	return m_iAccUID;
}

int32 Player::GetRoomNum( void )
{
	if ( m_spGameRoom )
	{
		return m_spGameRoom->GetID();
	}

	return -1;
}

int32 Player::GetSlotNum( void )
{
	if ( m_spGameRoom )
	{
		return m_iSlotIdx;
	}

	return -1;
}

bool Player::IsKonkrus( void )
{
	return m_bKonkrus;
}

bool Player::IsVictory( void )
{
	return m_bVictory;
}

int32 Player::DiceCast( void )
{
	Random rnd;
	for ( int32 i = 0; i < 8; i++ )
		rnd.Px( 1.0/8.0f );

	m_iFirstDice = rnd.Rand() + 1;
	m_iState = eMOVE_PLAYER;
	// 패킷 브로드캐스팅
	return m_iFirstDice + m_iSecondDice;
}

int32 Player::Move( void )
{
	GameMap& MapData = *(m_spGameRoom->GetGameMap());
	int32 iMovePos = (m_iLandIdx + m_iFirstDice + m_iSecondDice) % MapData.Size();
	MapData.Move( iMovePos, this );
	if ( MapData.IsEmptyLand(iMovePos) )
	{
		// 빈토지
		m_iTakeOverPrice = MapData.GetEmptyLandPrice( iMovePos );
		m_iUpgradeLevel = 0;
		m_iState = eWAIT_BUY_LAND;
	}
	else if ( MapData.IsOwner(iMovePos, this) )
	{
		// 자신의 소유 토지
		m_iTakeOverPrice = MapData.GetUpgradeBuildingPrice( iMovePos );
		m_iUpgradeLevel = MapData.GetUpgradeLevel( iMovePos );
		m_iState = eWAIT_UPGRADE_BUILDING;
	}
	else
	{
		// 다른 플레이어 토지
		m_iTakeOverPrice = MapData.GetTakeOverLandPrice( iMovePos );
		m_iPayRoomCharge = MapData.GetRoomCharge( iMovePos );
		m_iUpgradeLevel = MapData.GetUpgradeLevel( iMovePos );

		if ( m_iGolds < m_iPayRoomCharge )
		{
			int32 iPrice = 0;
			for ( size_t i = 0; i < m_MyLands.size(); i++ )
			{
				iPrice += MapData.GetTakeOverLandPrice( m_MyLands[i] );
			}
			if ( m_iGolds + iPrice >= m_iTakeOverPrice )
			{
				// 자산을 매각해야 한다.
				m_iState = eWAIT_CONFIRM_KONKRUS;
			}
			else
			{
				// 파산
				// 승리조건 -> 나를 제외한 모든 플레이어 파산.
				m_iState = eKONKRUS;
				m_bKonkrus = true;
			}
		}
		else
		{
			// 숙박료를 지불한다.
			m_iState = eWAIT_PAY_ROOMCHARGE;
		}
	}

	return 1;
}

int32 Player::BuyEmptyLand( void )
{
	GameMap& MapData = *(m_spGameRoom->GetGameMap());
	MapData.SetBuyEmptyLand( m_iLandIdx, this );
	m_MyLands.push_back( m_iLandIdx );
	m_iGolds -= m_iTakeOverPrice;
	m_iTakeOverPrice = 0;
	if ( m_spGameRoom->CheckVictory() )
	{
		printf( "victory game : player(%d)\n", m_spGameRoom->GetVictoryPlayer()->GetID() );
	}
	else
	{
		m_iState = eNONE_STATE;
		m_spGameRoom->MoveNextPlayer();
	}
	return 1;
}

int32 Player::PayRoomCharge( void )
{
	m_iGolds -= m_iPayRoomCharge;
	m_iPayRoomCharge = 0;
	if ( m_iGolds >= m_iTakeOverPrice )
	{
		m_iState = eWAIT_TAKE_OVER;
	}
	else
	{
		m_iState = eNONE_STATE;
		m_spGameRoom->MoveNextPlayer();
	}
	return 1;
}

int32 Player::TakeOverLand( void )
{
	m_iGolds -= m_iTakeOverPrice;
	m_iTakeOverPrice = 0;
	GameMap& MapData = *(m_spGameRoom->GetGameMap());
	MapData.SetTakeOverLand( m_iLandIdx, this );
	if ( m_spGameRoom->CheckVictory() )
	{
		printf( "victory game : player(%d)\n", m_spGameRoom->GetVictoryPlayer()->GetID() );
	}
	else
	{
		m_iState = eNONE_STATE;
		m_spGameRoom->MoveNextPlayer();
	}
	return 1;
}

int32 Player::Konkrus( void )
{
	if ( m_spGameRoom->CheckVictory() )
	{
		printf( "victory game : player(%d)\n", m_spGameRoom->GetVictoryPlayer()->GetID() );
	}
	else
	{
		m_iState = eNONE_STATE;
		m_spGameRoom->MoveNextPlayer();
	}
	return 1;
}

int32 Player::SellLand( void )
{
	GameMap& MapData = *(m_spGameRoom->GetGameMap());
	return 1;
}

int32 Player::UpgradeBuilding( void )
{
	GameMap& MapData = *(m_spGameRoom->GetGameMap());
	MapData.SetUpgradeBuilding( m_iLandIdx, this );
	m_iState = eNONE_STATE;
	m_spGameRoom->MoveNextPlayer();
	return 1;
}