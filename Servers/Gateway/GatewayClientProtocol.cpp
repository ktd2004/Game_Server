#include "stdafx.h"
#include "GatewayClientProtocol.h"

Buffer& operator<<( Buffer& p, const ClientLoginSuccessData& r )
{
	p << r.m_iCID << r.m_iAuthKey;
	return p;
}

Buffer& operator<<( Buffer& p, const ClientLoginData& r )
{
	p << r.m_iUID << r.m_sAccount << r.m_sVersion;
	return p;
}

Buffer& operator>>( Buffer& p, ClientLoginSuccessData& r )
{
	p >> r.m_iCID >> r.m_iAuthKey;
	return p;
}

Buffer& operator>>( Buffer& p, ClientLoginData& r )
{
	p >> r.m_iUID >> r.m_sAccount >> r.m_sVersion;
	return p;
}
