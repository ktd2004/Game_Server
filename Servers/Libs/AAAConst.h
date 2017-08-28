#pragma once

#include <DataTypes.h>

namespace AAAConstDef
{
	enum eLoginResult
	{
		eLR_SUCCESS	= 0,			// 인증 성공
		eLR_SYSTEM_ERROR,			// 시스템 에러
		eLR_LOAD_ERROR,				// 데이터 로드 에러
		eLR_INVALID_ACCOUNT,		// 계정ID 가 오류 / 계정ID 혹은 플레이어 데이타 조회 실패

		eLR_INVALID_VERSION,		// 접속 가능한 버전이 아님. 
		eLR_INVALID_AUTHKEY,		// 인증키가 맞지 않는다.
		eLR_INVALID_SESSION,		// 인증되지 않은 클라다.
		eLR_ALREADY_AUTH_SESSION,	// 인증이 진행중이다.
		eLR_DUPLICATED_SESSION,		// 중복로그인 이다.
		eLR_BLOCK_ACCOUNT,			// 운영자에 의해 계정이 블럭 당했다.
		eLR_WITHDRAW_ACCOUNT,		// 계정 탈퇴
		
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
