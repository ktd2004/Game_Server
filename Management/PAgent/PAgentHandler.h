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
		SAgent���� ������ ��û�Ѵ�.
		@param spLink SAgent �� NetLink
		@param arg1 PAgent ���̵�
	*/
	IMPORT_RPC_NM_V1( OnSAgentConnected, uint32, 1024 );
	/** 
		�������� ������ ��������� �˸���.
		@param spLink PAgent �� NetLink
		@param arg1 ��������
	*/
	IMPORT_RPC_NM_V1( OnSAgentUpdateProcess, const ServerProcess&, 1024 );
	/** 
		������ ��������� �˸���.
		@param spLink PAgent �� NetLink
		@param arg1 ��������
	*/
	IMPORT_RPC_NM_V1( OnSAgentExitProcess, const ServerProcess&, 1024 );
	/** 
		������ ���� ����� �˸���.
		@param spLink PAgent �� NetLink
		@param arg1 ���� �Ϸù�ȣ
		@param arg2 ������ ������ �������̵�
		@param arg3 ���� ���� ���
	*/
	IMPORT_RPC_NM_V3( OnSAgentExecuteCmdResult, uint32, uint32, bool, 1024 );

	//////////////////////////////////////////////////////////////////////////
	// sagent -> pagent
	/** 
		PAgent�� ���� ��뿩�θ� �����Ѵ�.
		@param spLink PAgent �� NetLink
		@param bRet ������뿩��
		@param sPAgentName PAgent �̸�
		@param DbCfg ���������� ��ȸ�� ����Ÿ���̽� ���� ����
	*/
	bool OnPAgentConnectResult( NetLinkPtr spLink, bool bRet, const std::string& sPAgentName, const IDBProvider::Properties& DbCfg );
	/** 
		PAgent�� ���������� �����Ѵ�.
		@param spLink PAgent �� NetLink
		@param Pkt ��������
	*/
	bool OnPAgentAddProcess( NetLinkPtr spLink, const ServerProcess& Pkt );
	/** 
		PAgent�� ���� ���� ��û�� �����Ѵ�.
		@param spLink PAgent �� NetLink
		@param iSeq ���� �Ϸù�ȣ
		@param iSID �������̵�
	*/
	bool OnPAgentLaunchProcess( NetLinkPtr spLink, uint32 iSeq, uint32 iSID );
	/** 
		PAgent�� ���� ���� ��û�� �����Ѵ�.
		@param spLink PAgent �� NetLink
		@param iSeq ���� �Ϸù�ȣ
		@param iSID �������̵�
	*/
	bool OnPAgentShutdownProcess( NetLinkPtr spLink, uint32 iSeq, uint32 iSID );
	/** 
		PAgent�� ���� ���� ���� ��û�� �����Ѵ�.
		@param spLink PAgent �� NetLink
		@param iSeq ���� �Ϸù�ȣ
		@param iSID �������̵�
	*/
	bool OnPAgentKillProcess( NetLinkPtr spLink, uint32 iSeq, uint32 iSID );
	/** 
		PAgent�� Ŭ���̾�Ʈ ���� ���� ��û�� �����Ѵ�.
		@param spLink PAgent �� NetLink
		@param iSeq �����Ϸù�ȣ
		@param iSID �������̵�
		@param bBlock Ŭ���̾�Ʈ�������ѿ���
	*/
	bool OnPAgentBlockClient( NetLinkPtr spLink, uint32 iSeq, uint32 iSID, bool bBlock );
	/** 
		PAgent�� Ŭ���̾�Ʈ ���Ӽ� ���� ��û�� �����Ѵ�.
		@param spLink PAgent �� NetLink
		@param iSeq �����Ϸù�ȣ
		@param iSID �������̵�
		@param iMaxClient �ִ����Ӽ�
	*/
	bool OnPAgentMaxClient( NetLinkPtr spLink, uint32 iSeq, uint32 iSID, uint32 iMaxClient );

	//////////////////////////////////////////////////////////////////////////
	// server -> pagent
	/** 
		PAgent���� ������ ������ ��û�Ѵ�.
		@param spLink ������ NetLink
		@param iSID �������̵�
		@param iPID �������μ������̵�
	*/
	bool OnPAgentRegSID( NetLinkPtr spLink, uint32 iSID, uint32 iPID );
	/** 
		PAgent���� ������ ���������� �˷��ش�.
		@param spLink ������ NetLink
		@param iSID �������̵�
		@param bBlock Ŭ���̾�Ʈ���ӱ�������
		@param iMaxClient Ŭ���̾�Ʈ�ִ����Ӽ�
		@param iUsers �������Ӽ�
	*/
	bool OnPAgentReport( NetLinkPtr spLink, uint32 iSID, bool bBlock, uint32 iMaxClient, uint32 iUsers );

	//////////////////////////////////////////////////////////////////////////
	// pagent -> server
	/** 
		������ ��������� �˷��ش�.
		@param spLink ������ NetLink
		@param arg1 ������뿩��
		@param arg2 Ŭ���̾�Ʈ���ӱ�������
		@param arg3 Ŭ���̾�Ʈ�ִ����Ӽ�
	*/
	IMPORT_RPC_NM_V3( OnServerConnectResult, bool, bool, uint32, 1024 );
	/** 
		������ ���Ḧ ��û�Ѵ�.
		@param spLink ������ NetLink
		@param arg1 ���� �Ϸù�ȣ
	*/
	IMPORT_RPC_NM_V1( OnServerShutdown, uint32, 1024 );
	/** 
		�������� Ŭ���̾�Ʈ ���� ������ ��û�Ѵ�.
		@param spLink PAgent �� NetLink
		@param arg1 Ŭ���̾�Ʈ�������ѿ���
	*/
	IMPORT_RPC_NM_V1( OnServerBlockClient, bool, 1024 );
	/** 
		�������� Ŭ���̾�Ʈ ���Ӽ� ���� ��û�� ��û�Ѵ�.
		@param spLink PAgent �� NetLink
		@param arg1 �ִ����Ӽ�
	*/
	IMPORT_RPC_NM_V1( OnServerMaxClient, uint32, 1024 );


	//////////////////////////////////////////////////////////////////////////
	// process handler
	/** 
		���μ��� ���۽� ȣ��
	*/
	void OnLaunchProcess( ProcessPtr spProcess ) override;
	/** 
		���μ��� ����� ȣ��
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