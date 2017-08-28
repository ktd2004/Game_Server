#pragma once

#include "AAATypes.h"
#include <Buffer.h>

////////////////////////////////////////////////////////////////////////////////
class MasterStatusData
{
public:
	MID			m_iMID;
	bool		m_bBlock;
	MasterStatusData()
		: m_iMID(0)
		, m_bBlock(false)
	{}
};

Buffer& operator<<( Buffer& p, const MasterStatusData& r );
Buffer& operator>>( Buffer& p, MasterStatusData& r );

////////////////////////////////////////////////////////////////////////////////
class ChannelStatusData
{
public:
	CID			m_iCID;
	uint32		m_iCurrentUser;
	uint32		m_iMaxClient;
	bool		m_bBlock;

	ChannelStatusData()
		: m_iCID(0)
		, m_iCurrentUser(0)
		, m_iMaxClient(0)
		, m_bBlock(false)
	{}
};

Buffer& operator<<(Buffer& p, const ChannelStatusData& r);
Buffer& operator>>(Buffer& p, ChannelStatusData& r);

////////////////////////////////////////////////////////////////////////////////
class GatewayConnData
{
public:
	GID		m_iGID;

	GatewayConnData()
		: m_iGID(null_id)
	{}
};

Buffer& operator<<(Buffer& p, const GatewayConnData& r);
Buffer& operator>>(Buffer& p, GatewayConnData& r);

////////////////////////////////////////////////////////////////////////////////
class ChannelConnData
{
public:
	CID		m_iCID;

	ChannelConnData()
		: m_iCID(null_id)
	{}
};

Buffer& operator<<(Buffer& p, const ChannelConnData& r);
Buffer& operator>>(Buffer& p, ChannelConnData& r);