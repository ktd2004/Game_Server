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
		���� ������ ��û�Ѵ�.
		@param spLink PAgent �� NetLink
		@param arg1 ���� �Ϸù�ȣ
		@param arg2 �������̵�
	*/
	IMPORT_RPC_NM_V2( OnPAgentLaunchProcess, uint32, uint32, 1024 );
	/** 
		���� ���Ḧ ��û�Ѵ�.
		@param spLink PAgent �� NetLink
		@param arg1 ���� �Ϸù�ȣ
		@param arg2 �������̵�
	*/
	IMPORT_RPC_NM_V2( OnPAgentShutdownProcess, uint32, uint32, 1024 );
	/** 
		���� ���� ���Ḧ ��û�Ѵ�.
		@param spLink PAgent �� NetLink
		@param arg1 ���� �Ϸù�ȣ
		@param arg2 �������̵�
	*/
	IMPORT_RPC_NM_V2( OnPAgentKillProcess, uint32, uint32, 1024 );
	/** 
		PAgent�� ������ ���������� �����Ѵ�.
		@param spLink PAgent �� NetLink
		@param arg1 ��������
	*/
	IMPORT_RPC_NM_V1( OnPAgentAddProcess, const ServerProcess&, 2048 );
	/** 
		PAgent�� ���� ��뿩�θ� �˷��ش�.
		@param spLink PAgent �� NetLink
		@param arg1 ������뿩��
		@param arg2 PAgent �̸�
		@param arg3 ���������� ��ȸ�� ����Ÿ���̽� ���� ����
	*/
	IMPORT_RPC_NM_V3( OnPAgentConnectResult, bool, const std::string&, const IDBProvider::Properties&, 1024 );
	/** 
		�������� Ŭ���̾�Ʈ ���� ������ ��û�Ѵ�
		@param spLink PAgent �� NetLink
		@param arg1 �����Ϸù�ȣ
		@param arg2 �������̵�
		@param arg3 Ŭ���̾�Ʈ�������ѿ���
	*/
	IMPORT_RPC_NM_V3( OnPAgentBlockClient, uint32, uint32, bool, 1024 );
	/** 
		�������� Ŭ���̾�Ʈ ���Ӽ� ���� ��û�Ѵ�.
		@param spLink PAgent �� NetLink
		@param arg1 �����Ϸù�ȣ
		@param arg2 �������̵�
		@param arg3 �ִ����Ӽ�
	*/
	IMPORT_RPC_NM_V3( OnPAgentMaxClient, uint32, uint32, uint32, 1024 );

	//////////////////////////////////////////////////////////////////////////
	// pagent -> sagent
	/** 
		PAgent�� ������ ��û�Ѵ�.
		@param spLink PAgent �� NetLink
		@param iPAID PAgent�� ���̵�
	*/
	bool OnSAgentConnected( NetLinkPtr spLink, uint32 iPAID );
	/** 
		�������� ������ ��������� �˸���.
		@param spLink PAgent �� NetLink
		@param Pkt ��������
	*/
	bool OnSAgentUpdateProcess( NetLinkPtr spLink, const ServerProcess& Pkt );
	/** 
		������ ��������� �˸���.
		@param spLink PAgent �� NetLink
		@param Pkt ��������
	*/
	bool OnSAgentExitProcess( NetLinkPtr spLink, const ServerProcess& Pkt );
	/** 
		������ ���� ����� �˸���.
		@param spLink PAgent �� NetLink
		@param iSeq ���� �Ϸù�ȣ
		@param iSID ������ ������ �������̵�
		@param bRet ���� ���� ���
	*/
	bool OnSAgentExecuteCmdResult( NetLinkPtr spLink, uint32 iSeq, uint32 iSID, bool bRet );

	bool OnConsoleConnected( NetLinkPtr spLink );
	IMPORT_RPC_NM_V0(OnReady, 1024);
	bool OnConsoleExecCommand( NetLinkPtr spLink, const std::string& sCmd );
	IMPORT_RPC_NM_V1( OnConsoleExecResult, const std::string&, 1024 );
	//////////////////////////////////////////////////////////////////////////
	// console
	/** 
		���� ������ ��û�Ѵ�.
		@param pCaller ȣ����
		@param arg1 PAgent�̸�
		@param arg2 �����̸�
	*/
	IMPORT_COMMAND_V2( OnCommandLaunchProcess );
	/** 
		���� ���Ḧ ��û�Ѵ�.
		@param pCaller ȣ����
		@param arg1 PAgent�̸�
		@param arg2 �����̸�
	*/
	IMPORT_COMMAND_V2( OnCommandShutdownProcess );
	/** 
		���� ���� ���Ḧ ��û�Ѵ�.
		@param pCaller ȣ����
		@param arg1 PAgent�̸�
		@param arg2 �����̸�
	*/
	IMPORT_COMMAND_V2( OnCommandKillProcess );
	/** 
		������ ���� ������ ����Ѵ�.
		@param pCaller ȣ����
		@param arg1 PAgent�̸�
		@param arg2 �����̸�
	*/
	IMPORT_COMMAND_V2( OnCommandDumpServerStatus );
	/** 
		������ ���� ������ ����Ѵ�.
		@param pCaller ȣ����
		@param arg1 PAgent�̸�
		@param arg2 �����̸�
		@param arg3 Ŭ���̾�Ʈ���ӱ�������
	*/
	IMPORT_COMMAND_V3( OnCommandBlockClient );
	/** 
		������ ���� ������ ����Ѵ�.
		@param pCaller ȣ����
		@param arg1 PAgent�̸�
		@param arg2 �����̸�
		@param arg3 Ŭ���̾�Ʈ�ִ����Ӽ�
	*/
	IMPORT_COMMAND_V3( OnCommandMaxClient );
	/** 
		ǥ��������� �α� ����� �����ϰų� ����Ѵ�.
		@param pCaller ȣ����
		@param arg1 �α� ��� ��� Ȥ�� ����
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