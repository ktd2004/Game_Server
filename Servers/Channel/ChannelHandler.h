#pragma once

#include <Channel2GatewayHandler.h>
#include "ClientProtocol.h"

class ChannelHandler : public Channel2GatewayHandler
{
public:
	ChannelHandler(uint32 iTaskID);
	virtual ~ChannelHandler();

	void	OnDuplicatedSession( NetLinkPtr spLink, const Session& r );
	void	OnNewSession( NetLinkPtr spLink, const Session& r );
	void	OnLogoffSession( NetLinkPtr spLink, const Session& r );

	void	OnLoadPlayer();

	// from client
	bool		OnTest( NetLinkPtr spLink, UID iUID, const std::string& data );
	IMPORT_RPC_V2(3004, OnTestAck, UID, const std::string&, 1024);
};
