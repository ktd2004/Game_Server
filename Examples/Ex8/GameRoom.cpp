#include "stdafx.h"

#include <Application.h>
#include <Random.h>
#include "Player.h"
#include "GameMap.h"
#include "GameRoom.h"
#include "BlueMarbleRule.h"

GameRoom::GameRoom()
{
	m_Players.resize( 4 );
	m_SlotState.resize( 4 );
	OnInitialize();
}

GameRoom::~GameRoom()
{
}

void GameRoom::OnInitialize( void )
{
	// 룸번호는 생성시점에 할당되고 삭제시점에는 초기화하지 않는다.
	for ( size_t i = 0; i < m_Players.size(); i++ )
	{
		m_Players[i] = nullptr;
		if ( i == 0 )
			m_SlotState[i] = eOPENED;
		else
			m_SlotState[i] = eCLOSED;
	}

	m_spGameMap = nullptr;
	m_spOwnerPlayer = nullptr;
	m_iState = GameRoom::eONLINE;
	m_iPlayerCount = 0;
	m_iOpenSlotCount = 1;

	OnReset();
}

void GameRoom::OnReset()
{
	m_iTurnOverTick = 0;
	m_iPrevPlayerIdx = -1;
	m_iCurrentPlayerIdx = 0;
	m_iLastTick = 0;
}

void GameRoom::OnFinalize( void )
{
	// 룸번호는 생성시점에 할당되고 삭제시점에는 초기화하지 않는다.
	for ( size_t i = 0; i < m_Players.size(); i++ )
	{
		if ( m_Players[i] )
		{
			m_Players[i]->OnReset();
			m_Players[i]->SetGameRoom(nullptr);
		}
	}

	OnInitialize();
}


int32 GameRoom::GetID( void )
{
	return m_iRoomIdx;
}

void GameRoom::SetID( int32 iRoomIdx )
{
	m_iRoomIdx = iRoomIdx;
}

void GameRoom::SetOwner( PlayerPtr spPlayer )
{
	m_spOwnerPlayer = spPlayer;
}

PlayerPtr GameRoom::GetOwner( void )
{
	return m_spOwnerPlayer;
}

void GameRoom::CloseSlot( int32 iSlotNum )
{
	if (m_Players[iSlotNum] == nullptr && m_SlotState[iSlotNum] == eOPENED)
	{
		m_SlotState[iSlotNum] = eCLOSED;
		m_iOpenSlotCount--;
	}
}

void GameRoom::OpenSlot( int32 iSlotNum )
{
	if ( m_SlotState[iSlotNum] == eCLOSED )
	{
		m_SlotState[iSlotNum] = eOPENED;
		m_iOpenSlotCount++;
	}
}

int32 GameRoom::GetOpenSlotCount( void )
{
	return m_iOpenSlotCount;
}


int32 GameRoom::GetPlayerCount( void )
{
	return m_iPlayerCount;
}

int32 GameRoom::Add( PlayerPtr spPlayer )
{
	for ( size_t i = 0; i < m_Players.size(); i++ )
	{
		if (m_Players[i] == nullptr && m_SlotState[i] == eOPENED)
		{
			spPlayer->OnReset();
			spPlayer->SetGameRoom(this);
			spPlayer->m_iSlotIdx = i;
			m_spGameMap->m_Lands[0]->Add( spPlayer );
			if (m_spOwnerPlayer == nullptr)
			{
				m_spOwnerPlayer = spPlayer;
				m_iOpenSlotCount = 1;
			}
			m_Players[i] = spPlayer;
			m_iPlayerCount++;
			return i;
		}
	}

	return -1;
}

int32 GameRoom::Remove( PlayerPtr spPlayer )
{
	int32 iIdx = spPlayer->m_iSlotIdx;
	if (m_Players[iIdx] != nullptr && m_Players[iIdx] == spPlayer)
	{
		m_Players[iIdx] = nullptr;
		spPlayer->OnReset();
		spPlayer->SetGameRoom(nullptr);
		m_spGameMap->m_Lands[0]->Remove( spPlayer );
		if ( m_spOwnerPlayer == spPlayer )
		{
			for ( size_t i = 0; i < m_Players.size(); i++ )
			{
				int32 iPos = (i + spPlayer->GetSlotNum() + 1) % m_Players.size();
				if (m_Players[iPos] != nullptr)
				{
					m_spOwnerPlayer = m_Players[iPos];
					break;
				}
			}
		}
		m_iPlayerCount--;
		return iIdx;
	}

	return -1;
}

