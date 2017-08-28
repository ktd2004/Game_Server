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
#include "stdafx.h"
//<
#include <NetCallback.h>
#include <Performance.h>
#include <Errorcodes.h>
#include <Logger.h>
#include "NetUtil.h"
#include "UDPEvent.h"

namespace
{
	Proactor_Counter& g_proactor_counter = PROACTOR_COUNTER;
}

//<
UDPRecvEvent::UDPRecvEvent( UDPLinkPtr spLink )
: m_pBuf(new Buffer(spLink->MaxBufSize()))
{ 
	prn_sys("this = %p", this);
	m_spLink = spLink;
	m_pNetCallback = spLink->m_pNetCallback;
	locked_inc( g_proactor_counter.iUDPReceiveCtxs );
}

UDPRecvEvent::~UDPRecvEvent()
{
	prn_sys("this = %p", this);
	locked_dec( g_proactor_counter.iUDPReceiveCtxs );
	if ( m_pBuf ) m_pBuf->release();
}

void UDPRecvEvent::OnHandle( DWORD dwTransferredBytes, DWORD dwErr )
{
//	prn_sys( "dwTransferredBytes (%d)", dwTransferredBytes );

	if ( dwTransferredBytes > 0 )
	{
		locked_exchange_add( g_proactor_counter.iUDPReceiveBytes, (long)dwTransferredBytes );
		locked_inc( g_proactor_counter.iUDPReceives );

		m_spLink.param( &m_RemoteAddr );
		m_pBuf->wr_ptr( dwTransferredBytes );
		m_pNetCallback->OnNetReceived( m_spLink, m_pBuf );
	}

	if ( m_spLink->NetST() != eLINKST_FIN && m_spLink->Recv(this) )
	{
		return;
	}

	SetLastError( PROACTOR_CLOSED_NETLINK );
	m_pNetCallback->OnNetClosed( m_spLink );
	delete this;
}

//<
UDPSendEvent::UDPSendEvent( UDPLinkPtr spLink )
{
	m_spLink = spLink;
	locked_inc( g_proactor_counter.iUDPSentCtxs );
}

UDPSendEvent::~UDPSendEvent()
{
	prn_sys("this = %p", this);
	locked_dec( g_proactor_counter.iUDPSentCtxs );
	m_pBuf->release();
}

void UDPSendEvent::OnHandle( DWORD dwTransferredBytes, DWORD dwErr )
{
	locked_exchange_add( g_proactor_counter.iUDPSentBytes, (long)dwTransferredBytes );
	locked_inc( g_proactor_counter.iUDPSents );
	if ( dwTransferredBytes != m_pBuf->length() )
	{
		SetLastError( PROACTOR_SEND_PACKET_FAILED );
	}
	delete this;
}