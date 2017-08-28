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
	PAgent 로 부터 수신한 운영명령어를 처리하는 인터페이스.
*/
struct PAgentReceiver
{
	/** 
		클라이언트 접근을 제어한다.
		@param bBlock 접근금지(true), 접근허용(false)
	*/
	virtual bool	OnBlockClient( bool bBlock ) = 0;
	/** 
		클라이언트 최대접속수를 제어한다.
		@param iMaxClient 최대접속수
	*/
	virtual bool	OnMaxClient( uint32 iMaxClient ) = 0;
};

/** 
	PAgent와의 연결을 관리하는 클래스

	ServerApp 내부에서 사용되며 PAgent와 연결하여 운영명령을 송수신하는 기능을 수행한다.
*/
class PAgentConnector 
	: public NetEventHandler
	, public LogWriteCallback
{
public:
	struct Properties
	{
		PAgentReceiver*		pHandler;		///< PAgent 운영명령 이벤트를 처리하는 핸들러
		bool				bLogTransmit;	///< 로그 전송 여부
		unsigned short		iLogsPort;		///< 로그서버 포트
		std::string			sLogsAddr;		///< 로그서버 주소
		std::string			sBindAddr;		///< 로그전송을 위해 생성하는 UDP 소캣의 주소
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
	NetLinkPtr			m_PAgent;			///< PAgent와 연결된 NetLink
	NetLinkPtr			m_Logger;			///< LogS와 연결된 NetLink
	Properties			m_Prop;				///< 설정값
	SOCKADDR_IN			m_LoggerAddr;		///< LogS의 주소값

public:
	PAgentConnector();
	PAgentConnector( const PAgentConnector& r ) : NetEventHandler(1) {}
	virtual ~PAgentConnector();

public:
	/** 
		초기화를 수행한다.
		@param Prop 설정값
	*/
	void Initialize( const PAgentConnector::Properties& Prop );
	/** 
		PAgent 에 연결하고 Log를 전송할 준비를 한다.
	*/
	bool Connect( void );
	/** 
		PAgent 에 연결되면 호출되는 콜백함수
		@param spLink PAgent의 NetLink
	*/
	bool OnConnected( NetLinkPtr spLink );
	/** 
		PAgent 에 연결이 종료되면 호출되는 콜백함수
		@param spLink PAgent의 NetLink
	*/
	bool OnClosed( NetLinkPtr spLink );
	/** 
		로그서버로 패킷을 전송한다.
		@param iLevel 로그 레벨
		@param pFrmt 로그 포멧
		@param argp 로그데이타
	*/
	void OnWrite( uint32 iLevel, const char* pFrmt, va_list argp );

protected:
	/** 
		PAgent의 서버정보를 등록한다.
		@param spLink PAgent의 NetLink
		@param arg1 서버아이디(SID)
		@param arg2 프로세스아이디(PID)
		@param arg3 패킷최대크기
	*/
	IMPORT_RPC_NM_V2( OnPAgentRegSID, uint32, uint32, 1024 );
	/** 
		PAgent의 서버상태정보를 전송한다.
		@param spLink PAgent의 NetLink
		@param arg1 서버아이디(SID)
		@param arg2 클라이언트 블럭 여부
		@param arg3 최대접속수
		@param arg4 현재 접속수
	*/
	IMPORT_RPC_NM_V4( OnPAgentReport, uint32, bool, uint32, uint32, 1024 );
	/** 
		로그서버로 로그를 전송한다.
		@param spLink LogS의 NetLink
		@param arg1 로그레벨
		@param arg2 로그데이타
	*/
	IMPORT_RPC_NM_V2( OnLogTransmit, uint32, const std::string&, 2048 );
	/** 
		PAgent에 연결된후 인증결과를 알려주기 위해 호출된다.
		@param spLink PAgent의 NetLink
		@param bRet 인증결과
		@param bBlock 클라이언트 블럭 여부
		@param iMaxClient 최대접속수
	*/
	bool OnServerConnectResult( NetLinkPtr spLink, bool bRet, bool bBlock, uint32 iMaxClient );
	/** 
		PAgent에 서버를 종료하라고 요청할때 호출된다.
		@param spLink PAgent의 NetLink
		@param arg1 운영명령 고유번호
	*/
	bool OnServerShutdown( NetLinkPtr spLink, uint32 iSeq );
	/** 
		PAgent에 클라이언의 접속제한 할때 호출된다.
		@param spLink PAgent의 NetLink
		@param bBlock 클라이언트 블럭 여부
	*/
	bool OnServerBlockClient( NetLinkPtr spLink, bool bBlock );
	/** 
		PAgent에 클라이언의 접속수를 할때 호출된다.
		@param spLink PAgent의 NetLink
		@param iMaxClient 최대접속수
	*/
	bool OnServerMaxClient( NetLinkPtr spLink, uint32 iMaxClient );
};