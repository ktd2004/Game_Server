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

/** 
	PAgent �� ���� ������ ���ɾ ó���ϴ� �������̽�.
*/
struct PAgentReceiver
{
	/** 
		Ŭ���̾�Ʈ ������ �����Ѵ�.
		@param bBlock ���ٱ���(true), �������(false)
	*/
	virtual bool	OnBlockClient( bool bBlock ) = 0;
	/** 
		Ŭ���̾�Ʈ �ִ����Ӽ��� �����Ѵ�.
		@param iMaxClient �ִ����Ӽ�
	*/
	virtual bool	OnMaxClient( uint32 iMaxClient ) = 0;
};

/** 
	PAgent���� ������ �����ϴ� Ŭ����

	ServerApp ���ο��� ���Ǹ� PAgent�� �����Ͽ� ������ �ۼ����ϴ� ����� �����Ѵ�.
*/
class PAgentConnector 
	: public NetEventHandler
	, public LogWriteCallback
{
public:
	struct Properties
	{
		PAgentReceiver*		pHandler;		///< PAgent ���� �̺�Ʈ�� ó���ϴ� �ڵ鷯
		bool				bLogTransmit;	///< �α� ���� ����
		unsigned short		iLogsPort;		///< �α׼��� ��Ʈ
		std::string			sLogsAddr;		///< �α׼��� �ּ�
		std::string			sBindAddr;		///< �α������� ���� �����ϴ� UDP ��Ĺ�� �ּ�
		Properties()
			: pHandler(nullptr)
			, bLogTransmit(false)
			, iLogsPort(3221)
			, sLogsAddr("localhost")
			, sBindAddr(null_string)
		{}
		Properties( const Properties& r )
			: pHandler(r.pHandler)
			, bLogTransmit(r.bLogTransmit)
			, iLogsPort(r.iLogsPort)
			, sLogsAddr(r.sLogsAddr)
			, sBindAddr(r.sBindAddr)
		{}
	};
private:
	NetLinkPtr			m_PAgent;			///< PAgent�� ����� NetLink
	NetLinkPtr			m_Logger;			///< LogS�� ����� NetLink
	Properties			m_Prop;				///< ������
	SOCKADDR_IN			m_LoggerAddr;		///< LogS�� �ּҰ�

public:
	PAgentConnector();
	PAgentConnector( const PAgentConnector& r ) : NetEventHandler(1) {}
	virtual ~PAgentConnector();

public:
	/** 
		�ʱ�ȭ�� �����Ѵ�.
		@param Prop ������
	*/
	void Initialize( const PAgentConnector::Properties& Prop );
	/** 
		PAgent �� �����ϰ� Log�� ������ �غ� �Ѵ�.
	*/
	bool Connect( void );
	/** 
		PAgent �� ����Ǹ� ȣ��Ǵ� �ݹ��Լ�
		@param spLink PAgent�� NetLink
	*/
	bool OnConnected( NetLinkPtr spLink );
	/** 
		PAgent �� ������ ����Ǹ� ȣ��Ǵ� �ݹ��Լ�
		@param spLink PAgent�� NetLink
	*/
	bool OnClosed( NetLinkPtr spLink );
	/** 
		�α׼����� ��Ŷ�� �����Ѵ�.
		@param iLevel �α� ����
		@param pFrmt �α� ����
		@param argp �α׵���Ÿ
	*/
	void OnWrite( uint32 iLevel, const char* pFrmt, va_list argp );

protected:
	/** 
		PAgent�� ���������� ����Ѵ�.
		@param spLink PAgent�� NetLink
		@param arg1 �������̵�(SID)
		@param arg2 ���μ������̵�(PID)
		@param arg3 ��Ŷ�ִ�ũ��
	*/
	IMPORT_RPC_NM_V2( OnPAgentRegSID, uint32, uint32, 1024 );
	/** 
		PAgent�� �������������� �����Ѵ�.
		@param spLink PAgent�� NetLink
		@param arg1 �������̵�(SID)
		@param arg2 Ŭ���̾�Ʈ �� ����
		@param arg3 �ִ����Ӽ�
		@param arg4 ���� ���Ӽ�
	*/
	IMPORT_RPC_NM_V4( OnPAgentReport, uint32, bool, uint32, uint32, 1024 );
	/** 
		�α׼����� �α׸� �����Ѵ�.
		@param spLink LogS�� NetLink
		@param arg1 �α׷���
		@param arg2 �α׵���Ÿ
	*/
	IMPORT_RPC_NM_V2( OnLogTransmit, uint32, const std::string&, 2048 );
	/** 
		PAgent�� ������� ��������� �˷��ֱ� ���� ȣ��ȴ�.
		@param spLink PAgent�� NetLink
		@param bRet �������
		@param bBlock Ŭ���̾�Ʈ �� ����
		@param iMaxClient �ִ����Ӽ�
	*/
	bool OnServerConnectResult( NetLinkPtr spLink, bool bRet, bool bBlock, uint32 iMaxClient );
	/** 
		PAgent�� ������ �����϶�� ��û�Ҷ� ȣ��ȴ�.
		@param spLink PAgent�� NetLink
		@param arg1 ���� ������ȣ
	*/
	bool OnServerShutdown( NetLinkPtr spLink, uint32 iSeq );
	/** 
		PAgent�� Ŭ���̾��� �������� �Ҷ� ȣ��ȴ�.
		@param spLink PAgent�� NetLink
		@param bBlock Ŭ���̾�Ʈ �� ����
	*/
	bool OnServerBlockClient( NetLinkPtr spLink, bool bBlock );
	/** 
		PAgent�� Ŭ���̾��� ���Ӽ��� �Ҷ� ȣ��ȴ�.
		@param spLink PAgent�� NetLink
		@param iMaxClient �ִ����Ӽ�
	*/
	bool OnServerMaxClient( NetLinkPtr spLink, uint32 iMaxClient );
};