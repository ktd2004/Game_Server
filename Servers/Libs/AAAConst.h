#pragma once

#include <DataTypes.h>

namespace AAAConstDef
{
	enum eLoginResult
	{
		eLR_SUCCESS	= 0,			// ���� ����
		eLR_SYSTEM_ERROR,			// �ý��� ����
		eLR_LOAD_ERROR,				// ������ �ε� ����
		eLR_INVALID_ACCOUNT,		// ����ID �� ���� / ����ID Ȥ�� �÷��̾� ����Ÿ ��ȸ ����

		eLR_INVALID_VERSION,		// ���� ������ ������ �ƴ�. 
		eLR_INVALID_AUTHKEY,		// ����Ű�� ���� �ʴ´�.
		eLR_INVALID_SESSION,		// �������� ���� Ŭ���.
		eLR_ALREADY_AUTH_SESSION,	// ������ �������̴�.
		eLR_DUPLICATED_SESSION,		// �ߺ��α��� �̴�.
		eLR_BLOCK_ACCOUNT,			// ��ڿ� ���� ������ �� ���ߴ�.
		eLR_WITHDRAW_ACCOUNT,		// ���� Ż��
		
		eLR_MAX	= 0xff, 
	};

	enum eSessionLoginResult
	{
		eSR_SUCCESS = 0, 
		eSR_DUPLICATE,
		eSR_SYSTEM_ERROR,
		eSR_MAX = 0xFF, 
	};

	enum eSessionLogoffResult
	{
		eSL_SUCCESS = 0,
		eSL_DUPLICATE,
		eSL_INVALID_AUTHKEY,
		eSL_NOT_FOUND_UID,
		eSL_INVALID_GATEWAY,
		eSL_MAX = 0xff, 
	};

	enum eLoadBalanceResult
	{
		eLB_ALL_MASTER_BLOCKED	= -2, 
		eLB_MASTER_IS_NOT_CONNECTED	= -3, 
		eLB_ALL_CHANNEL_BLOCKED	= -4, 
	};

	enum eAAA_ObjectType
	{
		eCHANNEL_OBJ = 1,
		eMASTER_OBJ,
		eGATEWAY_OBJ,
		eSESSION_OBJ,
	};
};

using namespace AAAConstDef;
