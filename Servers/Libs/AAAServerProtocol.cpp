#include "stdafx.h"
#include <Buffer.h>
#include "AAATypes.h"
#include "AAAServerProtocol.h"

Buffer& operator<<( Buffer& p, const MasterStatusData& r )
{
	p << r.m_iMID << r.m_bBlock;
	return p;
}

Buffer& operator>>(Buffer& p, MasterStatusData& r)
{
	p >> r.m_iMID >> r.m_bBlock;
	return p;
}

Buffer& operator<<(Buffer& p, const ChannelStatusData& r)
{
	p << r.m_iCID << r.m_iCurrentUser << r.m_iMaxClient << r.m_bBlock;
	return p;
}

Buffer& operator>>(Buffer& p, ChannelStatusData& r)
{
	p >> r.m_iCID >> r.m_iCurrentUser >> r.m_iMaxClient >> r.m_bBlock;
	return p;
}

Buffer& operator<<(Buffer& p, const GatewayConnData& r)
{
	p << r.m_iGID;
	return p;
}

Buffer& operator>>(Buffer& p, GatewayConnData& r)
{
	p >> r.m_iGID;
	return p;
}

Buffer& operator<<(Buffer& p, const ChannelConnData& r)
{
	p << r.m_iCID;
	return p;
}

Buffer& operator>>(Buffer& p, ChannelConnData& r)
{
	p >> r.m_iCID;
	return p;
}