int32 GameRoom::Remove( int32 iSlotNum )
{
	if (m_Players[iSlotNum] != nullptr)
	{
		PlayerPtr spPlayer = m_Players[iSlotNum];
		return Remove( spPlayer );
	}

	return -1;
}

GameMap* GameRoom::GetGameMap( void )
{
	return m_spGameMap;
}

int32 GameRoom::RandomSelectPlayer( void )
{
	Random rnd;
	for ( size_t i = 0; i < m_Players.size(); i++ )
	{
		if ( m_Players[i] )
			rnd.Px( 1.0/(float)m_iPlayerCount );
		else
		{
			rnd.Px( 0.0f );
		}
	}

	m_iCurrentPlayerIdx = rnd.Rand();
	return m_iCurrentPlayerIdx;
}


Player* GameRoom::GetCurrentPlayer( void )
{
	if (m_Players[m_iCurrentPlayerIdx] != nullptr)
	{
		return m_Players[m_iCurrentPlayerIdx];
	}

	return nullptr;
}


Player* GameRoom::MoveNextPlayer( void )
{
	for ( size_t i = 0; i < m_Players.size(); i++ )
	{
		int32 iPos = (i + m_iCurrentPlayerIdx + 1) % m_Players.size();
		if (m_Players[iPos] != nullptr)
		{
			m_iPrevPlayerIdx = m_iCurrentPlayerIdx;
			m_iCurrentPlayerIdx = iPos;
			return m_Players[iPos];
		}
	}

	// 여기까지 들어오면 에러 상황 ㅠㅠ
	return nullptr;
}


void GameRoom::EndGame( void )
{
	for ( size_t i = 0; i < m_Players.size(); i++ )
	{
		if ( m_Players[i] )
		{
			m_Players[i]->OnReset();
		}
	}
	GameRoomMgr::instance()->SetWaitRoom( this );
}

void GameRoom::ReadyGame( int32 iSlotNum )
{
	if ( m_Players[iSlotNum] && m_Players[iSlotNum]->m_iState != eREADY_GAME )
	{
		m_Players[iSlotNum]->m_iState = eREADY_GAME;
	}
}


bool GameRoom::StartGame( int32 iSlotNum )
{
	if ( m_spOwnerPlayer == m_Players[iSlotNum] )
	{
		int32 iCount = 0;
		for ( size_t i = 0; i < m_Players.size(); i++ )
		{
			if ( m_Players[i] && m_Players[i]->m_iState == eREADY_GAME )
				iCount++;
		}

		if ( iCount == GetPlayerCount() && iCount == GetOpenSlotCount() )
		{
			
			return true;
		}
	}

	return false;
}


bool GameRoom::PlayGame( int32 iSlotNum )
{
	if ( m_Players[iSlotNum] && m_Players[iSlotNum]->m_iState == eREADY_GAME )
	{
		m_Players[iSlotNum]->m_iState = ePLAY_GAME;
	}

	int32 iCount = 0;
	for ( size_t i = 0; i < m_Players.size(); i++ )
	{
		if ( m_Players[i] && m_Players[i]->m_iState == ePLAY_GAME )
			iCount++;
	}

	if ( iCount == GetPlayerCount() && iCount == GetOpenSlotCount() )
	{
		// 선공할 플레이어를 선택해서 구성원에게 알려준다. 
		RandomSelectPlayer();
		GameRoomMgr::instance()->SetPlayRoom( this );
		return true;
	}

	return false;
}

GameRoom::PlayerSlot& GameRoom::GetPlayerSlot( void )
{
	return m_Players;
}

GameRoom::SlotState& GameRoom::GetSlotState( void )
{
	return m_SlotState;
}

