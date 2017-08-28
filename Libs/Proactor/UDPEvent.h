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

//< UDP ��Ŷ ���� �̺�Ʈ
class UDPRecvEvent : public IOContext
{
public:
	Buffer*			m_pBuf;				///< ���� ����
	UDPLinkPtr		m_spLink;			///< UDP Link
	SOCKADDR_IN		m_RemoteAddr;		///< ���� �ּҰ�
	NetCallback*	m_pNetCallback;

public:
	UDPRecvEvent( UDPLinkPtr spLink );
	virtual ~UDPRecvEvent();
	void	OnHandle( DWORD dwTransferredBytes, DWORD dwErr ) override;
};


//< UDP ��Ŷ ���� �Ϸ� �̺�Ʈ
class UDPSendEvent : public IOContext
{
public:
	Buffer*			m_pBuf;			///< �۽Ź���
	UDPLinkPtr		m_spLink;		///< UDP Link

public:
	UDPSendEvent( UDPLinkPtr spLink );
	virtual ~UDPSendEvent();
	void	OnHandle( DWORD dwTransferredBytes, DWORD dwErr ) override;
};