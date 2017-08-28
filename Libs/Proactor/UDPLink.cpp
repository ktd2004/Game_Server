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
#include "UDPLink.h"
#include "NetUtil.h"
#include "UDPEvent.h"
#include "ProactorImpl.h"

namespace
{
	Proactor_Counter& g_proactor_counter = PROACTOR_COUNTER;
}

UDPLink::UDPLink( ProactorImpl* pProactor )
: m_pProactor(pProactor)
{
	NetST(eLINKST_NONE);
	m_iNID = locked_inc(pProactor->m_iUniqueID);
	m_hSocket = Socket::New( Socket::eUDP );
	locked_inc( g_proactor_counter.iUDPNetLinks );
}

UDPLink::~UDPLink()
{
	Socket::Delete( m_hSocket, Socket::eUDP );
	prn_sys("%s, %s", DEBUG_LINK_TYPE(LinkType()), DEBUG_LINK_STATE(NetST()));
	locked_inc( g_proactor_counter.iUDPNetLinks );
}

bool UDPLink::operator == (UDPLink &ref)
{
	istrue_ret(
		ref.m_hSocket == m_hSocket && ref.m_iNID == m_iNID,
		true );
	return false;
}

bool UDPLink::operator != (UDPLink &ref)
{
	isfalse_ret( ref == *this, true );
	return false;
}

bool UDPLink::Send( const char* pPkt, unsigned int iLen, SOCKADDR_IN* to )
{
	Buffer* pBuf = new Buffer(pPkt, iLen);
	return Send(pBuf, to);
}


bool UDPLink::Send( Buffer* pBuf, SOCKADDR_IN* to )
{
	UDPSendEvent *pCtx = new UDPSendEvent( this );
	isfalse_ret( pCtx, false );

	pCtx->m_pBuf = pBuf;
	WSABUF wsaBuf;
	wsaBuf.buf = pBuf->rd_ptr();
	wsaBuf.len = pBuf->size();

	isfalse_do_ret(
		Socket::AsyncSendTo(m_hSocket, wsaBuf, pCtx->m_OV, (SOCKADDR*)to),
		delete pCtx,
		false );

	return true;
}

void UDPLink::Kick( unsigned short iKickReason, const char* pFunc )
{
	prn_sys("%s@kick %s, %s",
		nvl_s(pFunc), DEBUG_LINK_TYPE(m_iLinkType), DEBUG_KICK_REASON(iKickReason) );
	locked_exchange( m_iSt, eLINKST_FIN );
	Socket::Delete( m_hSocket, Socket::eUDP );
}


bool UDPLink::Bind( const Proactor::Property& rProp )
{
	LinkType( eLINKTYPE_UDP );
	NetST( eLINKST_BIND );

	istrue_goto(
		false == IOCP::Associate(m_pProactor->GetHandle(), m_hSocket, (ULONG_PTR)ProactorImpl::eProactor_LNK),
		error_bind );

	istrue_goto( false == Socket::Bind(m_hSocket, rProp.sHost, rProp.iPort),
		error_bind );

	istrue_goto( false == this->Recv(),
		error_bind );

	return true;

error_bind:
	return false;
}


bool UDPLink::Recv( void )
{
	UDPRecvEvent* pCtx = new UDPRecvEvent( this );
	isfalse_goto(
		pCtx,
		error_recieve );

	WSABUF wsaBuf;
	wsaBuf.buf = pCtx->m_pBuf->wr_ptr();
	wsaBuf.len = pCtx->m_pBuf->space();

	isfalse_goto(
		Socket::ReceiveFrom(m_hSocket, wsaBuf, pCtx->m_OV, (SOCKADDR*)&pCtx->m_RemoteAddr),
		error_recieve );

	return true;

error_recieve:
	safe_delete(pCtx);
	return false;
}

bool UDPLink::Recv( UDPRecvEvent* pCtx )
{
	ZeroMemory( &pCtx->m_OV, sizeof(pCtx->m_OV) );
	WSABUF wsaBuf;
	wsaBuf.buf = pCtx->m_pBuf->wr_ptr();
	wsaBuf.len = pCtx->m_pBuf->space();

	isfalse_ret(
		Socket::ReceiveFrom(m_hSocket, wsaBuf, pCtx->m_OV, (SOCKADDR*)&pCtx->m_RemoteAddr),
		false );

	return true;
}

void UDPLink::SetBind( void )
{
	locked_exchange( m_iSt, eLINKST_BIND );
}

void UDPLink::SetFin( void )
{
	locked_exchange( m_iSt, eLINKST_FIN );
	Socket::Delete( m_hSocket, Socket::eUDP );
}