bool GameRoom::CheckVictory( void )
{
	int32 iKonkrus = 0;
	for ( size_t i = 0; i < m_Players.size(); i++ )
	{
		if ( m_Players[i] )
		{
			if ( m_Players[i]->IsKonkrus() )
			{
				iKonkrus++;
			}
			else
			{
				m_iVictoryPlayerIdx = i;
			}
		}
	}


	if ( iKonkrus >= GetPlayerCount() - 1 )
	{
		// 승리한 플레이어
		return true;
	}

	// 트리풀 독점 했냐 ?


	// 관광지 독점 했냐 ?

	return false;
}

Player* GameRoom::GetVictoryPlayer( void )
{
	return m_Players[m_iVictoryPlayerIdx];
}

void GameRoom::CreateTimer(uint32 iMSec)
{
	m_spTimerHandle = CREATE_TIMER(Application::GetProperty()->iNotifyTaskSchedulerID, this, &GameRoom::OnTimeout, iMSec, nullptr);
}

bool GameRoom::OnTimeout(void* pUserData)
{
	PlayerPtr spPlayer = GetCurrentPlayer();
	if ( spPlayer )
	{
	}

	return false;
}

void GameRoom::CancelTimer(void)
{
	if ( m_spTimerHandle )
	{
		CANCEL_TIMER( Application::GetProperty()->iNotifyTaskSchedulerID, m_spTimerHandle );
	}
}

//////////////////////////////////////////////////////////////////////////

GameRoomMgr::GameRoomMgr()
	: m_iRoomIdx(1)
{
}

GameRoomMgr::GameRoomMgr( const GameRoomMgr& copy )
{

}

GameRoomMgr::~GameRoomMgr()
{

}

GameRoomMgr* GameRoomMgr::instance()
{
	static GameRoomMgr inst;
	return &inst;
}

GameRoomPtr GameRoomMgr::Create( void )
{
	GameRoomPtr spGameRoom;
	GameRoomMap::iterator it = m_OfflineRoomMap.begin();
	if ( it != m_OfflineRoomMap.end() )
	{
		spGameRoom = it->second;
		spGameRoom->OnInitialize();
		spGameRoom->m_iState = GameRoom::eONLINE;
		m_OfflineRoomMap.erase( it );
		m_OnlineRoomMap.insert( GameRoomMap::value_type(spGameRoom->GetID(), spGameRoom) );
	}
	else
	{
		spGameRoom = new GameRoom;
		spGameRoom->OnInitialize();
		spGameRoom->m_iState = GameRoom::eONLINE;
		spGameRoom->SetID( m_iRoomIdx++ );
		m_OnlineRoomMap.insert( GameRoomMap::value_type(spGameRoom->GetID(), spGameRoom) );
	}

	return spGameRoom;
}

void GameRoomMgr::SetPlayRoom( GameRoomPtr spGameRoom )
{
	if ( spGameRoom->m_iState == GameRoom::eONLINE )
	{
		spGameRoom->m_iState = GameRoom::ePLAYING;
		m_OnlineRoomMap.erase( spGameRoom->GetID() );
		m_OnPlayRoomMap.insert( GameRoomMap::value_type(spGameRoom->GetID(), spGameRoom) );
	}
}

void GameRoomMgr::SetWaitRoom( GameRoomPtr spGameRoom )
{
	if ( spGameRoom->m_iState == GameRoom::ePLAYING )
	{
		spGameRoom->m_iState = GameRoom::eONLINE;
		m_OnPlayRoomMap.erase( spGameRoom->GetID() );
		m_OnlineRoomMap.insert( GameRoomMap::value_type(spGameRoom->GetID(), spGameRoom) );
	}
}

void GameRoomMgr::Delete( GameRoomPtr spGameRoom )
{
	if ( spGameRoom->m_iState == GameRoom::eONLINE )
	{
		m_OnlineRoomMap.erase( spGameRoom->GetID() );
		m_OnPlayRoomMap.erase( spGameRoom->GetID() );
		spGameRoom->OnFinalize();
		m_OfflineRoomMap.insert( GameRoomMap::value_type(spGameRoom->GetID(), spGameRoom) );
	}
}