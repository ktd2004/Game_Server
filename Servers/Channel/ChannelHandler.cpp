#include "stdafx.h"
#include "ChannelHandler.h"
#include "SessionMgr.h"

ChannelHandler::ChannelHandler(uint32 iTaskID)
	: Channel2GatewayHandler(iTaskID)
{
	EXPORT_RPC_V2(3003, this, &ChannelHandler::OnTest, UID, const std::string&);
}

ChannelHandler::~ChannelHandler()
{
}


void ChannelHandler::OnLoadPlayer()
{
//	ProcessClientLogin(spLink, r);
}

bool ChannelHandler::OnTest(NetLinkPtr spLink, UID iUID, const std::string& data)
{
	prn_dbg("UID: %I64u, data: %s", iUID, cstr(data));
	return rpc_OnTestAck(spLink, iUID, "this is ack");
}

void ChannelHandler::OnDuplicatedSession( NetLinkPtr spLink, const Session& r )
{
	ProcessClientLogin( spLink, r );
}

void ChannelHandler::OnNewSession( NetLinkPtr spLink, const Session& r )
{
	ProcessClientLogin(spLink, r );
}

void ChannelHandler::OnLogoffSession(NetLinkPtr spLink, const Session& r)
{

}
