// Ex8.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <MemoryEx.h>
#include <ServerFrame.h>

#include "Player.h"
#include "GameMap.h"
#include "GameRoom.h"
#include "Command.h"
#include "BlueMarbleRule.h"
#include <FSM.h>

#include <conio.h>	// for _kbhit()

#define trace		printf( __FUNCTION__"\n" )


/// for test
class Ex8Handler : public ServerApp
{
	GameRoomPtr		m_GameRoom;
	int32			m_idx;
public:
	Ex8Handler(int argc, char* argv[]) : ServerApp(argc, argv)
	{
		m_idx = 1;
		m_GameRoom = GameRoomMgr::instance()->Create();
		m_GameRoom->m_spGameMap = new GameMap;
		m_GameRoom->m_spGameMap->Load();
	}

	virtual ~Ex8Handler() {}

	bool OnSetProperty( ServerApp::Properties& appCfg ) override
	{
		appCfg.iSID						= 1;
		appCfg.iTaskDuration			= 10;
		appCfg.bLogArchive				= false;
		appCfg.bLogTransmit				= false;
		appCfg.iTaskSchedulersCount		= 1;
		appCfg.iNotifyTaskSchedulerID	= 1;
		appCfg.pPAgentHandler			= nullptr;	// this
		return true;
	}

	bool OnInitialized(int32 argc, char* argv[]) override
	{
		return true;
	}

	void OnQuitApplication( void ) override
	{
		QuitApplication(eNORMAL_EXIT);
	}
	
	bool OnRoomState( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			GameRoom::PlayerSlot& m_Players = m_GameRoom->GetPlayerSlot();
			GameRoom::SlotState& m_SlotState = m_GameRoom->GetSlotState();
			printf( "===================================\n" );
			printf( "room(%d), owner(%d), openslot(%d), players(%d)\n", m_GameRoom->m_iRoomIdx, m_GameRoom->GetOwner()->m_iAccUID, m_GameRoom->GetOpenSlotCount(), m_GameRoom->GetPlayerCount() );
			printf( "===================================\n" );
			for ( size_t i = 0; i < m_Players.size(); i++ )
			{
				printf( "slot(%d) %s : ", i, m_SlotState[i] == GameRoom::eOPENED ? "open " : "close" );
				if ( m_Players[i] )
				{
					printf( "player(%d), %s(%d)", m_Players[i]->GetID(), cstr(stat2str(m_Players[i]->m_iState)), m_Players[i]->m_iState );
				}
				else
				{
					printf( "none" );
				}
				printf( "\n" );
			}

			m_GameRoom->GetGameMap()->DebugLand();
		}
		return true;
	}

	bool OnEndGame( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			m_GameRoom->EndGame();
		}
		return true;
	}
	
	bool OnDiceCast( NetLinkPtr spLink )
	{
		PlayerPtr spPlayer = (Player*)spLink->UserData();
		if ( spPlayer )
		{
			spPlayer->DiceCast();
		}
		return true;
	}

	bool OnMovePlayer( NetLinkPtr spLink )
	{
		PlayerPtr spPlayer = (Player*)spLink->UserData();
		if ( spPlayer )
		{
			spPlayer->Move();
		}
		return true;
	}

	bool OnBuyLand( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			Player* pPlayer = m_GameRoom->GetCurrentPlayer();
			if ( pPlayer )
			{
				pPlayer->BuyEmptyLand();
			}

		}
		return true;
	}

	bool OnConstructBuilding( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			Player* pPlayer = m_GameRoom->GetCurrentPlayer();
			if ( pPlayer )
			{
				pPlayer->UpgradeBuilding();
			}
		}
		return true;
	}

	bool OnPayRoomCharge( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			Player* pPlayer = m_GameRoom->GetCurrentPlayer();
			if ( pPlayer )
			{
				pPlayer->PayRoomCharge();
			}

		}
		return true;
	}

	bool OnTakeOverLand( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			Player* pPlayer = m_GameRoom->GetCurrentPlayer();
			if ( pPlayer )
			{
				pPlayer->TakeOverLand();
			}

		}
		return true;
	}
	bool OnCloseSlot( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			m_GameRoom->CloseSlot( stoint32(sCmd[1]) );
		}
		return true;
	}

	bool OnOpenSlot( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			m_GameRoom->OpenSlot( stoint32(sCmd[1]) );
		}
		return true;
	}

	bool OnJoinRoom( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			PlayerPtr pPlayer = new Player( m_GameRoom );
			pPlayer->SetID( m_idx++ );
			m_GameRoom->Add( pPlayer );
		}
		return true;
	}

	bool OnLeaveRoom( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			m_GameRoom->Remove( stoint32(sCmd[1]) );
		}
		return true;
	}

	bool OnReadyPlayer( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			int32 iSlotNum = stoint32(sCmd[1]);
			m_GameRoom->ReadyGame( iSlotNum );

			// 게임룸에 들어온 구성원들의 정보를 브로드캐스팅
			// 방장(캐릭터id,캐릭터종류,주사위종류,전적,슬롯번호)+구성원들
		}
		return true;
	}

	bool OnStartGame( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			int32 iSlotNum = stoint32(sCmd[1]);
			m_GameRoom->StartGame( iSlotNum );
		}
		return true;
	}

	bool OnPlayGame( stringvector& sCmd )
	{
		if ( m_GameRoom )
		{
			int32 iSlotNum = stoint32(sCmd[1]);
			m_GameRoom->PlayGame( iSlotNum );
			// 선공할 플레이어를 선택해서 구성원에게 알려준다. 
		}

		return true;
	}

	void DoCommand( const std::string& sCommand )
	{

	}
};


#define CONFIG_FILE		".\\SAgent.ini"
#define SECTION_NAME	SAGENT


int _tmain(int argc, _TCHAR* argv[])
{
	RUN_SERVER_APP(Ex8Handler);
}

