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

//<
PTR(UDPLink);
class UDPLink : public NetLink
{
	friend class ProactorImpl;
	friend class NetEventHandler;
	friend class UDPRecvEvent;
	friend class UDPSendEvent;

private:
	SOCKET			m_hSocket;			//< ³» ¼ÒÄÏ
	NetCallback*	m_pNetCallback;
	ProactorImpl*	m_pProactor;

	UDPLink( const UDPLink& r ) {}

public:
	UDPLink( ProactorImpl* pProactor );
	virtual ~UDPLink();

	bool operator == (UDPLink &ref);
	bool operator != (UDPLink &ref);

	//< NetLink
	bool			Send( const char* pPkt, unsigned int iLen, SOCKADDR_IN* to ) override;
	bool			Send( Buffer* pBuf, SOCKADDR_IN* to ) override;
	void			Kick( unsigned short iKickReason, const char* pFunc = nullptr ) override;

protected:
	bool			Bind( const Proactor::Property& rProp );
	bool			Recv( void );
	bool			Recv( UDPRecvEvent* pCtx );
	void			SetBind( void );
	void			SetFin( void );
};

