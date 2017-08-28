#pragma once

#include <Proactor.h>
#include "AAA2ClientProtocol.h"
#include "AAAServerProtocol.h"

PTR(Session);
class Gateway2ClientHandler;
class Gateway2MasterHandler;
class Gateway2ChannelHandler : public NetEventHandler
{
	friend class Gateway2ClientHandler;
	friend class Gateway2MasterHandler;
protected:
	Gateway2ClientHandler*	m_pClientHandler;
	GID						m_iGID;
	uint32					m_iPriority;
	uint16					m_iThreadCnt;
	std::string				m_sAddr;
	uint16					m_iPort;

	typedef std::map<std::string, uint32> ConnectionMap;
	ConnectionMap m_ConnMap;

public:
	Gateway2ChannelHandler( uint32 iTaskID );
	virtual ~Gateway2ChannelHandler();

	bool	Initialize();
	bool	StartupChannelHandler();
	void	SetClientHandler(Gateway2ClientHandler* pHandler) { m_pClientHandler = pHandler; }

private:
	// connect channel
	bool	OnConnected( NetLinkPtr spLink );
	// disconnect channel
	bool	OnClosed( NetLinkPtr spLink );
	/**
		ä�μ������� ��Ŷ �����ϸ� ȣ��
	*/
	bool	OnReceived( NetLinkPtr spLink, Buffer* pBuffer );
	/**
		ä�μ����� ����Ʈ���� ������ �����Ѵ�.
	*/
	IMPORT_RPC_NM_V1(OnGatewayConnData, const GatewayConnData&, 1024);
	/**
		ä�μ����� �α����� ��û�Ѵ�.
	*/
	IMPORT_RPC_NM_V1(OnClientLogin, const Session&, 1024);
	bool	OnClientLogin( SessionPtr spSession );
	/*
		ä�μ����� �α׿��� ��Ű��� ��û�Ѵ�.
	*/
	IMPORT_RPC_NM_V2(OnClientLogoff, UID, uint32, 1024);	// uid, authkey
	bool	OnClientLogoff( CID iChannelID, UID iUID, uint32 iAuthKey );

	// from channel
	bool	OnChannelStatus( NetLinkPtr spLink, const ChannelStatusData& r );
	bool	OnClientLoginResult( NetLinkPtr spLink, int32 iResult, UID iUID );
	bool	OnClientLogoffResult( NetLinkPtr spLink, int32 iResult, UID iUID, uint32 iAuthKey );
};
