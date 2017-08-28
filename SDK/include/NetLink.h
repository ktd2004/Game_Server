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

#include <Buffer.h>


/** 
	RPC.NET에서 NetLink 생성시 부여되는 고유ID의 타입을 선언
*/
typedef uint64		TNID;

/** 
	NetLink 의 원격지 주소를 저장하기 위해서 사용된다.
*/
struct Network_IF
{
	TNID				m_iNID;		///< 네트웍 ID
	unsigned long		m_iAddr;	///< 호스트 주소
	unsigned short		m_iPort;	///< 포트
	Network_IF();
	Network_IF( TNID ID );
	Network_IF( TNID ID, const std::string& sHost, unsigned short iPort );
};

/** 
	NetLink 종류를 정의한 상수
*/
enum eLinkType
{
	eLINKTYPE_NONE,			///< 초기상태
	eLINKTYPE_P2P,			///< P2P 링크
	eLINKTYPE_UDP,			///< UDP 링크
	eLINKTYPE_TCP_LISTEN,	///< 서버의 접속 대기 링크
	eLINKTYPE_TCP_REMOTE,	///< 서버에 접속한 클라이언트의 연결에 대응되는 링크
	eLINKTYPE_TCP_CONNECT,	///< 클라이언트 링크
	eLINKTYPE_UNKNOWN,
};

inline const char* DEBUG_LINK_TYPE( unsigned short iLinkType )
{
	static const char* g_LinkType[] = {
		"eLINKTYPE_NONE",
		"eLINKTYPE_P2P",
		"eLINKTYPE_UDP",
		"eLinkType_TCP_LISTEN",
		"eLINKTYPE_TCP_REMOTE",
		"eLINKTYPE_TCP_CONNECT",
		"eLINKTYPE_UNKNOWN", };
	if ( sizeof(g_LinkType)/sizeof(const char*) <= iLinkType )
		iLinkType = eLINKTYPE_UNKNOWN;

	return g_LinkType[iLinkType];
}

/** 
	NetLink의 상태를 정의한 상수
*/
enum eLinkST
{
	eLINKST_NONE,		///< 초기 상태

	//< P2P
	eLINKST_UNLINK,		///< 홀펀칭 실패
	eLINKST_SYNC,		///< 홀펀칭 요청
	eLINKST_SYNC_ACK,	///< 홀펀칭 요청에 대한 응답
	eLINKST_LINK,		///< 홀펀칭 성공

	//< UDP
	eLINKST_BIND,		///< UDP 소캣 바인드

	//< TCP
	eLINKST_LISTENING,	///< 접속대기
	eLINKST_SRUNNING,	///< 서버에 연결된 클라이언트 초기화 완료
	eLINKST_CRUNNING,	///< 원격 서버에 연결된 클라이언트 초기화 완료
	eLINKST_ClOSING,	///< 종료처리중
	eLINKST_SHUTDOWN,	///< 서버셧다운

	//<
	eLINKST_FIN,		///< 종료완료
	eLINKST_UNKNOWN,
};

inline const char* DEBUG_LINK_STATE( unsigned short iLinkST )
{
	static const char *g_LinkStText[] = {
		"eLINKST_NONE",
		"eLINKST_UNLINK",
		"eLINKST_SYNC",
		"eLINKST_SYNC_ACK",
		"eLINKST_LINK",
		"eLINKST_BIND",
		"eLINKST_LISTENING",
		"eLINKST_SRUNNING",
		"eLINKST_CRUNNING",
		"eLINKkST_ClOSING",
		"eLINKkST_SHUTDOWN",
		"eLINKST_FIN",
		"eLINKST_UNKNOWN", };

	if ( sizeof(g_LinkStText)/sizeof(const char*) <= iLinkST )
		iLinkST = eLINKST_UNKNOWN;

	return g_LinkStText[iLinkST];
}

