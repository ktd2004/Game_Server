#pragma once

#include <AAATypes.h>


enum eSelectCharacterRst
{
	eSC_SUCCESS = 0, 
	eSC_INVALID_STATUS,		// ĳ���� ������ �� �� �ִ� ���°� �ƴ�
	eSC_INVALID_ID,			// ���� ĳ���͸� ����
	eSC_SYSTEM_ERR,			// ��Ÿ ����
};

enum eCreateRoomRst
{
	eCR_SUCCESS = 0, 
	eCR_INVALID_MAP,		// ���� ���� �����͸� ���
	eCR_INVALID_STAGE,		// ���� ��������
	eCR_INVALID_TYPE,		// ���� ���� Ÿ�� ����
	eCR_INVALID_STATUS,		// ���� ���� �� ���� ���� (�̹� �濡 ������ ����, ���� ���� ��)
	eCR_GET_ROOM_NO_ERR,	// �� ��ȣ ������ ����
	eCR_ROOM_START_FAILED,	// �� ���� ����
	eCR_SYSTEM_ERR,			// ��Ÿ ����
};

enum eRemoveRoomRst
{
	eRR_SUCCESS = 0, 
	eRR_INVALID_STATUS,		// ���� ���� �� ���� ����
	eRR_NOT_FOUND,			// ���� ã�� ����
	eRR_SYSTEM_ERR,			// ��Ÿ ����
};

enum eRoomStartRst
{
	eRS_SUCCESS = 0, 
	eRS_FAILED, 
};

enum eUserType
{
	eSession_Player	= 1, 
	eSession_AI		= 2, 
};

/*
enum eLandAction
{
	eLA_PlaceMonster = 1, 
	eLA_UseItem, 
	eLA_Battle, 
	eLA_Pass, 
};
*/

enum eLandActionResult
{
	eLA_SUCCESS = 0, 
	eLA_NOT_ENOUGH_MANA, 
	eLA_SYSTEM_ERR, 
};

enum eFriendStatus
{
	eFS_NONE = 0, 
	eFS_FRIEND, 
	eFS_PLATFORM_FRIEND, 
	eFS_REQUEST,	// ��û
	eFS_REQUESTED,	// ��û ����
	eFS_REFUSE,		// ����
	eFS_REFUSED,	// ���� ����
	eFS_PLATFORM_BLOCK,		// �÷��� ģ�� ����
	eFS_PLATFORM_BLOCKED,	// �÷��� ģ���κ��� ���� ����
};

enum eFriendAction
{
	eFA_REQUEST = 1, 	// ģ�� ��û
	eFA_REMOVE,			// ģ�� ����
	eFA_REFUSE,			// ģ�� ��û ����
	eFA_ACCEPT,			// ģ�� ��û ����
	eFA_CANCEL_REQUEST, // ģ�� ��û ���
};

enum eFriendActionQueryResult
{
	eFQ_SUCCESS = 0, 
	eFQ_MAX_COUNT_GAME_FRIEND, 
	eFQ_MAX_COUNT_REQUESTED_FRIEND, 
	eFQ_INVALID_CASE, 
};

enum eFriendActionResult
{
	eFAR_SUCCESS = 0, 
	eFAR_NOT_FOUND, 
	eFAR_ALREADY_FRIEND, 
	eFAR_ALREADY_REQUEST, 
	eFAR_OVER_MAX_GAME_FRIEND, 
	eFAR_OVER_MAX_REQUEST_FRIEND, 
	eFAR_NO_LEFT_DEL_COUNT, 
	eFAR_OVER_MAX_GAME_FRIEND_BY_FRIEND, 
	eFAR_OVER_MAX_REQUEST_FRIEND_BY_FRIEND, 
	eFAR_INVALID_ACTION, 
	eFAR_SYSTEM_ERR, 
};

enum eFriendSearchResult
{
	eFR_SUCCESS = 0, 
	eFR_EXIST_GAME,			// �̹� ���� ģ��
	eFR_EXIST_PLATFORM,		// �̹� �÷��� ģ��
	eFR_NOT_FOUND,			// ã�� ����
	eFR_REQEUST,			// �̹� ��û�� ģ��
	eFR_REQUESTED,			// �̹� ��û ���� ģ��
	eFR_SYSTEM_ERR, 
};

enum eBuyCharacterRst
{
	eBC_SUCCESS = 0, 
	eBC_EXIST_CHARACTER, 
	eBC_NOT_ENOUGH_MONEY, 
	eBC_SYSTEM_ERR, 
};

enum ePullMonsterRst
{
	ePM_SUCCESS = 0, 
	ePM_INVALID_PULL, 
	ePM_NOT_ENOUGH_MONEY, 
	ePM_MAX_MONSTER, 
	ePM_SYSTEM_ERR, 
};

enum eBuyDiceRst
{
	eBD_SUCCESS = 0, 
	eBD_EXIST_DICE, 
	eBD_NOT_ENOUGH_MONEY, 
	eBD_SYSTEM_ERR, 
};

enum eBuyItemRst
{
	eBI_SUCCESS = 0, 
	eBI_MAX_INVENTORY, 
	eBI_OVER_QTY, 
	eBI_INVALID_ITEM, 
	eBI_NOT_ENOUGH_MONEY, 
	eBI_SYSTEM_ERR, 
};

