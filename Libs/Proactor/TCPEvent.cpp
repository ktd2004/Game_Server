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
#include <Errorcodes.h>
#include <Logger.h>
#include <NetCallback.h>
//<
#include "NetUtil.h"
#include "TCPEvent.h"

namespace
{
	Proactor_Counter& g_proactor_counter = PROACTOR_COUNTER;
}

//<
TCPAcceptEvent::TCPAcceptEvent( TCPLinkPtr spLink )
{
	m_spLink = spLink;
	m_pNetCallback = spLink->m_pNetCallback;
	locked_inc( g_proactor_counter.iTCPAcceptCtxs );
}

TCPAcceptEvent::~TCPAcceptEvent()
{
	prn_sys( "this = %p", this );
	locked_dec( g_proactor_counter.iTCPAcceptCtxs );
}

void TCPAcceptEvent::OnHandle(DWORD dwTransferredBytes, DWORD dwError)
{
	TCPRecvEvent *pCtx = nullptr;
	istrue_goto(
		NOERROR != dwError,
		error_critical );
	isfalse_goto(
		IOCP::InheritAcceptContext(m_spLink->m_spListen->m_hSocket, m_spLink->m_hSocket),
		error_accept );

	sockaddr_in		m_RemoteAddr;		//< 원격 주소값
	int iRemoteLen = sizeof(m_RemoteAddr);
	istrue_goto(
		SOCKET_ERROR == getpeername(m_spLink->m_hSocket,
		(sockaddr*)&m_RemoteAddr, &iRemoteLen),
		error_accept );

	Network_IF& rNIF = m_spLink->NetIF();
	rNIF.m_iAddr = m_RemoteAddr.sin_addr.S_un.S_addr;
	rNIF.m_iPort = m_RemoteAddr.sin_port;

//	prn_sys( "accept remote addr %s", cstr(Socket::SockAddr2String(&m_spLink->m_RemoteAddr)) );

	// change link state
	m_spLink->SetSRunning();
	m_pNetCallback->OnNetAccepted( m_spLink );

	isfalse_goto( m_spLink->Recv(), error_recieve );

	delete this;
	return;

error_accept :
	SetLastError( PROACTOR_REMOTE_NETLINK_ACCEPT_FAILED );
	// IOAcceptEvent.m_spLink is remote link, disconnect remote link
	m_spLink->SetFin();
	safe_delete(pCtx);
	delete this;
	return;

error_recieve :
	SetLastError( PROACTOR_REMOTE_NETLINK_ACCEPT_SUCCESS_BUT_RECEIVE_FAILED );
	// IOAcceptEvent.m_spLink is remote link, disconnect remote link & reuse link
	m_pNetCallback->OnNetClosed( m_spLink );
	safe_delete(pCtx);
	delete this;
	return;

// listen 소켓을 닫으면, 아래와 같은 error 가 발생한다.
// error: err=995 transferedbytes=0 key=FFFFFFFE ov=028E07F4
// ov 는 Accept() 호출시 할당한 IOAcceptEvent
error_critical :
	//prn_sys( "maybe listen socket closed");
	SetLastError( PROACTOR_CLOSED_LISTEN_NETLINK );
	m_spLink->SetFin();
	safe_delete(pCtx);
	delete this;
}

//<
TCPRecvEvent::TCPRecvEvent( TCPLinkPtr spLink )
: m_pBuf(new Buffer(spLink->MaxBufSize()))
{ 
	m_spLink = spLink;
	m_pNetCallback = spLink->m_pNetCallback;
	locked_inc( g_proactor_counter.iTCPReceiveCtxs );
}

TCPRecvEvent::~TCPRecvEvent()
{
	prn_sys("this = %p", this);
	locked_dec( g_proactor_counter.iTCPReceiveCtxs );
	if ( m_pBuf ) m_pBuf->release();
}

void TCPRecvEvent::OnHandle(DWORD dwTransferredBytes, DWORD dwError)
{
	prn_sys("dwTransferredBytes (%d)", dwTransferredBytes);
	isfalse_goto( dwTransferredBytes, error_recieve );

	locked_exchange_add( g_proactor_counter.iTCPReceiveBytes, (long)dwTransferredBytes );
	locked_inc( g_proactor_counter.iTCPReceives );
	m_pBuf->wr_ptr( dwTransferredBytes );
	m_pNetCallback->OnNetReceived( m_spLink, m_pBuf );
	if ( m_spLink->Recv(this) )
	{
		return;
	}

error_recieve:
	prn_sys( "maybe client socket closed");
	SetLastError( PROACTOR_CLOSED_NETLINK );
	m_pNetCallback->OnNetClosed( m_spLink );
	delete this;
}

#define delete_buf_list(x) do{ Buffer* obj = x; while(obj) { Buffer* t = obj; obj = obj->m_next; t->release(); } x = nullptr; }while(0)

//<
TCPSendEvent::TCPSendEvent( TCPLinkPtr spLink )
{
	m_spLink = spLink;
	m_pNetCallback = spLink->m_pNetCallback;
	locked_inc( g_proactor_counter.iTCPSentCtxs );
}

TCPSendEvent::~TCPSendEvent()
{
//	prn_sys( "this = %p", this );
	locked_dec( g_proactor_counter.iTCPSentCtxs );

//	delete_buf_list(m_pBuf);
	m_pBuf->release();
}

