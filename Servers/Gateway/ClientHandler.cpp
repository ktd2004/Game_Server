#include "stdafx.h"
#include <Application.h>
#include <ChannelMgr.h>
#include "Query/SelectQuery.h"
#include "ClientHandler.h"


ClientHandler::ClientHandler( uint32 iTaskID )
	: Gateway2ClientHandler( iTaskID )
	, m_pProvider(nullptr)
{
	EXPORT_RPC_V1(eCLIENT_GTWS_LOGIN, this, &ClientHandler::OnClientLogin, const ClientLoginData&);
}

ClientHandler::~ClientHandler()
{
}

bool ClientHandler::ExecSelectAccountQuery(SessionPtr spSession)
{
	if (!m_pProvider)
	{
		return false;
	}

	SelectAccount* pQry = new SelectAccount(spSession);
	pQry->SetFn(this, &ClientHandler::OnSelectAccount);
	int32 iRst = m_pProvider->Push(pQry);
	if (!iRst)
	{
		prn_err("Query failed, UID(%I64u) error code(%d)", spSession->GetUID(), iRst);
		ProcessClientError(spSession->GetLink(), eLR_SYSTEM_ERROR, eKICK_DATABASE_ERROR, __FUNCTION__);
		return false;
	}
	return true;
}

void ClientHandler::OnSelectAccount(NetLinkPtr spLink, SelectAccount* pQry)
{
	Session* pSession = pQry->GetSession();
	ProcessClientLogin(pSession->GetLink(), pSession->m_iUID, pSession->GetAccount());
}

bool ClientHandler::OnClientLogin(NetLinkPtr spLink, const ClientLoginData& r)
{
	SessionPtr spSession = (Session*)spLink->UserData();
	if (spSession)
	{
		prn_err("already authentication proceed session");
		return false;
	}

	if (Application::GetBlockClient())
	{
		// 접속 금지
		ProcessClientError(spLink, eLR_SYSTEM_ERROR, eKICK_ADMIN, __FUNCTION__);
		return false;
	}

	std::string sAccount = util::lower(r.m_sAccount);
	sAccount = util::trim(sAccount);

	if (sAccount.empty() || util::escape(sAccount))
	{
		prn_err("account is not valid(%s)", cstr(sAccount));
		ProcessClientError(spLink, eLR_INVALID_ACCOUNT, eKICK_INVALID_ACCOUNT, __FUNCTION__);
		return false;
	}

	// version check
	if (m_MinVersion > r.m_sVersion)
	{
		prn_err("invalid client version(%s), min_version(%s)", cstr(r.m_sVersion), cstr(m_MinVersion.ToString()));
		ProcessClientError(spLink, eLR_INVALID_VERSION, eKICK_INVALID_VERSION, __FUNCTION__);
		return false;
	}

	ProcessClientLogin(spLink, r.m_iUID, sAccount);
	
	return true;
}

void ClientHandler::OnAuthentication(SessionPtr spSession)
{
	ClientLoginSuccessData pk;
	pk.m_iCID = spSession->GetChannelID();
	pk.m_iAuthKey = spSession->GetAuthKey();
	rpc_OnClientLoginSuccess(spSession->GetLink(), pk);
	prn_inf("authenicate uid(%I64u)", spSession->GetUID());
}