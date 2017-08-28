#pragma once

#include "AAATypes.h"
#include "AAAConst.h"
#include <Buffer.h>

////////////////////////////////////////////////////////////////////////////////
class ClientLoginSuccessData
{
public:
	CID				m_iCID;
	uint32			m_iAuthKey;

	ClientLoginSuccessData()
		: m_iCID(0)
		, m_iAuthKey(0) {}
};

Buffer& operator<<( Buffer& p, const ClientLoginSuccessData& r );
Buffer& operator>>( Buffer& p, ClientLoginSuccessData& r );

////////////////////////////////////////////////////////////////////////////////
class ClientLoginData
{
public:
	UID				m_iUID;
	std::string		m_sAccount;
	std::string		m_sVersion;

	ClientLoginData()
		: m_iUID(0)
		, m_sAccount("")
		, m_sVersion("0.0.0")
	{}
};

Buffer& operator<<( Buffer& p, const ClientLoginData& r );
Buffer& operator>>( Buffer& p, ClientLoginData& r );