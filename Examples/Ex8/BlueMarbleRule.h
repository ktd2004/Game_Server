#pragma once


enum eBuleMarbleState
{
	eNONE_STATE,
	eREADY_GAME,
	ePLAY_GAME,
	eKONKRUS,					/// �Ļ� ����
	eDICE_CAST,					/// �ֻ��� ������
	eMOVE_PLAYER,				/// �÷��̾� �̵�
	eWAIT_UPGRADE_BUILDING,		/// �ڽ��� ������ �ǹ��� Upgrade �Ҳ��� ?
	eWAIT_BUY_LAND,				/// ����ִ� ������ �μ��Ҳ��� ?
	eWAIT_TAKE_OVER,			/// �ٸ� �÷��̾� �������� �μ��Ҳ��� ?
	eWAIT_CONFIRM_KONKRUS,		/// �Ļ��Ҳ��� �ƴ� �ڽ��� �Ű��Ҳ��� ?
	eWAIT_PAY_ROOMCHARGE,		/// ���ڷḦ �����Ҳ��� ?
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