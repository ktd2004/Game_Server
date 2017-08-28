#pragma once


enum eBuleMarbleState
{
	eNONE_STATE,
	eREADY_GAME,
	ePLAY_GAME,
	eKONKRUS,					/// 파산 상태
	eDICE_CAST,					/// 주사위 굴리기
	eMOVE_PLAYER,				/// 플레이어 이동
	eWAIT_UPGRADE_BUILDING,		/// 자신의 토지에 건물을 Upgrade 할꺼냐 ?
	eWAIT_BUY_LAND,				/// 비어있는 토지를 인수할꺼냐 ?
	eWAIT_TAKE_OVER,			/// 다른 플레이어 소유지를 인수할꺼냐 ?
	eWAIT_CONFIRM_KONKRUS,		/// 파산할꺼냐 아님 자신을 매각할꺼냐 ?
	eWAIT_PAY_ROOMCHARGE,		/// 숙박료를 지불할꺼냐 ?
};

inline std::string stat2str( int32 iState )
{
	char* p[] = {
		"eNONE_STATE",
		"eREADY_GAME",
		"ePLAY_GAME",
		"eKONKRUS",
		"eWAIT_DICE_CAST",
		"eWAIT_MOVE_PLAYER",
		"eWAIT_UPGRADE_BUILDING",
		"eWAIT_BUY_LAND",
		"eWAIT_TAKE_OVER",
		"eWAIT_CONFIRM_KONKRUS",
		"eWAIT_PAY_ROOMCHARGE",
		NULL
	};

	if ( sizeof(p)/sizeof(char*) < iState || iState < 0 )
	{
		return std::string();
	}

	return std::string( p[iState] );
}