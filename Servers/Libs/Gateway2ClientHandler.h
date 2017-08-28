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
	게이트웨이에서 클라이언트와 통신하기 위해 사용되는 네트웍 이벤트 핸들러.
*/
class Gateway2ClientHandler : public NetEventHandler
{
	friend class Gateway2MasterHandler;
	friend class Gateway2ChannelHandler;
protected:
	Version					m_MinVersion;		//< 접속 가능한 클라이언트 최소 버전
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
		클라에게 에러 메세지를 전송한후 킥한다.
	*/
	void			ProcessClientError(SessionPtr spSession, int32 eResult, int32 eKick, const char* szFunc);
	/**
		클라에게 에러 메세지를 전송한후 킥한다.
	*/
	void			ProcessClientError(NetLinkPtr spLink, int32 eResult, int32 eKick, const char* szFunc);
	/**
		클라를 로그오프 시키도록 마스터에 요청한다.
	*/
	bool			ProcessClientLogoff(SessionPtr spSession);
	/**
		클라의 로그인 정보를 마스터로 알린다.
	*/
	bool			ProcessClientLogin(NetLinkPtr spLink, UID iUID, const std::string& sAccount);
	/*
		클라 정보를 마스터에 등록된후 호출된다.
	*/
	virtual void	OnAuthentication(SessionPtr spSession) {};
	
private:
	/** 
		클라가 인증서버에 연결
	*/
	bool			OnAccepted( NetLinkPtr spLink );
	/** 
		클라가 인증서버와 연결이 종료
	*/
	bool			OnClosed( NetLinkPtr spLink );

	// to client
	IMPORT_RPC_V1(eGTWS_CLIENT_LOGIN_ERROR,   ErrorToClient,   int32, 1024);

	// from client
	bool		OnReceived(NetLinkPtr spLink, Buffer* pBuffer);
};
