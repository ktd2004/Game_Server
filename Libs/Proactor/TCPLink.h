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
#include <NetLink.h>
#include <ProactorFactory.h>
#include "Lockfreeq.h"

//<
PTR(TCPLink);
class TCPLink : public NetLink
{
	friend class ProactorImpl;
	friend class NetEventHandler;
	friend class TCPAcceptEvent;
	friend class TCPConnectEvent;
	friend class TCPRecvEvent;
	friend class TCPSendEvent;
	friend class TCPUnlinkEvent;

private:
	TCPLinkPtr		m_spListen;			///< 나를 받아들인 Listen Socket
	SOCKET			m_hSocket;			///< 내 소켓
	long			m_iCreatedSockets;	///< Listen Socket 에만 의미가 있다.
										///< 현재의 대기소캣의 개수를 의미한다
	NetCallback*	m_pNetCallback;		
	ProactorImpl*	m_pProactor;
	lock_free_queue m_PktQ;

	TCPLink( const TCPLink& r ) {}

public:
	TCPLink( ProactorImpl* pProactor, TCPLinkPtr spListen = nullptr );
	virtual ~TCPLink();

	bool operator == (TCPLink &ref);
	bool operator != (TCPLink &ref);

	//< NetLink
	bool			Send( const char* pPkt, unsigned int iLen, bool bOption = false ) override;
	bool			Send( Buffer* spBuf, bool bOption = false ) override;
	void			Kick( unsigned short iKickReason, const char* pFunc = nullptr ) override;
	bool			Close( void ) override;

protected:
	bool			Listen( const Proactor::Property& rProp );
	bool			Connect( const Proactor::Property& rProp );
	bool			Prepare( void );
	void			SetListening( void );
	void			SetSRunning( void );
	void			SetCRunning( void );
	void			SetFin( void );
	long			GetCreatedSocketCount( void );
	bool			Recv( void );
	bool			Recv( TCPRecvEvent* pCtx );
};

