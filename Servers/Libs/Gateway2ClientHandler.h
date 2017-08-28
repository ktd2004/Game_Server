#pragma once

#include <Proactor.h>
#include "AAATypes.h"
#include "VersionNum.h"
#include "SessionMgr.h"
#include "AAA2ClientProtocol.h"

PTR(Session);

class IDBProvider;
class Version;
class Gateway2MasterHandler;
/**
	����Ʈ���̿��� Ŭ���̾�Ʈ�� ����ϱ� ���� ���Ǵ� ��Ʈ�� �̺�Ʈ �ڵ鷯.
*/
class Gateway2ClientHandler : public NetEventHandler
{
	friend class Gateway2MasterHandler;
	friend class Gateway2ChannelHandler;
protected:
	Version					m_MinVersion;		//< ���� ������ Ŭ���̾�Ʈ �ּ� ����
	Gateway2MasterHandler*	m_pMasterHandler;
	MID						m_iMID;
	GID						m_iGID;
	uint32					m_iPriority;
	uint16					m_iThreadCnt;
	std::string				m_sAddr;
	uint16					m_iPort;

public:
	Gateway2ClientHandler( uint32 iTaskID );
	virtual ~Gateway2ClientHandler();

	bool			Initialize();
	bool			StartupClientHandler();
	void			SetMasterHandler( Gateway2MasterHandler* pHandler ) { m_pMasterHandler = pHandler; }
	virtual void	SetDBProvider( IDBProvider* pProvider ) {}

	/**
		Ŭ�󿡰� ���� �޼����� �������� ű�Ѵ�.
	*/
	void			ProcessClientError(SessionPtr spSession, int32 eResult, int32 eKick, const char* szFunc);
	/**
		Ŭ�󿡰� ���� �޼����� �������� ű�Ѵ�.
	*/
	void			ProcessClientError(NetLinkPtr spLink, int32 eResult, int32 eKick, const char* szFunc);
	/**
		Ŭ�� �α׿��� ��Ű���� �����Ϳ� ��û�Ѵ�.
	*/
	bool			ProcessClientLogoff(SessionPtr spSession);
	/**
		Ŭ���� �α��� ������ �����ͷ� �˸���.
	*/
	bool			ProcessClientLogin(NetLinkPtr spLink, UID iUID, const std::string& sAccount);
	/*
		Ŭ�� ������ �����Ϳ� ��ϵ��� ȣ��ȴ�.
	*/
	virtual void	OnAuthentication(SessionPtr spSession) {};
	
private:
	/** 
		Ŭ�� ���������� ����
	*/
	bool			OnAccepted( NetLinkPtr spLink );
	/** 
		Ŭ�� ���������� ������ ����
	*/
	bool			OnClosed( NetLinkPtr spLink );

	// to client
	IMPORT_RPC_V1(eGTWS_CLIENT_LOGIN_ERROR,   ErrorToClient,   int32, 1024);

	// from client
	bool		OnReceived(NetLinkPtr spLink, Buffer* pBuffer);
};
