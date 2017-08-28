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
#include <DataTypes.h>
#include <Mmsystem.h>
#include <Logger.h>
#include "Pc.h"
#include "NetLinkManager.h"



NetLinkManager::NetLinkManager( const Network_IF* pNIF, P2PAgentHandler* pHandler, PktTransmit* pTransmit, unsigned long iKeepConnection )
: m_Handler(pHandler)
, m_pTransmit(pTransmit)
, m_Relay(nullptr)
, m_iKeepConnection(iKeepConnection)
{
	m_Self = *pNIF;
}

NetLinkManager::~NetLinkManager()
{
	Clear();
	if ( m_pTransmit )
		delete m_pTransmit;
}

NetLink* NetLinkManager::Find( const Network_IF& nif )
{
	NetLinkMap::iterator it = m_NetLinkMap.find( nif.m_iNID );
	if ( it != m_NetLinkMap.end() )
	{
		return it->second;
	}

	return nullptr;
}

NetLink* NetLinkManager::Connect( const Network_IF& nif )
{
	Pc* pLink = (Pc*)OnCreate();
	pLink->NetIF( nif );
	pLink->NetST( eLINKST_SYNC );
	prn_sys( "create link nid(%I64u)", pLink->m_iNID );
	m_NetLinkMap.insert( NetLinkMap::value_type(pLink->m_iNID, (NetLink*)pLink) );
	return (NetLink*)pLink;
}

bool NetLinkManager::Close( NetLink* pLink )
{
	if ( !pLink )
		return false;

	NetLinkMap::iterator it;
	it = m_NetLinkMap.find( pLink->NetIF().m_iNID );
	if ( it != m_NetLinkMap.end() )
	{
		m_NetLinkMap.erase( it );
		return true;
	}

	return false;
}

void NetLinkManager::Clear()
{
	for ( NetLinkMap::iterator it = m_NetLinkMap.begin(); it != m_NetLinkMap.end(); )
	{
		it = m_NetLinkMap.erase( it );
	}

	while ( !m_Received.empty() )
	{
		Buffer* p = m_Received.front(); m_Received.pop_front();
//		delete p;
		p->release();
	}
}

void NetLinkManager::ReceivePkt( unsigned long iWait )
{
	unsigned long iCurrTick = timeGetTime() + MAX_PACKET_RECEIVE_TM;
	while ( m_pTransmit->IsReceived(iWait) && iCurrTick > timeGetTime() )
	{
		Buffer* pBuf = new Buffer( m_pTransmit->MaxBufferSize() );
		P2PNET_PACKET_BASE* p = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();
		int iLen = m_pTransmit->RecvFrom( (char*)&p->iLen, &p->Addr );
		if (  iLen <= 0 )
		{
			pBuf->release();
			break;
		}

		pBuf->wr_ptr(iLen + sizeof(SOCKADDR_IN));
		m_Received.push_back( pBuf );
	}
}

void NetLinkManager::Process( unsigned long iWait )
{
	Pc* pLink = nullptr;
	NetLinkMap::iterator it;

	// 수신 패킷 처리
	// 수신패킷 처리
	while ( !m_Received.empty() )
	{
		pLink = nullptr;
		Buffer* pBuf = m_Received.front(); m_Received.pop_front();
		P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();
		it = m_NetLinkMap.find( pPkt->iFromID );
		if ( it != m_NetLinkMap.end() )
		{
			pLink = (Pc*)it->second.get();
			pLink->OnReceived(pBuf);
		}
		else if ( pPkt->iPackID == eP2P_SYNC )
		{
			// 등록되지 않은 사용자는 Pc 를 생성한다.
			Network_IF nif;
			nif.m_iNID	= pPkt->iFromID;
			nif.m_iAddr	= pPkt->Addr.sin_addr.S_un.S_addr;
			nif.m_iPort	= pPkt->Addr.sin_port;
			pLink		= (Pc*)Connect( nif );
			pLink->OnSync( pBuf );
		}
		else if ( pPkt->iPackID == eP2P_JOIN )
		{
			P2P_JOIN_ACK pk(pPkt->iPktSeq, pPkt->iCellID, Self().m_iNID, pPkt->iTransTick);
			m_pTransmit->Send((const char*)&pk.iLen, pk.iLen, &pPkt->Addr);
		}
		else if ( pPkt->iPackID == eP2P_LEAVE )
		{
			P2P_LEAVE_ACK pk(pPkt->iPktSeq, pPkt->iCellID, Self().m_iNID, pPkt->iTransTick);
			m_pTransmit->Send((const char*)&pk.iLen, pk.iLen, &pPkt->Addr);
		}
		else
		{
			prn_sys( "pkt id(%d), from id(%I64u)", pPkt->iPackID, pPkt->iFromID );
		}

//		delete pBuf;
		pBuf->release();
	}

	// 패킷 전송 및 연결유지
	for ( it = m_NetLinkMap.begin(); it != m_NetLinkMap.end(); )
	{
		pLink = (Pc*)it->second.get();
		switch ( pLink->m_iSt )
		{
		case eLINKST_SYNC :
			pLink->Sync();
			break;
		case eLINKST_LINK :
			pLink->KeepConnection();
			if ( !pLink->CheckAlive() )
			{
				it = m_NetLinkMap.erase( it );
				continue;
			}
			break;
		}

		pLink->Process();
		++it;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

P2PAgentHandler* NetLinkManager::Handler( void )
{
	return m_Handler;
}

P2PAgentHandler* NetLinkManager::Handler( P2PAgentHandler* pHandler )
{
	m_Handler = pHandler;
	return m_Handler;
}

NetLink* NetLinkManager::Relay( const Network_IF& rNIF )
{
	m_Relay = Connect( rNIF );
	prn_sys( "create relay(%I64u), public(%s)", m_Relay->NetIF().m_iNID, NetLink::tostr(&m_Relay->NetIF()).c_str() );
	return m_Relay;
}

NetLink* NetLinkManager::Relay( void )
{
	return m_Relay;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool NetLinkManager::OnConnected( NetLinkPtr spLink )
{
	if ( !m_Handler )
		return false;

	return m_Handler->OnConnected( spLink );
}

bool NetLinkManager::OnClosed( NetLinkPtr spLink )
{
	if ( !m_Handler )
		return false;

	return m_Handler->OnClosed( spLink );
}

bool NetLinkManager::OnReceived( NetLinkPtr spLink, Buffer* pBuf )
{
	if ( !m_Handler )
		return false;

	return m_Handler->OnReceived( spLink, pBuf );
}