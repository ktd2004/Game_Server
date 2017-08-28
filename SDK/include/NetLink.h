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
	RPC.NET���� NetLink ������ �ο��Ǵ� ����ID�� Ÿ���� ����
*/
typedef uint64		TNID;

/** 
	NetLink �� ������ �ּҸ� �����ϱ� ���ؼ� ���ȴ�.
*/
struct Network_IF
{
	TNID				m_iNID;		///< ��Ʈ�� ID
	unsigned long		m_iAddr;	///< ȣ��Ʈ �ּ�
	unsigned short		m_iPort;	///< ��Ʈ
	Network_IF();
	Network_IF( TNID ID );
	Network_IF( TNID ID, const std::string& sHost, unsigned short iPort );
};

/** 
	NetLink ������ ������ ���
*/
enum eLinkType
{
	eLINKTYPE_NONE,			///< �ʱ����
	eLINKTYPE_P2P,			///< P2P ��ũ
	eLINKTYPE_UDP,			///< UDP ��ũ
	eLINKTYPE_TCP_LISTEN,	///< ������ ���� ��� ��ũ
	eLINKTYPE_TCP_REMOTE,	///< ������ ������ Ŭ���̾�Ʈ�� ���ῡ �����Ǵ� ��ũ
	eLINKTYPE_TCP_CONNECT,	///< Ŭ���̾�Ʈ ��ũ
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
	NetLink�� ���¸� ������ ���
*/
enum eLinkST
{
	eLINKST_NONE,		///< �ʱ� ����

	//< P2P
	eLINKST_UNLINK,		///< Ȧ��Ī ����
	eLINKST_SYNC,		///< Ȧ��Ī ��û
	eLINKST_SYNC_ACK,	///< Ȧ��Ī ��û�� ���� ����
	eLINKST_LINK,		///< Ȧ��Ī ����

	//< UDP
	eLINKST_BIND,		///< UDP ��Ĺ ���ε�

	//< TCP
	eLINKST_LISTENING,	///< ���Ӵ��
	eLINKST_SRUNNING,	///< ������ ����� Ŭ���̾�Ʈ �ʱ�ȭ �Ϸ�
	eLINKST_CRUNNING,	///< ���� ������ ����� Ŭ���̾�Ʈ �ʱ�ȭ �Ϸ�
	eLINKST_ClOSING,	///< ����ó����
	eLINKST_SHUTDOWN,	///< �����˴ٿ�

	//<
	eLINKST_FIN,		///< ����Ϸ�
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
	NetLink ���� �ڵ带 ������ ���
*/
enum eKickReason
{
	// system reserved
	eKICK_ADMIN,			///< ��ڰ� ������ ����
	eKICK_NORMAL,			///< Ŭ���̾�Ʈ�� ����
	eKICK_SHUTDOWN,			///< �����˴ٿ����� ���� ����
	eKICK_INVALID_LENGTH,	///< ��Ŷ�� ���� ������ ���� ����
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
	Ŭ���̾�Ʈ�� �������� TCP/IP ������ �ǰų� UDP ����, P2P ��� ������ �Ϸ�Ǹ� ������ ���� ����Ÿ�� �ۼ����� ����
	��Ʈ�� ��ü�� NetLink �� �����ȴ�. NetLink�� ��Ʈ�� �ּ�, ���� ������ �����ϰ� ������, ���� ��󿡰� ����Ÿ��
	�ۼ����ϱ� ���� �Լ����� �����Ѵ�.
*/
class NetLink : public shared_ref, public Network_IF
{
public:
	long			m_iSt;				///< NetLink ����

protected:
	interface_ref*	m_pUserData;		///< ����ڵ���Ÿ
	unsigned short	m_iUserDataType;	///< ����ڵ���Ÿ����
	size_t			m_iMaxBufSize;		///< �ۼ��� ���� �ִ� ũ��(bytes)
	eLinkType		m_iLinkType;		///< NetLink ����

public:
	NetLink();
	NetLink(const NetLink& r);
	virtual ~NetLink();


