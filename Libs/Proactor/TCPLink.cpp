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
#include <Performance.h>
#include <Logger.h>
#include "NetUtil.h"
#include "TCPEvent.h"
#include "ProactorImpl.h"

namespace
{
	Proactor_Counter& g_proactor_counter = PROACTOR_COUNTER;
}

//<
TCPLink::TCPLink( ProactorImpl* pProactor, TCPLinkPtr spListen )
: m_pProactor(pProactor)
, m_spListen(spListen)
, m_hSocket(INVALID_SOCKET)
, m_iCreatedSockets(0)
{
	NetST(eLINKST_NONE);
	m_iNID = locked_inc(pProactor->m_iUniqueID);
	m_pNetCallback = m_spListen ? m_spListen->m_pNetCallback : nullptr;
	m_hSocket = Socket::New( Socket::eTCP );
	locked_inc( g_proactor_counter.iTCPNetLinks );
}

TCPLink::~TCPLink()
{
	Socket::Delete( m_hSocket, Socket::eTCP );
	prn_sys("%s, %s", DEBUG_LINK_TYPE(LinkType()), DEBUG_LINK_STATE(NetST()));
	locked_dec( g_proactor_counter.iTCPNetLinks );
}

bool TCPLink::operator == (TCPLink &ref)
{
	istrue_ret(
		ref.m_hSocket == m_hSocket && ref.m_iNID == m_iNID,
		true );
	return false;
}

bool TCPLink::operator != (TCPLink &ref)
{
	isfalse_ret( ref == *this, true );
	return false;
}

bool TCPLink::Send( const char* pPkt, unsigned int iLen, bool bOption )
{
	Buffer* pBuf = new Buffer( pPkt, iLen );
	return Send(pBuf, bOption);
}


bool TCPLink::Send( Buffer* pBuf, bool bOption )
{
// 모바일 환경에서 이상하게 문제가 발생해서 막는다.
// 	if ( m_PktQ.is_sending() )
// 	{
// 		prn_sys( "push pkt into sending queue" );
// 		m_PktQ.push( pBuf );
// 		return true;
// 	}

	TCPSendEvent *pCtx = new TCPSendEvent( this );
	isfalse_ret( pCtx, false );

	pCtx->m_pBuf = pBuf;
	WSABUF wsaBuf;
	wsaBuf.buf = pBuf->rd_ptr();
	wsaBuf.len = pBuf->size();

	if ( bOption == true )
	{
		isfalse_do_ret(
			Socket::AsyncSend(m_hSocket, wsaBuf, pCtx->m_OV),
			delete pCtx,
			false );
	}
	else
	{
		isfalse_do_ret(
			Socket::SyncSend(m_hSocket, wsaBuf, pCtx->m_OV),
			delete pCtx,
			false );
	}

	return true;
}

bool TCPLink::Listen( const Proactor::Property& rProp )
{
	istrue_ret( m_hSocket == INVALID_SOCKET, false );

	m_pNetCallback = rProp.pNetCallback;

	SetListening();
	LinkType( eLINKTYPE_TCP_LISTEN );

	m_iAddr = NetLink::ip2addr( rProp.sHost );
	m_iPort = NetLink::port2addr( rProp.iPort );

	istrue_ret(
		false == Socket::Listen(m_hSocket, rProp.sHost, rProp.iPort),
		false );
	istrue_ret(
		false == IOCP::Associate(m_pProactor->GetHandle(), m_hSocket, (ULONG_PTR)ProactorImpl::eProactor_LNK),
		false );

	return true;
}

bool TCPLink::Connect( const Proactor::Property& rProp )
{
	istrue_ret( INVALID_SOCKET == m_hSocket, false );

	Network_IF& rNIF = NetIF();
	rNIF.m_iAddr = NetLink::ip2addr(rProp.sHost);
	rNIF.m_iPort = NetLink::port2addr(rProp.iPort);

	m_pNetCallback = rProp.pNetCallback;
	LinkType( eLINKTYPE_TCP_CONNECT );

	TCPConnectEvent* pCtx = new TCPConnectEvent( this );
	isfalse_ret( pCtx, false );

	istrue_goto(
		false == IOCP::Associate(m_pProactor->GetHandle(), m_hSocket, (ULONG_PTR)ProactorImpl::eProactor_LNK),
		error_connect );

	istrue_goto(
		Socket::ConnectEx(m_hSocket, &pCtx->m_OV, rProp.iPort, rProp.sHost),
		error_connect );

	return true;

error_connect :
	safe_delete(pCtx);
	return false;
}

bool TCPLink::Prepare( void )
{
	TCPLinkPtr spListen;
	switch (LinkType())
	{
	case eLINKTYPE_TCP_REMOTE :
		spListen = m_spListen;
		break;
	case eLINKTYPE_TCP_LISTEN :
		spListen = this;
		break;
	default :
		return false;
	}

	TCPAcceptEvent *pCtx = nullptr;
	if ( eLINKST_FIN != spListen->m_iSt )
	{
		TCPLinkPtr spLink = new TCPLink( m_pProactor, spListen );
		isfalse_goto( spLink, error_accept );	// spLink 삭제되면 TCPLink::~TCPLink()에서 hSocket 삭제
		spLink->LinkType( eLINKTYPE_TCP_REMOTE );
		spLink->MaxBufSize( spListen->MaxBufSize() );

		pCtx = new TCPAcceptEvent( spLink );
		isfalse_goto( pCtx, error_accept );

		istrue_goto(
			false == IOCP::Associate(m_pProactor->GetHandle(), spLink->m_hSocket, (ULONG_PTR)pCtx),
			error_accept ); // pCtx 삭제되면 pCtx->m_spLnk 가 삭제되고 NetLink::~NetLinK()에서 hSocket 삭제

		DWORD dwBytes = 0;
		if ( FALSE == Socket::AcceptEx(
			spListen->m_hSocket,			// listen socket
			spLink->m_hSocket,				// accept socket
			pCtx->m_AddrBuf,				// buffer for accept( server, remote address )
			0,								// default 0
			sizeof(sockaddr_in)+16,			// local address length
			sizeof(sockaddr_in)+16,			// remote address length
			&dwBytes,						// received bytes
			&pCtx->m_OV) )					// overlapped
		{
			istrue_goto( ERROR_IO_PENDING != WSAGetLastError(), error_accept );
		}

		locked_inc( spListen->m_iCreatedSockets );
		return true;
	}

error_accept :
	safe_delete(pCtx);
	return false;
}

