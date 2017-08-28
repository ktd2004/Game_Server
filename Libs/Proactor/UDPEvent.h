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

//<
#include "IOContext.h"
#include "UDPLink.h"

//< UDP 패킷 수신 이벤트
class UDPRecvEvent : public IOContext
{
public:
	Buffer*			m_pBuf;				///< 수신 버퍼
	UDPLinkPtr		m_spLink;			///< UDP Link
	SOCKADDR_IN		m_RemoteAddr;		///< 원격 주소값
	NetCallback*	m_pNetCallback;

public:
	UDPRecvEvent( UDPLinkPtr spLink );
	virtual ~UDPRecvEvent();
	void	OnHandle( DWORD dwTransferredBytes, DWORD dwErr ) override;
};


//< UDP 패킷 전송 완료 이벤트
class UDPSendEvent : public IOContext
{
public:
	Buffer*			m_pBuf;			///< 송신버퍼
	UDPLinkPtr		m_spLink;		///< UDP Link

public:
	UDPSendEvent( UDPLinkPtr spLink );
	virtual ~UDPSendEvent();
	void	OnHandle( DWORD dwTransferredBytes, DWORD dwErr ) override;
};