void TCPSendEvent::OnHandle(DWORD dwTransferredBytes, DWORD dwError)
{
	locked_exchange_add( g_proactor_counter.iTCPSentBytes, (long)dwTransferredBytes );
	locked_inc( g_proactor_counter.iTCPSents );
/*
	delete_buf_list(m_pBuf);

	WSABUF wsaBuf[30];
	int i = 0;

	TCPSendEvent* pCtx = this;
	Buffer* pBuf = m_spLink->m_PktQ.pop();
	Buffer* pHead = pBuf;

	while ( pBuf )
	{
		wsaBuf[i].buf = pBuf->rd_ptr();
		wsaBuf[i].len = pBuf->size();
		// wsaBuf 가 full 이면 전송.
		if ( i == (int)sizeof(wsaBuf)/sizeof(WSABUF) -1)
		{
			pCtx = new TCPSendEvent( m_spLink );
			pCtx->m_pBuf = pHead;
			// 전송완료되면 Buffer 삭제되므로 linked list 의 연결을 끊는다.
			Buffer* pTail = pBuf;
			pBuf = pBuf->m_next;
			pTail->m_next = nullptr;

			if ( Socket::AsyncSend(m_spLink->m_hSocket, wsaBuf, i, pCtx->m_OV) == false )
			{
				delete_buf_list(pBuf);
				delete pCtx;
				goto Label_Send_Error;
			}

			i = 0;
			pCtx = this;
			pHead = pBuf;
			continue;
		}

		i++;
		pBuf = pBuf->m_next;
	}

	if ( i > 0 )
	{
		prn_sys( "pop (%d) in sending que", i );
		pCtx->m_pBuf = pHead;
		if ( Socket::AsyncSend(m_spLink->m_hSocket, wsaBuf, i, pCtx->m_OV) == false )
		{
			goto Label_Send_Error;
		}

		return;
	}
*/
// Label_Send_Error:
//	m_spLink->m_PktQ.off_sending();
	delete this;
}

//<
TCPUnlinkEvent::TCPUnlinkEvent( TCPLinkPtr spLink )
{ 
	m_spLink = spLink;
	locked_inc( g_proactor_counter.iTCPUnlinkCtxs );
}

TCPUnlinkEvent::~TCPUnlinkEvent()
{
	prn_sys("this = %p", this);
	locked_dec( g_proactor_counter.iTCPUnlinkCtxs );
}

void TCPUnlinkEvent::OnHandle(DWORD dwTransferredBytes, DWORD dwError)
{
	prn_sys("remote netlink reuse");
	DWORD dwBytes = 0;
	TCPAcceptEvent *pCtx = new TCPAcceptEvent( m_spLink );
	isfalse_goto( pCtx, error_accept );

	if ( FALSE == Socket::AcceptEx(
		m_spLink->m_spListen->m_hSocket,// listen socket
		m_spLink->m_hSocket,			// accept socket
		pCtx->m_AddrBuf,				// buffer for accept( server, remote address )
		0,								// default 0
		sizeof(sockaddr_in)+16,			// local address length
		sizeof(sockaddr_in)+16,			// remote address length
		&dwBytes,						// received bytes
		&pCtx->m_OV) )					// overlapped
	{
		istrue_goto( ERROR_IO_PENDING != WSAGetLastError(), error_accept );
	}

	locked_inc( m_spLink->m_spListen->m_iCreatedSockets );
	delete this;
	return;

error_accept:
	safe_delete(pCtx);
}

//<
TCPConnectEvent::TCPConnectEvent( TCPLinkPtr spLink )
{
	m_spLink = spLink;
	m_pNetCallback = spLink->m_pNetCallback;
	locked_inc( g_proactor_counter.iTCPConnectCtxs );
}

TCPConnectEvent::~TCPConnectEvent()
{
	prn_sys("this = %p", this);
	locked_dec( g_proactor_counter.iTCPConnectCtxs );
}

void TCPConnectEvent::OnHandle(DWORD dwTransferredBytes, DWORD dwError)
{
	istrue_goto(
		NOERROR != dwError,
		error_connect );
	isfalse_goto(
		IOCP::UpdateConnectContext(m_spLink->m_hSocket),
		error_connect );

	sockaddr_in		m_RemoteAddr;		//< 원격 주소값
	int iRemoteLen = sizeof(m_RemoteAddr);
	istrue_goto(
		SOCKET_ERROR == getpeername(m_spLink->m_hSocket,
		(sockaddr*)&m_RemoteAddr, &iRemoteLen),
		error_connect );
	//	prn_sys( "connect remote addr (%s)", cstr(Socket::SockAddr2String(&m_spLink->m_RemoteAddr)) );

	Network_IF& rNIF = m_spLink->NetIF();
	rNIF.m_iAddr = m_RemoteAddr.sin_addr.S_un.S_addr;
	rNIF.m_iPort = m_RemoteAddr.sin_port;

	m_pNetCallback->OnNetConnected( m_spLink );

	isfalse_goto( m_spLink->Recv(), error_recieve );
	// change link state
	m_spLink->SetCRunning();
	delete this;
	return;

error_connect :
	//prn_sys( "connect server failed, close client\n" );
	SetLastError( PROACTOR_CONNECTING_REMOTE_HOST_FAILED );
	m_pNetCallback->OnNetClosed( m_spLink );
	delete this;
	return;

error_recieve :
	//prn_err( "connect success, but receive failed" );
	SetLastError( PROACTOR_CONNECT_SUCCESS_BUT_RECEIVE_FAILED );
	m_pNetCallback->OnNetClosed( m_spLink );
	delete this;
}