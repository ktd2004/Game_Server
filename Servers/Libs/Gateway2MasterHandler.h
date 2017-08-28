#pragma once

#include <Proactor.h>
#include "AAA2ClientProtocol.h"
#include "AAAServerProtocol.h"

PTR(Session)

class Gateway2ClientHandler;
class Gateway2ChannelHandler;

/** 
	Gateway���� Master������ ����� ���� ����ϴ� ��Ʈ�� �̺�Ʈ �ڵ鷯
*/
class Gateway2MasterHandler : public NetEventHandler
{
	friend class Gateway2ClientHandler;
protected:
	GID						m_iGID;				///< ����Ʈ���̾��̵�
	Gateway2ClientHandler*	m_pClientHandler;
	Gateway2ChannelHandler* m_pChannelHandler;
	uint32					m_iPriority;
	uint16					m_iThreadCnt;
	std::string				m_sAddr;
	uint16					m_iPort;

	typedef std::map<std::string, uint32> ConnectionMap;
	ConnectionMap m_ConnMap;

public:
	Gateway2MasterHandler( uint32 iTaskID );
	virtual ~Gateway2MasterHandler();

	bool	Initialize();
	void	SetClientHandler( Gateway2ClientHandler* pHandler ) { m_pClientHandler = pHandler; }
	void	SetChannelHandler(Gateway2ChannelHandler* pHandler) { m_pChannelHandler = pHandler; }
	bool	StartupMasterHandler();

private:
	// connect master
	bool			OnConnected( NetLinkPtr spLink );
	// disconnect master
	bool			OnClosed( NetLinkPtr spLink );
	/**
		�����ͼ����� ����Ʈ���� ������ �����Ѵ�.
	*/
	IMPORT_RPC_NM_V1(OnGatewayConnData, const GatewayConnData&, 1024);
	/**
		�����ͼ����� �α����� ��û�Ѵ�.
	*/
	IMPORT_RPC_NM_V1( OnClientLogin, const Session&, 1024 );
	bool	OnClientLogin( SessionPtr spSession );
	/*
		������ ������ �α׿��� ��Ű��� ��û�Ѵ�.
	*/
	IMPORT_RPC_NM_V3( OnClientLogoff, CID, UID, uint32, 1024 );		// cid, uid, authkey
	bool	OnClientLogoff( MID iMasterID, CID iChannelID, UID iUID, uint32 iAuthKey );

	// from master
	bool	OnMasterStatus( NetLinkPtr spLink, const MasterStatusData& r );
	bool	OnClientLoginResult( NetLinkPtr spLink, int32 iResult, UID iUID );
	bool	OnClientLogoffResult( NetLinkPtr spLink, int32 iResult, CID iChannelID, UID iUID, uint32 iAuthKey );
};