/** 
	NetLink 종료 코드를 정의한 상수
*/
enum eKickReason
{
	// system reserved
	eKICK_ADMIN,			///< 운영자가 강제로 종료
	eKICK_NORMAL,			///< 클라이언트가 종료
	eKICK_SHUTDOWN,			///< 서버셧다운으로 강제 종료
	eKICK_INVALID_LENGTH,	///< 패킷의 길이 오류로 강제 종료
	// below user defined
	eKICK_TIMEOUT, 
	eKICK_SYSTEM_ERROR,
	eKICK_DATABASE_ERROR,
	eKICK_LOAD_ERROR,
	eKICK_INVALID_MASTER,
	eKICK_INVALID_CHANNEL,
	eKICK_INVALID_ACCOUNT,
	eKICK_INVALID_VERSION,
	eKICK_INVALID_AUTHKEY,
	eKICK_INVALID_SESSION,
	eKICK_ALREADY_AUTH_SESSION,
	eKICK_DUPLICATED_SESSION, 
	eKICK_UNKNOWN,
};


inline const char* DEBUG_KICK_REASON( unsigned short iKickReason )
{
	static const char *g_KickText[] = {
		"eKICK_ADMIN",
		"eKICK_NORMAL",
		"eKICK_SHUTDOWN",
		"eKICK_INVALID_LENGTH",
		// user defined
		"eKICK_TIMEOUT",
		"eKICK_SYSTEM_ERROR",
		"eKICK_DATABASE_ERROR",
		"eKICK_LOAD_ERROR",
		"eKICK_INVALID_MASTER",
		"eKICK_INVALID_CHANNEL",
		"eKICK_INVALID_ACCOUNT",
		"eKICK_INVALID_VERSION",
		"eKICK_INVALID_AUTHKEY",
		"eKICK_INVALID_SESSION",
		"eKICK_ALREADY_AUTH_SESSION",
		"eKICK_DUPLICATED_SESSION",
		"eKICK_UNKNOWN", };

		if ( sizeof(g_KickText)/sizeof(const char*) <= iKickReason )
			iKickReason = (unsigned short)sizeof(g_KickText)/sizeof(const char*) - 1;

		return g_KickText[iKickReason];
}

PTR(NetLink);
/** 
	클라이언트와 서버간에 TCP/IP 연결이 되거나 UDP 소켓, P2P 통신 설정이 완료되면 원격의 대상과 데이타를 송수신을 위해
	네트웍 객체인 NetLink 를 생성된다. NetLink는 네트웍 주소, 상태 정보를 저장하고 있으며, 연결 대상에게 데이타를
	송수신하기 위한 함수들을 제공한다.
*/
class NetLink : public shared_ref, public Network_IF
{
public:
	long			m_iSt;				///< NetLink 상태

protected:
	interface_ref*	m_pUserData;		///< 사용자데이타
	unsigned short	m_iUserDataType;	///< 사용자데이타종류
	size_t			m_iMaxBufSize;		///< 송수신 버퍼 최대 크기(bytes)
	eLinkType		m_iLinkType;		///< NetLink 종류

public:
	NetLink();
	NetLink(const NetLink& r);
	virtual ~NetLink();


