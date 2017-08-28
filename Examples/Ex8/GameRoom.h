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

	int32		m_iState;					/// ���ӷ� ����( Online, Playing, Offline )
	PlayerSlot	m_Players;					/// ���ӷ뿡 ���� �÷��̾��
	SlotState	m_SlotState;				/// ������ ����
	GameMapPtr	m_spGameMap;				/// ���ӷ��� ��
	PlayerPtr	m_spOwnerPlayer;			/// ����
	int32		m_iRoomIdx;					/// ���ӷ� ��ȣ
	DWORD		m_iTurnOverTick;			/// �÷��̾� ���� Ÿ�Ӿƿ� �ð�
	int32		m_iPrevPlayerIdx;			/// ���� ���� ���� �÷��̾�
	int32		m_iCurrentPlayerIdx;		/// ���� ���� ���� �÷��̾�
	DWORD		m_iLastTick;				/// DoAction() ȣ�� �ð�
	int32		m_iPlayerCount;				/// ���ӷ뿡 ���� �÷��̾� ��
	int32		m_iOpenSlotCount;			/// ���ӷ뿡�� ���尡���� ���� ��
	int32		m_iVictoryPlayerIdx;		/// �¸��� �÷��̾�
	TimerHandle m_spTimerHandle;

public:
	GameRoom();
	virtual ~GameRoom();
	
	/// �ʱ�ȭ
	void		OnInitialize( void );
	/// �缳��
	void		OnReset( void );
	/// ����
	void		OnFinalize( void );
	/// ���ӷ���̵�
	int32		GetID( void );
	/// ���ӷ���̵���
	void		SetID( int32 iRoomIdx );
	/// ���弳��
	void		SetOwner( PlayerPtr spPlayer );
	/// ������ȸ
	PlayerPtr	GetOwner( void );
	/// ���� ����
	void		OpenSlot( int32 iSlotNum );
	/// ���� �ݱ�
	void		CloseSlot( int32 iSlotNum );
	/// 
	int32		GetOpenSlotCount( void );
	/// ���ӷ뿡 ���� �÷����̼�
	int32		GetPlayerCount( void );

	/// �÷��̾��߰�
	int32		Add( PlayerPtr spPlayer );
	/// �÷��̾����
	int32		Remove( PlayerPtr spPlayer );
	/// �÷��̾����
	int32		Remove( int32 iSlotNum );
	/// ���Ӹ�
	GameMap*	GetGameMap( void );
	/// ���� �����Ѵ�.
	int32		RandomSelectPlayer( void );
	/// �����÷��̾�
	Player*		GetCurrentPlayer( void );
	/// �����÷��̾�
	Player*		MoveNextPlayer( void );
	/// �÷��̾� ����
	PlayerSlot&	GetPlayerSlot( void );
	/// �÷��̾� ���Ի���
	SlotState&	GetSlotState( void );
	/// �����غ�
	void		ReadyGame( int32 iSlotNum );
	/// ���ӽ���
	bool		StartGame( int32 iSlotNum );
	/// ��������
	bool		PlayGame( int32 iSlotNum );
	/// ������ ����
	void		EndGame( void );
	/// �¸����� üũ
	bool		CheckVictory( void );
	/// �¸��� �÷��̾�
	Player*		GetVictoryPlayer( void );
	/// Ÿ�̸� ����
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