	/** ��Ʈ�� �ּ� ���� ��ȸ�մϴ�. */
	inline Network_IF& NetIF( void ) { return *(Network_IF*)this; }
	/**
		��Ʈ�� ���� �����մϴ�.
		@param nif ��Ʈ�� �ּҰ� ����
	*/
	void NetIF( const Network_IF& nif );
	/**
		NetLink ���� ������ ��ȸ�Ѵ�.
	*/
	inline eLinkST NetST( void ) { return (eLinkST)m_iSt; }
	/**
		NetLink ���� ������ �����Ѵ�.
		@param st NetLink�� ���� ���
		@li eLINKST_NONE,		�ʱ� ����
		@li eLINKST_UNLINK,		p2p Ȧ��Ī ����
		@li eLINKST_SYNC,		p2p Ȧ��Ī ��û
		@li eLINKST_SYNC_ACK,	p2p Ȧ��Ī ��û�� ���� ����
		@li eLINKST_LINK,		p2p Ȧ��Ī ����
		@li eLINKST_BIND,		udp ��Ĺ ���ε�.
		@li eLINKST_LISTENING,	tcp ���Ӵ��.
		@li eLINKST_SRUNNING,	tcp ������ũ����.
		@li eLINKST_CRUNNING,	tcp Ŭ���̾�Ʈ��ũ����.
		@li eLINKkST_ClOSING,	tcp ����ó��.
		@li eLINKST_FIN,		tcp ��ũ����Ϸ�.
	*/
	void	NetST( const eLinkST st );
	/**
		NetLink�� ������ ����� ����Ÿ�� �ݵ�� interface_ref �� ��ӹ��� ��ü�� �ּҰ��� �����ؾ� �Ѵ�.
		NetLink�� ����ڵ���Ÿ�� ����ī��Ʈ�� �����Ͽ� NetLink�� �����ɶ� ����ڵ���Ÿ�� �������θ� �����Ѵ�.
		@param pUserData NetLink�� ������ ����ڵ���Ÿ
	*/
	void UserData( interface_ref *pUserData, unsigned short iUserDataType = 0 );
	/**
		����� ����Ÿ�� ��ȸ�մϴ�.
	*/
	inline interface_ref* UserData( void ) { return m_pUserData; }
	/** ����� ����Ÿ ���� �����մϴ�. */
	inline void UserDataType(unsigned short iUserDataType) { m_iUserDataType = iUserDataType; }
	/** ����� ����Ÿ ���� ��ȸ�մϴ�. */
	inline unsigned short UserDataType( void ) { return m_iUserDataType; }
	/**
		����Ÿ�� �ۼ����� ������ �ִ� ũ�⸦ �����Ѵ�.
		@param iMaxBufSize ����Ÿ �ۼ��� ������ ũ��
	*/
	inline void MaxBufSize( size_t iMaxBufSize ) { m_iMaxBufSize = iMaxBufSize; }
	/**
		����Ÿ�� �ۼ����� ������ �ִ� ũ�� ��ȸ�Ѵ�.
	*/
	inline size_t MaxBufSize( void ) { return m_iMaxBufSize; }
	/**
		NetLink ������ ��ȸ�Ѵ�.
	*/
	inline eLinkType LinkType( void ) { return (eLinkType)m_iLinkType; }
	/**
		NetLink ������ �����մϴ�.
		@param lt NetLink�� ����
		@li eLINKTYPE_NONE			�ʱ����
		@li eLINKTYPE_P2P			P2P ��ũ
		@li eLINKTYPE_UDP			UDP ��ũ
		@li eLINKTYPE_TCP_LISTEN	������ ���� ��� ��ũ
		@li eLINKTYPE_TCP_REMOTE	�������� Ŭ���̾�Ʈ�� �����Ǵ� ��ũ
		@li eLINKTYPE_TCP_CONNECT	Ŭ���̾�Ʈ ��ũ
	*/
	inline void LinkType( const eLinkType lt ) { m_iLinkType = lt; }
	/** 
		TCP/IP�� ������ ���� NetLink�� �ּҸ� ��ȸ�Ѵ�.
	*/
	inline std::string GetAddr( void ) { return NetLink::tostr(&NetIF()); }
	/**
		TCP/IP�� P2P�� ����� ���� ��󿡰� ��Ŷ�� �����Ѵ�.
		@param pPkt ������ ����Ÿ�� ���� �ּҰ�
		@param iLen ������ ����Ÿ�� ����
		@param bOption ����� TCP/IP �� ��� true �� async, false�� sync�� �����Ѵ�.
		����� P2P�� ���� true�� reliable, false�� unreliable�� �����Ѵ�.
	*/
	virtual bool Send( const char* pPkt, unsigned int iLen, bool bOption = true ) { return false; }
	/**
		TCP/IP�� P2P�� ����� ���� ��󿡰� ��Ŷ�� �����Ѵ�.
		@param pBuf ������ ����Ÿ�� �����ϰ� �ִ� ������ �ּҰ�
		@param bOption ����� TCP/IP �� ��� true �� async, false�� sync�� �����Ѵ�.
		����� P2P�� ���� true�� reliable, false�� unreliable�� �����Ѵ�.
	*/
	virtual bool Send( Buffer* pBuf, bool bOption = true ) { return false; }
	/**
		TCP/IP�� UDP����� ���� �Ҵ�� NetLink�� �����մϴ�. P2P ����� ���� NetLink�� ������ �� ����.
		@param iKickReason �����ڵ�( �Ʒ��� RPC.NET���� �̹� ���ǵ� �����ڵ��̴� )
		@li eKICK_ADMIN				��ڰ� ������ ����
		@li eKICK_NORMAL			Ŭ�� �����
		@li eKICK_SHUTDOWN			�����˴ٿ����� ���� ����
		@li eKICK_INVALIDLENGTH		��Ŷ�� ���� ������ ���� ����

	*/
	virtual void Kick( unsigned short iKickReason, const char* pFunc = nullptr ) { }
	/**
		UDP�� ���� ��󿡰� ��Ŷ�� �����Ѵ�.
		@param pPkt ������ ����Ÿ�� ���� �ּҰ�
		@param iLen ������ ����Ÿ�� ����
		@param to ���� �ּҰ�
	*/
	virtual bool Send( const char* pPkt, unsigned int iLen, SOCKADDR_IN* to ) { return false; }
	/**
		UDP�� ���� ��󿡰� ��Ŷ�� �����Ѵ�.
		@param pBuf ������ ����Ÿ�� �����ϰ� �ִ� ������ �ּҰ�
		@param to ���� �ּҰ�
	*/
	virtual bool Send( Buffer* pBuf, SOCKADDR_IN* to ) { return false; }
	/**
		TCP/IP�� ���ݿ��� ������ NetLink�� ����ó���Ѵ�.
	*/
	virtual bool Close( void ) { return false; }
	/** 
		��Ʈ�� �ּ� ������ ip ���ڿ��� ��ȯ�Ѵ�.
		@param pNIF ��Ʈ������ ����ü�� �ּҰ�
	*/
	static std::string nif2ip( const Network_IF* pNIF );
	/**
		��Ʈ�� �ּ� ������ port�� ��ȯ�Ѵ�.
		@param pNIF ��Ʈ������ ����ü�� �ּҰ�
	*/
	static unsigned short nif2port( const Network_IF* pNIF );
	/**
		ip���ڿ��� network address �� ��ȯ�Ѵ�.
		@param sHost IP���ڿ��̳� ������ �ּҰ� ex) "127.0.0.1" or "localhost"
	*/
	static unsigned long ip2addr( const std::string& sHost );
	/**
		port�� network address �� ��ȯ�Ѵ�.
		@param iPort ��Ʈ
	*/
	static unsigned short port2addr( unsigned short iPort );
	/** 
		��Ʈ���ּ�(ip, port)�� ��Ʈ�� �ּ� ����ü�� ��ȯ�Ѵ�.
		@param pNIF ��Ʈ�� �ּ� ����ü
		@param sHost IP���ڿ��̳� ������ �ּҰ� ex) "127.0.0.1" or "localhost"
		@param iPort ��Ʈ
	*/
	static void toaddr( Network_IF* pNIF, const std::string& sHost, unsigned short iPort );
	/** 
		��Ʈ�� �ּ� ������ SOCKADDR_IN ����ü�� ��ȯ�Ѵ�.
		@param addr SOCKADDR_IN �ּ� ����ü
		@param nif ��Ʈ�� �ּ� ����ü
	*/
	static void toaddr( SOCKADDR_IN& addr, Network_IF& nif );
	/**
		��Ʈ���ּ�(ip, port) �� SOCKADDR_IN ����ü�� ��ȯ�Ѵ�.
		@param addr SOCKADDR_IN �ּ� ����ü
		@param sHost IP���ڿ��̳� ������ �ּҰ� ex) "127.0.0.1" or "localhost"
		@param iPort ��Ʈ
	*/
	static void toaddr( SOCKADDR_IN& addr, const std::string& sHost, unsigned short iPort );
	/**
		��Ʈ�� �ּ� ������ ip:port ���ڿ��� ��ȯ�Ѵ�.
		@param pNIF ��Ʈ�� �ּ� ����ü
	*/
	static std::string tostr( Network_IF* pNIF );
	/**
		SOCKADDR_IN �ּ� ������ ip:port ���ڿ��� ��ȯ�Ѵ�.
		@param pNIF SOCKADDR_IN �ּ� ����ü
	*/
	static std::string tostr( SOCKADDR_IN* addr );
};

/** 
	RPC.NET���� ������ NetLink �� �����ϴ� �� Ÿ��
*/
typedef std::map<TNID, NetLinkPtr>		NetLinkMap;