	/** 네트웍 주소 정보 조회합니다. */
	inline Network_IF& NetIF( void ) { return *(Network_IF*)this; }
	/**
		네트웍 정보 설정합니다.
		@param nif 네트웍 주소값 정보
	*/
	void NetIF( const Network_IF& nif );
	/**
		NetLink 상태 정보를 조회한다.
	*/
	inline eLinkST NetST( void ) { return (eLinkST)m_iSt; }
	/**
		NetLink 상태 정보를 설정한다.
		@param st NetLink의 상태 상수
		@li eLINKST_NONE,		초기 상태
		@li eLINKST_UNLINK,		p2p 홀펀칭 실패
		@li eLINKST_SYNC,		p2p 홀펀칭 요청
		@li eLINKST_SYNC_ACK,	p2p 홀펀칭 요청에 대한 응답
		@li eLINKST_LINK,		p2p 홀펀칭 성공
		@li eLINKST_BIND,		udp 소캣 바인드.
		@li eLINKST_LISTENING,	tcp 접속대기.
		@li eLINKST_SRUNNING,	tcp 서버링크시작.
		@li eLINKST_CRUNNING,	tcp 클라이언트링크시작.
		@li eLINKkST_ClOSING,	tcp 종료처리.
		@li eLINKST_FIN,		tcp 링크종료완료.
	*/
	void	NetST( const eLinkST st );
	/**
		NetLink에 저장할 사용자 데이타는 반드시 interface_ref 를 상속받은 객체의 주소값을 전달해야 한다.
		NetLink는 사용자데이타의 참조카운트를 관리하여 NetLink가 삭제될때 사용자데이타의 삭제여부를 결정한다.
		@param pUserData NetLink에 저장할 사용자데이타
	*/
	void UserData( interface_ref *pUserData, unsigned short iUserDataType = 0 );
	/**
		사용자 데이타를 조회합니다.
	*/
	inline interface_ref* UserData( void ) { return m_pUserData; }
	/** 사용자 데이타 종류 설정합니다. */
	inline void UserDataType(unsigned short iUserDataType) { m_iUserDataType = iUserDataType; }
	/** 사용자 데이타 종류 조회합니다. */
	inline unsigned short UserDataType( void ) { return m_iUserDataType; }
	/**
		데이타를 송수신할 버퍼의 최대 크기를 설정한다.
		@param iMaxBufSize 데이타 송수신 버퍼의 크기
	*/
	inline void MaxBufSize( size_t iMaxBufSize ) { m_iMaxBufSize = iMaxBufSize; }
	/**
		데이타를 송수신할 버퍼의 최대 크기 조회한다.
	*/
	inline size_t MaxBufSize( void ) { return m_iMaxBufSize; }
	/**
		NetLink 종류를 조회한다.
	*/
	inline eLinkType LinkType( void ) { return (eLinkType)m_iLinkType; }
	/**
		NetLink 종류를 설정합니다.
		@param lt NetLink의 종류
		@li eLINKTYPE_NONE			초기상태
		@li eLINKTYPE_P2P			P2P 링크
		@li eLINKTYPE_UDP			UDP 링크
		@li eLINKTYPE_TCP_LISTEN	서버의 접속 대기 링크
		@li eLINKTYPE_TCP_REMOTE	서버에서 클라이언트에 대응되는 링크
		@li eLINKTYPE_TCP_CONNECT	클라이언트 링크
	*/
	inline void LinkType( const eLinkType lt ) { m_iLinkType = lt; }
	/** 
		TCP/IP로 접속한 원격 NetLink의 주소를 조회한다.
	*/
	inline std::string GetAddr( void ) { return NetLink::tostr(&NetIF()); }
	/**
		TCP/IP나 P2P로 연결된 원격 대상에게 패킷을 전송한다.
		@param pPkt 전송할 데이타의 시작 주소값
		@param iLen 전송할 데이타의 길이
		@param bOption 대상이 TCP/IP 일 경우 true 면 async, false면 sync로 동작한다.
		대상이 P2P일 경우는 true면 reliable, false면 unreliable로 전송한다.
	*/
	virtual bool Send( const char* pPkt, unsigned int iLen, bool bOption = true ) { return false; }
	/**
		TCP/IP나 P2P로 연결된 원격 대상에게 패킷을 전송한다.
		@param pBuf 전송할 데이타를 저장하고 있는 버퍼의 주소값
		@param bOption 대상이 TCP/IP 일 경우 true 면 async, false면 sync로 동작한다.
		대상이 P2P일 경우는 true면 reliable, false면 unreliable로 전송한다.
	*/
	virtual bool Send( Buffer* pBuf, bool bOption = true ) { return false; }
	/**
		TCP/IP나 UDP통신을 위해 할당된 NetLink을 종료합니다. P2P 통신을 위한 NetLink는 종료할 수 없다.
		@param iKickReason 종료코드( 아래는 RPC.NET에서 이미 정의된 종료코드이다 )
		@li eKICK_ADMIN				운영자가 강제로 종료
		@li eKICK_NORMAL			클라가 종료됨
		@li eKICK_SHUTDOWN			서버셧다운으로 강제 종료
		@li eKICK_INVALIDLENGTH		패킷의 길이 오류로 강제 종료

	*/
	virtual void Kick( unsigned short iKickReason, const char* pFunc = nullptr ) { }
	/**
		UDP로 원격 대상에게 패킷을 전송한다.
		@param pPkt 전송할 데이타의 시작 주소값
		@param iLen 전송할 데이타의 길이
		@param to 원격 주소값
	*/
	virtual bool Send( const char* pPkt, unsigned int iLen, SOCKADDR_IN* to ) { return false; }
	/**
		UDP로 원격 대상에게 패킷을 전송한다.
		@param pBuf 전송할 데이타를 저장하고 있는 버퍼의 주소값
		@param to 원격 주소값
	*/
	virtual bool Send( Buffer* pBuf, SOCKADDR_IN* to ) { return false; }
	/**
		TCP/IP로 원격에서 접속한 NetLink를 종료처리한다.
	*/
	virtual bool Close( void ) { return false; }
	/** 
		네트웍 주소 정보를 ip 문자열로 변환한다.
		@param pNIF 네트웍정보 구조체의 주소값
	*/
	static std::string nif2ip( const Network_IF* pNIF );
	/**
		네트웍 주소 정보를 port로 변환한다.
		@param pNIF 네트웍정보 구조체의 주소값
	*/
	static unsigned short nif2port( const Network_IF* pNIF );
	/**
		ip문자열을 network address 로 변환한다.
		@param sHost IP문자열이나 도메인 주소값 ex) "127.0.0.1" or "localhost"
	*/
	static unsigned long ip2addr( const std::string& sHost );
	/**
		port을 network address 로 변환한다.
		@param iPort 포트
	*/
	static unsigned short port2addr( unsigned short iPort );
	/** 
		네트웍주소(ip, port)를 네트웍 주소 구조체로 변환한다.
		@param pNIF 네트웍 주소 구조체
		@param sHost IP문자열이나 도메인 주소값 ex) "127.0.0.1" or "localhost"
		@param iPort 포트
	*/
	static void toaddr( Network_IF* pNIF, const std::string& sHost, unsigned short iPort );
	/** 
		네트웍 주소 정보를 SOCKADDR_IN 구조체로 변환한다.
		@param addr SOCKADDR_IN 주소 구조체
		@param nif 네트웍 주소 구조체
	*/
	static void toaddr( SOCKADDR_IN& addr, Network_IF& nif );
	/**
		네트웍주소(ip, port) 를 SOCKADDR_IN 구조체로 변환한다.
		@param addr SOCKADDR_IN 주소 구조체
		@param sHost IP문자열이나 도메인 주소값 ex) "127.0.0.1" or "localhost"
		@param iPort 포트
	*/
	static void toaddr( SOCKADDR_IN& addr, const std::string& sHost, unsigned short iPort );
	/**
		네트웍 주소 정보를 ip:port 문자열로 변환한다.
		@param pNIF 네트웍 주소 구조체
	*/
	static std::string tostr( Network_IF* pNIF );
	/**
		SOCKADDR_IN 주소 정보를 ip:port 문자열로 변환한다.
		@param pNIF SOCKADDR_IN 주소 구조체
	*/
	static std::string tostr( SOCKADDR_IN* addr );
};

/** 
	RPC.NET에서 생성된 NetLink 를 관리하는 맵 타입
*/
typedef std::map<TNID, NetLinkPtr>		NetLinkMap;