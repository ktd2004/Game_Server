#pragma once

#include "Gateway2ClientHandler.h"
#include "GatewayClientProtocol.h"

class IDBProvider;
class SelectAccount;
class ClientHandler : public Gateway2ClientHandler
{
private:
	IDBProvider*			m_pProvider;

public:
	ClientHandler( uint32 iTaskID );
	virtual ~ClientHandler();

	void		SetDBProvider( IDBProvider* pProvider ) { m_pProvider = pProvider; }
	bool		ExecSelectAccountQuery( SessionPtr spSession );
	void		OnSelectAccount( NetLinkPtr spLink, SelectAccount* pQry );

	/**
		Ŭ�� ����Ʈ���̿� �α����� ��û�Ѵ�.
	*/
	bool		OnClientLogin( NetLinkPtr spLink, const ClientLoginData& r );
	/**
		Ŭ�� �����Ϳ� ��ϵǸ� ȣ��ȴ�.
	*/
	void		OnAuthentication( SessionPtr spSession );
	IMPORT_RPC_V1(eGTWS_CLIENT_LOGIN_SUCCESS, OnClientLoginSuccess, const ClientLoginSuccessData&, 1024);
};