void TCPLink::Kick( unsigned short iKickReason, const char* pFunc )
{
	if ( iKickReason == eKICK_SHUTDOWN )
	{
		// 원격에서 접속한 소켓은 재사용
		locked_exchange( m_iSt, eLINKST_SHUTDOWN );
		prn_sys("%s, %s, %s",
			nvl_s(pFunc), DEBUG_LINK_TYPE(m_iLinkType), DEBUG_KICK_REASON(iKickReason) );

		if ( eLINKTYPE_UDP == m_iLinkType )
		{
			Socket::Delete( m_hSocket, Socket::eUDP );
		}
		else
		{
			Socket::Delete( m_hSocket, Socket::eTCP );
		}
	}
	else if ( eLINKTYPE_TCP_REMOTE == m_iLinkType && eLINKST_SRUNNING == m_iSt )
	{
		// 원격에서 접속한 소켓은 재사용
		locked_exchange( m_iSt, eLINKST_ClOSING );
		prn_sys("%s, %s, %s",
			nvl_s(pFunc), DEBUG_LINK_TYPE(m_iLinkType), DEBUG_KICK_REASON(iKickReason) );

		TCPUnlinkEvent *pCtx = new TCPUnlinkEvent( this );
		isfalse_ret( pCtx, );
		istrue_do_ret(
			Socket::DisconnectEx(m_hSocket, &pCtx->m_OV),
			delete pCtx, );
	}
	else if ( eLINKST_FIN != m_iSt && eLINKST_ClOSING != m_iSt )
	{
		locked_exchange( m_iSt, eLINKST_FIN );
		prn_sys("%s, %s, %s",
			nvl_s(pFunc), DEBUG_LINK_TYPE(m_iLinkType), DEBUG_KICK_REASON(iKickReason) );

		if ( eLINKTYPE_UDP == m_iLinkType )
		{
			Socket::Delete( m_hSocket, Socket::eUDP );
		}
		else
		{
			Socket::Delete( m_hSocket, Socket::eTCP );
		}
	}
}

bool TCPLink::Close( void )
{
	TCPUnlinkEvent *pCtx = nullptr;

	if ( eLINKTYPE_TCP_REMOTE == m_iLinkType )
	{
		if ( eLINKST_SHUTDOWN != m_iSt && eLINKST_ClOSING != m_iSt )
		{
			pCtx = new TCPUnlinkEvent( this );

			// 원격에서 접속한 소켓은 재사용
			locked_exchange( m_iSt, eLINKST_ClOSING );
			prn_sys("closing %s", DEBUG_LINK_TYPE(m_iLinkType));

			isfalse_ret( pCtx, false);
			istrue_do_ret(
				Socket::DisconnectEx(m_hSocket, &pCtx->m_OV),
				delete pCtx, false );
		}
	}

	return true;
}


void TCPLink::SetListening()
{
	locked_exchange( m_iSt, eLINKST_LISTENING );
}

void TCPLink::SetSRunning()
{
	locked_exchange( m_iSt, eLINKST_SRUNNING );
	locked_dec(m_spListen->m_iCreatedSockets);
}

void TCPLink::SetCRunning()
{
	locked_exchange( m_iSt, eLINKST_CRUNNING );
}

void TCPLink::SetFin( void )
{
	if ( (eLINKTYPE_TCP_REMOTE == m_iLinkType) && (eLINKST_SRUNNING == m_iSt) )
	{
		locked_dec(m_spListen->m_iCreatedSockets);
	}
	locked_exchange( m_iSt, eLINKST_FIN );
	Socket::Delete( m_hSocket, Socket::eTCP );
}

long TCPLink::GetCreatedSocketCount( void )
{
	return m_spListen ? m_spListen->m_iCreatedSockets : m_iCreatedSockets;
}

bool TCPLink::Recv( void )
{
	TCPRecvEvent* pCtx = new TCPRecvEvent( this );
	isfalse_goto(
		pCtx,
		error_recieve );

	WSABUF wsaBuf;
	wsaBuf.buf = pCtx->m_pBuf->wr_ptr();
	wsaBuf.len = pCtx->m_pBuf->space();

	isfalse_goto(
		Socket::Receive(m_hSocket, wsaBuf, pCtx->m_OV),
		error_recieve );

	return true;

error_recieve:
	safe_delete(pCtx);
	return false;
}

bool TCPLink::Recv( TCPRecvEvent* pCtx )
{
	ZeroMemory( &pCtx->m_OV, sizeof(pCtx->m_OV) );
	WSABUF wsaBuf;
	wsaBuf.buf = pCtx->m_pBuf->wr_ptr();
	wsaBuf.len = pCtx->m_pBuf->space();

	isfalse_ret(
		Socket::Receive(m_hSocket, wsaBuf, pCtx->m_OV),
		false );

	return true;
}
