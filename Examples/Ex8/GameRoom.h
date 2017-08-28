#pragma once

#include <DataTypes.h>
#include <Ptr.h>
#include <TaskSchedulerFactory.h>

class GameRoomMgr;

PTR(Player);
PTR(GameMap);
PTR(GameRoom);
class GameRoom : public single_ref
{
	friend class GameRoomMgr;
public:
	enum eRoomState
	{
		eONLINE,
		ePLAYING,
		eOFFLINE,
	};

	enum eSlotState
	{
		eCLOSED,
		eOPENED,
	};
	typedef std::vector<PlayerPtr>	PlayerSlot;
	typedef std::vector<int32>		SlotState;

	int32		m_iState;					/// 게임룸 상태( Online, Playing, Offline )
	PlayerSlot	m_Players;					/// 게임룸에 들어온 플레이어들
	SlotState	m_SlotState;				/// 슬롯의 상태
	GameMapPtr	m_spGameMap;				/// 게임룸의 맵
	PlayerPtr	m_spOwnerPlayer;			/// 방장
	int32		m_iRoomIdx;					/// 게임룸 번호
	DWORD		m_iTurnOverTick;			/// 플레이어 턴의 타임아웃 시간
	int32		m_iPrevPlayerIdx;			/// 이전 턴을 받은 플레이어
	int32		m_iCurrentPlayerIdx;		/// 현재 턴을 받은 플레이어
	DWORD		m_iLastTick;				/// DoAction() 호출 시간
	int32		m_iPlayerCount;				/// 게임룸에 들어온 플레이어 수
	int32		m_iOpenSlotCount;			/// 게임룸에서 입장가능한 슬롯 수
	int32		m_iVictoryPlayerIdx;		/// 승리한 플레이어
	TimerHandle m_spTimerHandle;

public:
	GameRoom();
	virtual ~GameRoom();
	
	/// 초기화
	void		OnInitialize( void );
	/// 재설정
	void		OnReset( void );
	/// 삭제
	void		OnFinalize( void );
	/// 게임룸아이디
	int32		GetID( void );
	/// 게임룸아이디설정
	void		SetID( int32 iRoomIdx );
	/// 방장설정
	void		SetOwner( PlayerPtr spPlayer );
	/// 방장조회
	PlayerPtr	GetOwner( void );
	/// 슬롯 열기
	void		OpenSlot( int32 iSlotNum );
	/// 슬롯 닫기
	void		CloseSlot( int32 iSlotNum );
	/// 
	int32		GetOpenSlotCount( void );
	/// 게임룸에 들어온 플레어이서
	int32		GetPlayerCount( void );

	/// 플레이어추가
	int32		Add( PlayerPtr spPlayer );
	/// 플레이어삭제
	int32		Remove( PlayerPtr spPlayer );
	/// 플레이어삭제
	int32		Remove( int32 iSlotNum );
	/// 게임맵
	GameMap*	GetGameMap( void );
	/// 선을 선택한다.
	int32		RandomSelectPlayer( void );
	/// 현재플레이어
	Player*		GetCurrentPlayer( void );
	/// 다음플레이어
	Player*		MoveNextPlayer( void );
	/// 플레이어 슬롯
	PlayerSlot&	GetPlayerSlot( void );
	/// 플레이어 슬롯상태
	SlotState&	GetSlotState( void );
	/// 게임준비
	void		ReadyGame( int32 iSlotNum );
	/// 게임시작
	bool		StartGame( int32 iSlotNum );
	/// 게임진행
	bool		PlayGame( int32 iSlotNum );
	/// 게임을 종료
	void		EndGame( void );
	/// 승리조건 체크
	bool		CheckVictory( void );
	/// 승리한 플레이어
	Player*		GetVictoryPlayer( void );
	/// 타이머 시작
	void		CreateTimer( uint32 iMSec );
	bool		OnTimeout( void* pUserData );
	void		CancelTimer( void );
};


class GameRoomMgr
{
public:
	typedef std::map<int32, GameRoomPtr>	GameRoomMap;
	GameRoomMap		m_OnlineRoomMap;
	GameRoomMap		m_OnPlayRoomMap;
	GameRoomMap		m_OfflineRoomMap;
	int32			m_iRoomIdx;

protected:
	GameRoomMgr();
	GameRoomMgr( const GameRoomMgr& copy );
	virtual ~GameRoomMgr();
public:
	static GameRoomMgr* instance();

	GameRoomPtr	Create( void );
	void		SetPlayRoom( GameRoomPtr spGameRoom );
	void		SetWaitRoom( GameRoomPtr spGameRoom );
	void		Delete( GameRoomPtr spGameRoom );
};