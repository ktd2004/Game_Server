#pragma once

#include <AAATypes.h>


enum eSelectCharacterRst
{
	eSC_SUCCESS = 0, 
	eSC_INVALID_STATUS,		// 캐릭터 선택을 할 수 있는 상태가 아님
	eSC_INVALID_ID,			// 없는 캐릭터를 선택
	eSC_SYSTEM_ERR,			// 기타 에러
};

enum eCreateRoomRst
{
	eCR_SUCCESS = 0, 
	eCR_INVALID_MAP,		// 없는 지도 데이터를 사용
	eCR_INVALID_STAGE,		// 없는 스테이지
	eCR_INVALID_TYPE,		// 없는 지도 타입 생성
	eCR_INVALID_STATUS,		// 방을 만들 수 없는 상태 (이미 방에 입장해 있음, 게임 중임 등)
	eCR_GET_ROOM_NO_ERR,	// 방 번호 얻어오기 실패
	eCR_ROOM_START_FAILED,	// 방 시작 실패
	eCR_SYSTEM_ERR,			// 기타 에러
};

enum eRemoveRoomRst
{
	eRR_SUCCESS = 0, 
	eRR_INVALID_STATUS,		// 방을 떠날 수 없는 상태
	eRR_NOT_FOUND,			// 방을 찾지 못함
	eRR_SYSTEM_ERR,			// 기타 에러
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
	eFS_REQUEST,	// 요청
	eFS_REQUESTED,	// 요청 받음
	eFS_REFUSE,		// 거절
	eFS_REFUSED,	// 거절 당함
	eFS_PLATFORM_BLOCK,		// 플랫폼 친구 차단
	eFS_PLATFORM_BLOCKED,	// 플랫폼 친구로부터 차단 당함
};

enum eFriendAction
{
	eFA_REQUEST = 1, 	// 친구 요청
	eFA_REMOVE,			// 친구 삭제
	eFA_REFUSE,			// 친구 요청 거절
	eFA_ACCEPT,			// 친구 요청 수락
	eFA_CANCEL_REQUEST, // 친구 요청 취소
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
	eFR_EXIST_GAME,			// 이미 게임 친구
	eFR_EXIST_PLATFORM,		// 이미 플랫폼 친구
	eFR_NOT_FOUND,			// 찾지 못함
	eFR_REQEUST,			// 이미 요청한 친구
	eFR_REQUESTED,			// 이미 요청 받은 친구
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

const int32	MAX_NPC = 3;			// 게임에 세팅할 수 있는 최대 AI 수
const int32 MAX_DECK = 6;			// 게임 내 최대 몬스터 덱 수
const UID	AI_UID = 0;				// AI로 할당되는 UID

const uint8 TEAM_PROVIDE_MONSTER = 1;		// 유저 팀에서 세팅된 몬스터
const uint8 SYSTEM_PROVIDE_MONSTER = 2;		// 시스템에서 지급된 몬스터
const uint8 FRIEND_MONSTER = 3;				// 친구 몬스터

const int8 MAX_DICE_NUMBER = 6;		// 주사위 최대 값
const uint16 MAX_LAND_LEVEL = 5;	// 부지 최대 레벨

const uint16 DECK_FRIEND = 0;		// 친구 덱 인덱스
const uint16 DECK_ERROR = 65000;

// timer duration
const int32 TIMER_SESSION_WAIT = 1000 * 5000;
const int32 TIMER_SELL_LAND_WAIT = 1000 * 10;

const uint32 START_SINGLE_MANA = 400;

// 친구 관련 정의
const int32 MAX_RECOMMEND_FRIEND_REQ = 10;	// 한 번에 요청 가능한 추천 친구 수
const uint8 MAX_DEL_FRIEND = 10;			// 하루에 삭제 가능한 친구 수
const int32 MAX_SEND_FRIEND = 10;			// 한 번의 패킷에 내려 줄 친구 수
const int32 MAX_GAME_FRIEND = 50;			// 최대 게임 친구 수
const int32 MAX_REQUEST_FRIEND = 20;		// 친구 요청
const int32 MAX_REQUESTED_FRIEND = 20;		// 수락 대기

// 몬스터 관련 정의
const int32 MAX_MONSTER = 99;		// 최대 몬스터 수
const int32 MAX_MONSTER_RARE = 6;	// 몬스터 최대 등급 (1 ~ 6)

// 우편 관련 정의
const int32 MAX_POST_COUNT = 50;		// 우편 최대 수
const uint32 POST_EXPIRE_TIME = 259200;	// 우편 유효 시간 (3일)
const uint32 DAILY_HEART = 2;			// 일일 하트 선물