enum eChangeCharacterRst
{
	eCCR_SUCCESS = 0, 
	eCCR_INVALID_CID, 
	eCCR_SYSTEM_ERR, 
};

enum eChangeDiceRst
{
	eCDR_SUCCESS = 0, 
	eCDR_INVALID_DICE, 
	eCDR_SYSTEM_ERR, 
};

enum eChangeLeaderMonsterRst
{
	eCLM_SUCCESS = 0, 
	eCLM_INVALID_UNIQUE_ID, 
	eCLM_SYSTEM_ERR, 
};

enum eAddTeamMonsterRst
{
	eATM_SUCCESS = 0, 
	eATM_INVALID_UNIQUE_ID, 
	eATM_TEAM_NO_ERR, 
	eATM_SYSTEM_ERR, 
};

enum eRemoveTeamMonsterRst
{
	eRTM_SUCCESS = 0, 
	eRTM_INVALID_UNIQUE_ID, 
	eRTM_TEAM_NO_ERR, 
	eRTM_ORDER_ERR, 
	eRTM_SYSTEM_ERR, 
};

enum eExtendEquipRst
{
	eEER_SUCCESS = 0, 
	eEER_MAX_EQUIP, 
	eEER_NOT_ENOUGH_MONEY, 
	eEER_SYSTEM_ERR, 
};

enum eAttachEquipRst
{
	eAER_SUCCESS = 0, 
	eAER_INVALID_ORDER, 
	eAER_INVALID_ITEM, 
	eAER_EXIST_ATTACH, 
	eAER_SYSTEM_ERR, 
};

enum eDetachEquipRst
{
	eDER_SCCESS = 0, 
	eDER_INVALID_ORDER, 
	eDER_SYSTEM_ERR, 
};

enum ePostSendType
{
	ePostSend_None	= 0, 
	ePostSend_Admin = 1, 
	ePostSend_User	= 2, 
};

enum ePostStatus
{
	ePostStatus_NoRead	= 0, 
	ePostStatus_Readed	= 1, 
	ePostStatus_Expired	= 2, 
};

enum ePostType
{
	ePostType_None			= 0, 
	ePostType_Gold			= 1, 
	ePostType_Heart			= 2, 
	ePostType_Cash			= 3, 
	ePostType_Key			= 4, 
	ePostType_Item			= 5, 
	ePostType_DailyHeart	= 6, 
};

enum ePostRecvRst
{
	ePRR_SUCCESS = 0, 
	ePRR_SYSTEM_ERR, 
};

enum ePostSendDailyHeartRst
{
	ePSD_SUCCESS = 0, 
	ePSD_NOT_FRIEND, 
	ePSD_INVALID_DATE, 
	ePSD_SYSTEM_ERR, 
};

const int32	MAX_NPC = 3;			// ���ӿ� ������ �� �ִ� �ִ� AI ��
const int32 MAX_DECK = 6;			// ���� �� �ִ� ���� �� ��
const UID	AI_UID = 0;				// AI�� �Ҵ�Ǵ� UID

const uint8 TEAM_PROVIDE_MONSTER = 1;		// ���� ������ ���õ� ����
const uint8 SYSTEM_PROVIDE_MONSTER = 2;		// �ý��ۿ��� ���޵� ����
const uint8 FRIEND_MONSTER = 3;				// ģ�� ����

const int8 MAX_DICE_NUMBER = 6;		// �ֻ��� �ִ� ��
const uint16 MAX_LAND_LEVEL = 5;	// ���� �ִ� ����

const uint16 DECK_FRIEND = 0;		// ģ�� �� �ε���
const uint16 DECK_ERROR = 65000;

// timer duration
const int32 TIMER_SESSION_WAIT = 1000 * 5000;
const int32 TIMER_SELL_LAND_WAIT = 1000 * 10;

const uint32 START_SINGLE_MANA = 400;

// ģ�� ���� ����
const int32 MAX_RECOMMEND_FRIEND_REQ = 10;	// �� ���� ��û ������ ��õ ģ�� ��
const uint8 MAX_DEL_FRIEND = 10;			// �Ϸ翡 ���� ������ ģ�� ��
const int32 MAX_SEND_FRIEND = 10;			// �� ���� ��Ŷ�� ���� �� ģ�� ��
const int32 MAX_GAME_FRIEND = 50;			// �ִ� ���� ģ�� ��
const int32 MAX_REQUEST_FRIEND = 20;		// ģ�� ��û
const int32 MAX_REQUESTED_FRIEND = 20;		// ���� ���

// ���� ���� ����
const int32 MAX_MONSTER = 99;		// �ִ� ���� ��
const int32 MAX_MONSTER_RARE = 6;	// ���� �ִ� ��� (1 ~ 6)

// ���� ���� ����
const int32 MAX_POST_COUNT = 50;		// ���� �ִ� ��
const uint32 POST_EXPIRE_TIME = 259200;	// ���� ��ȿ �ð� (3��)
const uint32 DAILY_HEART = 2;			// ���� ��Ʈ ����
