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
#include <DBEngine.h>
#include <ManagementProtocol.h>

class SAgentHandler
	: public ServerApp
	, public NetEventHandler
{
private:
	uint32					m_iUniqueID;
	uint32					m_iBindPort;
	std::string				m_sBindAddr;
	IDBProvider::Properties m_DbCfg;

	enum { ePAgent = 1, eConsole = 2 };

public:
	SAgentHandler(int argc, char* argv[]);
	virtual ~SAgentHandler();

	//////////////////////////////////////////////////////////////////////////
	// sagent -> pagent
	/** 
		서버 실행을 요청한다.
		@param spLink PAgent 의 NetLink
		@param arg1 운영명령 일련번호
		@param arg2 서버아이디
	*/
	IMPORT_RPC_NM_V2( OnPAgentLaunchProcess, uint32, uint32, 1024 );
	/** 
		서버 종료를 요청한다.
		@param spLink PAgent 의 NetLink
		@param arg1 운영명령 일련번호
		@param arg2 서버아이디
	*/
	IMPORT_RPC_NM_V2( OnPAgentShutdownProcess, uint32, uint32, 1024 );
	/** 
		서버 강제 종료를 요청한다.
		@param spLink PAgent 의 NetLink
		@param arg1 운영명령 일련번호
		@param arg2 서버아이디
	*/
	IMPORT_RPC_NM_V2( OnPAgentKillProcess, uint32, uint32, 1024 );
	/** 
		PAgent가 관리할 서버정보를 전달한다.
		@param spLink PAgent 의 NetLink
		@param arg1 서버정보
	*/
	IMPORT_RPC_NM_V1( OnPAgentAddProcess, const ServerProcess&, 2048 );
	/** 
		PAgent의 접속 허용여부를 알려준다.
		@param spLink PAgent 의 NetLink
		@param arg1 접속허용여부
		@param arg2 PAgent 이름
		@param arg3 서버정보를 조회할 데이타베이스 접속 정보
	*/
	IMPORT_RPC_NM_V3( OnPAgentConnectResult, bool, const std::string&, const IDBProvider::Properties&, 1024 );
	/** 
		서버에게 클라이언트 접속 금지를 요청한다
		@param spLink PAgent 의 NetLink
		@param arg1 운영명령일련번호
		@param arg2 서버아이디
		@param arg3 클라이언트접속제한여부
	*/
	IMPORT_RPC_NM_V3( OnPAgentBlockClient, uint32, uint32, bool, 1024 );
	/** 
		서버에게 클라이언트 접속수 제한 요청한다.
		@param spLink PAgent 의 NetLink
		@param arg1 운영명령일련번호
		@param arg2 서버아이디
		@param arg3 최대접속수
	*/
	IMPORT_RPC_NM_V3( OnPAgentMaxClient, uint32, uint32, uint32, 1024 );

	//////////////////////////////////////////////////////////////////////////
	// pagent -> sagent
	/** 
		PAgent가 인증을 요청한다.
		@param spLink PAgent 의 NetLink
		@param iPAID PAgent의 아이디
	*/
	bool OnSAgentConnected( NetLinkPtr spLink, uint32 iPAID );
	/** 
		서버상태 정보를 변경됬음을 알린다.
		@param spLink PAgent 의 NetLink
		@param Pkt 서버정보
	*/
	bool OnSAgentUpdateProcess( NetLinkPtr spLink, const ServerProcess& Pkt );
	/** 
		서버가 종료됬음을 알린다.
		@param spLink PAgent 의 NetLink
		@param Pkt 서버정보
	*/
	bool OnSAgentExitProcess( NetLinkPtr spLink, const ServerProcess& Pkt );
	/** 
		운영명령의 실행 결과를 알린다.
		@param spLink PAgent 의 NetLink
		@param iSeq 운영명령 일련번호
		@param iSID 운영명령을 전달할 서버아이디
		@param bRet 운영명령 실행 결과
	*/
	bool OnSAgentExecuteCmdResult( NetLinkPtr spLink, uint32 iSeq, uint32 iSID, bool bRet );

	bool OnConsoleConnected( NetLinkPtr spLink );
	IMPORT_RPC_NM_V0(OnReady, 1024);
	bool OnConsoleExecCommand( NetLinkPtr spLink, const std::string& sCmd );
	IMPORT_RPC_NM_V1( OnConsoleExecResult, const std::string&, 1024 );
	//////////////////////////////////////////////////////////////////////////
	// console
	/** 
		서버 실행을 요청한다.
		@param pCaller 호출자
		@param arg1 PAgent이름
		@param arg2 서버이름
	*/
	IMPORT_COMMAND_V2( OnCommandLaunchProcess );
	/** 
		서버 종료를 요청한다.
		@param pCaller 호출자
		@param arg1 PAgent이름
		@param arg2 서버이름
	*/
	IMPORT_COMMAND_V2( OnCommandShutdownProcess );
	/** 
		서버 강제 종료를 요청한다.
		@param pCaller 호출자
		@param arg1 PAgent이름
		@param arg2 서버이름
	*/
	IMPORT_COMMAND_V2( OnCommandKillProcess );
	/** 
		서버의 상태 정보를 출력한다.
		@param pCaller 호출자
		@param arg1 PAgent이름
		@param arg2 서버이름
	*/
	IMPORT_COMMAND_V2( OnCommandDumpServerStatus );
	/** 
		서버의 상태 정보를 출력한다.
		@param pCaller 호출자
		@param arg1 PAgent이름
		@param arg2 서버이름
		@param arg3 클라이언트접속금지여부
	*/
	IMPORT_COMMAND_V3( OnCommandBlockClient );
	/** 
		서버의 상태 정보를 출력한다.
		@param pCaller 호출자
		@param arg1 PAgent이름
		@param arg2 서버이름
		@param arg3 클라이언트최대접속수
	*/
	IMPORT_COMMAND_V3( OnCommandMaxClient );
	/** 
		표준출력으로 로그 출력을 금지하거나 허용한다.
		@param pCaller 호출자
		@param arg1 로그 출력 허용 혹은 금지
	*/
	IMPORT_COMMAND_V1( OnCommandVerbose );

protected:
	bool OnSetProperty( ServerApp::Properties& appCfg ) override;
	bool OnInitialized(int32 argc, char* argv[]) override;
	void OnQuitApplication( void ) override;
	bool OnAccepted( NetLinkPtr spLink );
	bool OnConnected( NetLinkPtr spLink );
	bool OnClosed( NetLinkPtr spLink );
	void DoCommand( const std::string& sCommand ) override;
};