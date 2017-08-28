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
#include "RelayPcManager.h"
#include "RelayPc.h"
//<
#include <P2PAgent.h>

//< 위로 올리지 말것
#include <Mmsystem.h>
#include <Logger.h>

RelayPc::RelayPc()
{
}

RelayPc::RelayPc(NetLinkManager* pRelayPcMgr, PktTransmit* pTransmit)
: Pc(pRelayPcMgr, pTransmit)
{
}


RelayPc::~RelayPc()
{
}

void RelayPc::Process( void )
{
	Pc::Process();
	// control packet
	P2PNET_PACKET_LIST::iterator it;
	for ( it = m_Control.begin(); it != m_Control.end(); it++ )
	{
		Buffer* pBuf = *it;
		P2PNET_PACKET_BASE* p = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();
		unsigned long iTick = timeGetTime();
		if ( p->iTransTick <= iTick )
		{
			prn_sys("send control seq (%lu) (%s)", p->iPktSeq, NetLink::tostr(&NetIF()).c_str());
			p->iTransTick = iTick;
			if ( m_pTransmit->Send((const char*)&p->iLen, p->iLen, &p->Addr) == -1 )
			{
				break;
			}
			p->iTransTick = iTick + m_iAvgLatency;
		}
	}
}

void RelayPc::Clear( void )
{
	Pc::Clear();
	while ( !m_Control.empty() )
	{
		Buffer* p = m_Control.front(); m_Control.pop_front();
//		delete p;
		p->release();
	}
}

void RelayPc::PushControl( Buffer* pBuf )
{
	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();

	NetLink::toaddr( pPkt->Addr, NetIF() );
	pPkt->iPktSeq = m_iControlPktSeq++;
	//prn_sys( "push control msg (%lu)", pPkt->iPktSeq );
	m_Control.push_back( pBuf );
}

int RelayPc::OnControlAck( Buffer* pBuf )
{
	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();
	prn_sys("recv control ack (%lu) from (%s)", pPkt->iPktSeq, NetLink::tostr(&pPkt->Addr).c_str());
	for ( P2PNET_PACKET_LIST::iterator it = m_Control.begin(); it != m_Control.end(); it++ )
	{
		Buffer* pTmpBuf = *it;
		P2PNET_PACKET_BASE* p = (P2PNET_PACKET_BASE*)pTmpBuf->rd_ptr();
		if ( p->iPktSeq == pPkt->iPktSeq )
		{
			prn_sys("erase control ack (%lu)", pPkt->iPktSeq);
			m_Control.erase( it );
			pTmpBuf->release();
			break;
		}
	}

	return 1;
}

int RelayPc::OnReceived( Buffer* pBuf )
{
	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();

	switch (pPkt->iPackID)
	{
	case eP2P_SYNC :
		return OnSync( pBuf );
	case eP2P_SYNC_ACK :
		return OnSyncAck( pBuf );
	case eP2P_KEEPING_CONNECTION :
		return OnKeepConnection( pBuf );
	case eP2P_JOIN_ACK :
	case eP2P_LEAVE_ACK :
		return OnControlAck( pBuf );
	default :
		return m_NetLinkManager->OnReceived( this, pBuf );
	}
}