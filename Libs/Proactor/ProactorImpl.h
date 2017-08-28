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

#include <ThreadFactory.h>
#include <ProactorFactory.h>
#include <Sync.h>
#include "TCPLink.h"
#include "UDPLink.h"

typedef std::list<spRef<Thread> > ProactorThreadList;

/** 
	Proactor 패턴으로 구현된 네트웍 이벤트 처리 클래스
*/
class ProactorImpl : public Proactor, public NetCallback
{
public:
	enum PostMsg
	{
		eProactor_FIN = 0xFFFFFFFF,
		eProactor_LNK = 0xFFFFFFFE,
	};

	TNID			m_iUniqueID;

protected:
	unsigned int		m_iID;
	HANDLE				m_hIOCP;
	Sync				m_Sync;
	NetLinkMap			m_ListenMap;
	NetLinkMap			m_ConnectMap;
	NetLinkMap			m_RemoteMap;
	NetLinkMap			m_UDPMap;
	volatile bool		m_bOnShutdown;
	ProactorThreadList	m_Threads;

	ProactorImpl(const ProactorImpl& r) {}

public:
	ProactorImpl();
	virtual ~ProactorImpl();

	//< Proactor
	unsigned int	ID( void ) override { return m_iID; }
	void			ID( unsigned int iID ) { m_iID = iID; }
	bool			Listen( const Proactor::Property& rProp ) override;
	NetLinkPtr		Connect( const Proactor::Property& rProp ) override;
	NetLinkPtr		Bind( const Proactor::Property& rProp ) override;
	void			KickAll( unsigned short iKickReason ) override;
	void			Finalize( void ) override;
	HANDLE			GetHandle( void ) override;
	NetLinkMap&		GetNetLinkMap( unsigned short iLinkType ) override;
	bool			RegLink( NetLinkPtr spLink ) override;
	bool			UnregLink( NetLinkPtr spLink ) override;
	size_t			GetCount( unsigned short iLinkType ) override;

	//<
	bool			Create( unsigned short iBindCPU, unsigned short iThreadCnt );
	void			Resume( void );
	void			Wait( void );
	void			CloseListener( void );

	//< NetCallback
	void			OnNetAccepted( NetLinkPtr spLink ) override;
	void			OnNetConnected( NetLinkPtr spLink ) override;
	void			OnNetReceived( NetLinkPtr spLink, Buffer* pBuf ) override;
	void			OnNetClosed( NetLinkPtr spLink ) override;

	//< thread main loop
	void			Running( void );

	//< call by ProactorFactory
	bool			OnKickAll( void );
};

