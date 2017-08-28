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
#include "NetPcManager.h"
#include "NetPc.h"
#include <Logger.h>
#include <P2PAgent.h>

//< 위로 올리지 말것
#include <Mmsystem.h>

NetPc::NetPc()
{
}

NetPc::NetPc(NetPcManager* pNetPcMgr, PktTransmit* pTransmit)
: Pc(pNetPcMgr, pTransmit)
{
}

NetPc::~NetPc()
{
}

int NetPc::OnJoin( Buffer* pBuf )
{
	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();

	prn_sys( "recv join (%I64u), seq (%lu)", pPkt->iFromID, pPkt->iPktSeq  );
	P2P_JOIN_ACK pk(pPkt->iPktSeq, pPkt->iCellID, m_NetLinkManager->Self().m_iNID, pPkt->iTransTick);
	pk.iTransTick = pPkt->iTransTick;
	m_pTransmit->Send((const char*)&pk.iLen, pk.iLen, &pPkt->Addr);
	Network_IF& nif = ((P2P_JOIN*)pPkt)->NIF;

	if ( m_iWishControlPktSeq == pPkt->iPktSeq )
	{
		m_iWishControlPktSeq++;
		if ( m_NetLinkManager->Self().m_iNID == nif.m_iNID )
		{
			prn_sys( "join self(%I64u), public(%s)", nif.m_iNID, NetLink::tostr(&nif).c_str() );
		}
		else
		{
			prn_sys( "join peer(%I64u) public(%s)", nif.m_iNID, NetLink::tostr(&nif).c_str() );
			m_NetLinkManager->Connect( nif );
		}
	}

	return 1;
}

int NetPc::OnLeave( Buffer* pBuf )
{
	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();

	prn_sys( "recv join from(%I64u)", pPkt->iFromID  );
	P2P_LEAVE_ACK pk(pPkt->iPktSeq, pPkt->iCellID,  m_NetLinkManager->Self().m_iNID, pPkt->iTransTick);
	pk.iTransTick = pPkt->iTransTick;
	m_pTransmit->Send((const char*)&pk.iLen, pk.iLen, &pPkt->Addr);
	Network_IF& nif = ((P2P_JOIN*)pPkt)->NIF;

	if ( m_iWishControlPktSeq == pPkt->iPktSeq )
	{
		m_iWishControlPktSeq++;
		if ( m_NetLinkManager->Self().m_iNID == nif.m_iNID )
		{
			prn_sys("leave self(%I64u), public(%s)", nif.m_iNID, NetLink::tostr(&nif).c_str());
		}
		else
		{
			prn_sys("leave peer(%I64u), public(%s)", nif.m_iNID, NetLink::tostr(&nif).c_str());
			NetLink* pLink = m_NetLinkManager->Find( nif );
			m_NetLinkManager->Close( pLink );
		}
	}

	return 1;
}

int NetPc::OnReliableAck( Buffer* pBuf )
{
	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();

	Latency( timeGetTime() - pPkt->iTransTick );
	prn_sys("recv reliable ack(%lu)", pPkt->iPktSeq);
	for ( P2PNET_PACKET_LIST::iterator it = m_Reliable.begin(); it != m_Reliable.end(); it++ )
	{
		Buffer* pTmpBuf = *it;
		P2PNET_PACKET_BASE* p = (P2PNET_PACKET_BASE*)pTmpBuf->rd_ptr();
		if ( p->iPktSeq == pPkt->iPktSeq )
		{
			prn_sys("erase reliable ack(%lu)", pPkt->iPktSeq);
			m_Reliable.erase( it );
			pTmpBuf->release();
			break;
		}
	}

	return 1;
}

int NetPc::OnReliableSendTo( Buffer* pBuf )
{
	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();

	prn_sys("recv reliable pkt(%lu), wish(%lu)", pPkt->iPktSeq, m_iWishReliablePktSeq);
//	prn_sys( "from (%I64u) to (%I64u)", iNID, m_NetLinkManager->Self().iNID );
	P2P_RELIABLE_ACK pk(pPkt->iPktSeq, pPkt->iCellID, m_NetLinkManager->Self().m_iNID, m_iNID, pPkt->iTransTick);
	m_pTransmit->Send((const char*)&pk.iLen, pk.iLen, &pPkt->Addr);

	// 수신 받은 패킷 순서가 올바른지 검증
	if ( m_iWishReliablePktSeq == pPkt->iPktSeq )
	{
		prn_sys("find reliable pkt(%lu)", pPkt->iPktSeq);
		m_iWishReliablePktSeq++;
		pBuf->rd_ptr(sizeof(P2PNET_PACKET_BASE));
		m_NetLinkManager->OnReceived( this, pBuf );

		// 혹시 순서가 바뀐 패킷이 있는지 확인
		for ( P2PNET_PACKET_LIST::iterator it = m_WaitReliable.begin(); it != m_WaitReliable.end(); )
		{
			Buffer* pTmpBuf = *it;
			P2PNET_PACKET_BASE* p = (P2PNET_PACKET_BASE*)pTmpBuf->rd_ptr();
			if ( m_iWishReliablePktSeq == p->iPktSeq )
			{
				prn_sys("find reliable pkt(%lu)", pPkt->iPktSeq);
				m_iWishReliablePktSeq++;
				pTmpBuf->rd_ptr(sizeof(P2PNET_PACKET_BASE));
				m_NetLinkManager->OnReceived( this, pTmpBuf );
				it = m_WaitReliable.erase(it);
				pTmpBuf->release();
			}
			else if ( m_iWishReliablePktSeq > p->iPktSeq )
			{
				it = m_WaitReliable.erase(it);
				pTmpBuf->release();
			}
			else
			{
				it++;
			}
		}
	}
	else if ( m_iWishReliablePktSeq < pPkt->iPktSeq )
	{
		prn_sys("wait reliable pkt(%lu)", pPkt->iPktSeq);
		// pBuf 가 NetLinkManager::Process() 에서 삭제되지 않게 레퍼런스카운트를 올린다.
		pBuf->incref();
		m_WaitReliable.push_back( pBuf );
		return 0;
	}

	return 1;
}


int NetPc::OnReceived( Buffer* pBuf )
{
	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();
	m_iReceivedPktTm = timeGetTime();

	switch (pPkt->iPackID)
	{
	case eP2P_SYNC :
		return OnSync( pBuf );
	case eP2P_SYNC_ACK :
		return OnSyncAck( pBuf );
	case eP2P_KEEPING_CONNECTION :
		return OnKeepConnection( pBuf );
	case eP2P_JOIN :
		return OnJoin( pBuf );
	case eP2P_LEAVE :
		return OnLeave( pBuf );
	case eP2P_RELIABLE_ACK :
		return OnReliableAck( pBuf );
	case eP2P_RELIABLE_SEND_TO :
		return OnReliableSendTo( pBuf );
	default :
		pBuf->rd_ptr(sizeof(P2PNET_PACKET_BASE));
		return m_NetLinkManager->OnReceived( this, pBuf );
	}
}