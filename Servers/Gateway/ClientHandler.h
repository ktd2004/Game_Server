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
		클라가 게이트웨이에 로그인을 요청한다.
	*/
	bool		OnClientLogin( NetLinkPtr spLink, const ClientLoginData& r );
	/**
		클라가 마스터에 등록되면 호출된다.
	*/
	void		OnAuthentication( SessionPtr spSession );
	IMPORT_RPC_V1(eGTWS_CLIENT_LOGIN_SUCCESS, OnClientLoginSuccess, const ClientLoginSuccessData&, 1024);
};
