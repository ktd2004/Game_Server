/* Copyright 2013 by Lee yong jun
 * All rights reserved
 *
 * Distribute freely, except: don't remove my name from the source or
 * documentation (don't take credit for my work), mark your changes (don't
 * get me blamed for your possible bugs), don't alter or remove this
 * notice.  May be sold if buildable source is provided to buyer.  No
 * warrantee of any kind, express or implied, is included with this
 * software; use at your own risk, responsibility for damages (if any) to
 * anyone resulting from the use of this software rests entirely with the
 * user.
 *
 * Send bug reports, bug fixes, enhancements, requests, flames, etc., and
 * I'll try to keep a version up to date.  I can be reached as follows:
 * Lee yong jun          iamhere01@naver.com
 */
#pragma once

#include <Proactor.h>
#include <ServerFrame.h>
#include <ProcessEx.h>
#include <ManagementProtocol.h>
#include <ServerProcessManager.h>
#include <DBEngine.h>

class PAgentHandler
	: public ServerApp
	, public NetEventHandler
	, public ProcessHandler
{
private:
	NetLinkPtr				m_SAgent;
	ServerProcessManager	m_ServerProcessManager;
	uint32					m_iSAgentPort;
	std::string				m_sSAgentAddr;
	uint32					m_iMaxRestartCnt;
	uint32					m_iPAgentPort;
	std::string				m_sPAgentAddr;
	std::string				m_sDbconnectstring;
public:
	PAgentHandler(int argc, char* argv[]);
	virtual ~PAgentHandler();

public:
	//////////////////////////////////////////////////////////////////////////
	// pagent -> sagent
	/** 
		SAgent에게 인증을 요청한다.
		@param spLink SAgent 의 NetLink
		@param arg1 PAgent 아이디
	*/
	IMPORT_RPC_NM_V1( OnSAgentConnected, uint32, 1024 );
	/** 
		서버상태 정보를 변경됬음을 알린다.
		@param spLink PAgent 의 NetLink
		@param arg1 서버정보
	*/
	IMPORT_RPC_NM_V1( OnSAgentUpdateProcess, const ServerProcess&, 1024 );
	/** 
		서버가 종료됬음을 알린다.
		@param spLink PAgent 의 NetLink
		@param arg1 서버정보
	*/
	IMPORT_RPC_NM_V1( OnSAgentExitProcess, const ServerProcess&, 1024 );
	/** 
		운영명령의 실행 결과를 알린다.
		@param spLink PAgent 의 NetLink
		@param arg1 운영명령 일련번호
		@param arg2 운영명령을 전달할 서버아이디
		@param arg3 운영명령 실행 결과
	*/
	IMPORT_RPC_NM_V3( OnSAgentExecuteCmdResult, uint32, uint32, bool, 1024 );

	//////////////////////////////////////////////////////////////////////////
	// sagent -> pagent
	/** 
		PAgent의 접속 허용여부를 수신한다.
		@param spLink PAgent 의 NetLink
		@param bRet 접속허용여부
		@param sPAgentName PAgent 이름
		@param DbCfg 서버정보를 조회할 데이타베이스 접속 정보
	*/
	bool OnPAgentConnectResult( NetLinkPtr spLink, bool bRet, const std::string& sPAgentName, const IDBProvider::Properties& DbCfg );
	/** 
		PAgent가 서버정보를 수신한다.
		@param spLink PAgent 의 NetLink
		@param Pkt 서버정보
	*/
	bool OnPAgentAddProcess( NetLinkPtr spLink, const ServerProcess& Pkt );
	/** 
		PAgent가 서버 실행 요청를 수신한다.
		@param spLink PAgent 의 NetLink
		@param iSeq 운영명령 일련번호
		@param iSID 서버아이디
	*/
	bool OnPAgentLaunchProcess( NetLinkPtr spLink, uint32 iSeq, uint32 iSID );
	/** 
		PAgent가 서버 종료 요청를 수신한다.
		@param spLink PAgent 의 NetLink
		@param iSeq 운영명령 일련번호
		@param iSID 서버아이디
	*/
	bool OnPAgentShutdownProcess( NetLinkPtr spLink, uint32 iSeq, uint32 iSID );
	/** 
		PAgent가 서버 강제 종료 요청를 수신한다.
		@param spLink PAgent 의 NetLink
		@param iSeq 운영명령 일련번호
		@param iSID 서버아이디
	*/
	bool OnPAgentKillProcess( NetLinkPtr spLink, uint32 iSeq, uint32 iSID );
	/** 
		PAgent가 클라이언트 접속 금지 요청를 수신한다.
		@param spLink PAgent 의 NetLink
		@param iSeq 운영명령일련번호
		@param iSID 서버아이디
		@param bBlock 클라이언트접속제한여부
	*/
	bool OnPAgentBlockClient( NetLinkPtr spLink, uint32 iSeq, uint32 iSID, bool bBlock );
	/** 
		PAgent가 클라이언트 접속수 제한 요청을 수신한다.
		@param spLink PAgent 의 NetLink
		@param iSeq 운영명령일련번호
		@param iSID 서버아이디
		@param iMaxClient 최대접속수
	*/
	bool OnPAgentMaxClient( NetLinkPtr spLink, uint32 iSeq, uint32 iSID, uint32 iMaxClient );

	//////////////////////////////////////////////////////////////////////////
	// server -> pagent
	/** 
		PAgent에게 서버가 인증을 요청한다.
		@param spLink 서버의 NetLink
		@param iSID 서버아이디
		@param iPID 서버프로세스아이디
	*/
	bool OnPAgentRegSID( NetLinkPtr spLink, uint32 iSID, uint32 iPID );
	/** 
		PAgent에게 서버의 상태정보를 알려준다.
		@param spLink 서버의 NetLink
		@param iSID 서버아이디
		@param bBlock 클라이언트접속금지여부
		@param iMaxClient 클라이언트최대접속수
		@param iUsers 현재접속수
	*/
	bool OnPAgentReport( NetLinkPtr spLink, uint32 iSID, bool bBlock, uint32 iMaxClient, uint32 iUsers );

	//////////////////////////////////////////////////////////////////////////
	// pagent -> server
	/** 
		서버가 인증결과를 알려준다.
		@param spLink 서버의 NetLink
		@param arg1 접속허용여부
		@param arg2 클라이언트접속금지여부
		@param arg3 클라이언트최대접속수
	*/
	IMPORT_RPC_NM_V3( OnServerConnectResult, bool, bool, uint32, 1024 );
	/** 
		서버의 종료를 요청한다.
		@param spLink 서버의 NetLink
		@param arg1 운영명령 일련번호
	*/
	IMPORT_RPC_NM_V1( OnServerShutdown, uint32, 1024 );
	/** 
		서버에게 클라이언트 접속 금지를 요청한다.
		@param spLink PAgent 의 NetLink
		@param arg1 클라이언트접속제한여부
	*/
	IMPORT_RPC_NM_V1( OnServerBlockClient, bool, 1024 );
	/** 
		서버에게 클라이언트 접속수 제한 요청을 요청한다.
		@param spLink PAgent 의 NetLink
		@param arg1 최대접속수
	*/
	IMPORT_RPC_NM_V1( OnServerMaxClient, uint32, 1024 );


	//////////////////////////////////////////////////////////////////////////
	// process handler
	/** 
		프로세스 시작시 호출
	*/
	void OnLaunchProcess( ProcessPtr spProcess ) override;
	/** 
		프로세스 종료시 호출
	*/
	void OnExitProcess( ProcessPtr spProcess ) override;

protected:
	bool OnSetProperty( ServerApp::Properties& appCfg ) override;
	bool OnInitialized(int32 argc, char* argv[]) override;
	void OnQuitApplication( void ) override;
	bool OnAccepted( NetLinkPtr spLink );
	bool OnConnected( NetLinkPtr spLink );
	bool OnClosed( NetLinkPtr spLink );
};