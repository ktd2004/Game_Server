#include "stdafx.h"
//<
#include "NetLinkManager.h"
#include "Pc.h"

//< 위로 올리지 말것
#include <Mmsystem.h>
#include <Logger.h>

Pc::Pc()
{
	Init( nullptr, nullptr );
}

Pc::Pc(NetLinkManager* pNetLinkMgr, PktTransmit* pTransmit)
{
	Init( pNetLinkMgr, pTransmit );
}

Pc::~Pc()
{
	Clear();
}

void Pc::Init( NetLinkManager* pNetLinkMgr, PktTransmit* pTransmit )
{
	this->m_NetLinkManager	= pNetLinkMgr;
	this->m_pTransmit		= pTransmit;
	memset( m_Latency, 0x00, sizeof(m_Latency) );
	this->m_iSyncTm			= 0;
	this->m_iSyncCnt		= 0;
	this->m_iSelfAddr		= m_NetLinkManager ? m_NetLinkManager->m_Self.m_iAddr : 0;
	this->m_iSelfPort		= m_NetLinkManager ? m_NetLinkManager->m_Self.m_iPort : 0;

	//<
	m_iLinkType				= eLINKTYPE_P2P;
	m_iSt					= eLINKST_UNLINK;
	m_iUnreliablePktSeq		= 0;
	m_iReliablePktSeq		= 0;
	m_iWishReliablePktSeq	= 0;
	m_iControlPktSeq		= 0;
	m_iWishControlPktSeq	= 0;
	m_iKeepConnection		= 0;
	m_iReceivedPktTm		= 0;
	m_iAvgLatency			= MIN_PACKET_RESEND_LATENCY;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool Pc::Send( Buffer* pBuf, bool bReliable )
{
	P2PNET_PACKET_BASE* p = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();
	p->iLen = pBuf->length() - sizeof(SOCKADDR_IN);
	p->iTransTick = 0;

#if !defined(TEST_REALY)		// 릴레이 테스트 할때
	if ( m_iSt == eLINKST_LINK )
	{
		NetLink::toaddr( p->Addr, NetIF() );
	}
	else
#endif
	if ( m_NetLinkManager->Relay() && m_NetLinkManager->Relay()->NetST() == eLINKST_LINK )
	{
		prn_sys("send pkt to relay (%I64u)", m_NetLinkManager->Relay()->m_iNID);
		p->iToID = m_iNID;
		NetLink::toaddr( p->Addr, m_NetLinkManager->Relay()->NetIF() );
	}
	else
	{
		pBuf->release();
		return false;
	}

	p->iFromID = m_NetLinkManager->m_Self.m_iNID;
	if ( bReliable )
	{
		p->iPackID = eP2P_RELIABLE_SEND_TO;
		p->iPktSeq = m_iReliablePktSeq++;
		prn_sys("send from (%I64u) to (%I64u)", m_NetLinkManager->m_Self.m_iNID, m_iNID);
		m_Reliable.push_back( pBuf );
	}
	else
	{
		p->iPackID = eP2P_SEND_TO;
		p->iPktSeq = m_iUnreliablePktSeq++;
		m_Reserved.push_back( pBuf );
	}

	return true;
}

bool Pc::Send( const char* pPkt, unsigned int iLen, bool bReliable )
{
	Buffer* pBuf = new Buffer( iLen + sizeof(P2PNET_PACKET_BASE) );
	pBuf->reserve(sizeof(P2PNET_PACKET_BASE));
	memset( pBuf->rd_ptr(), 0x00, sizeof(P2PNET_PACKET_BASE));
	pBuf->copy( pPkt, iLen );
	return Send( pBuf, bReliable );
}

void Pc::Process( void )
{
	P2PNET_PACKET_LIST::iterator it;
	// unreliable packet
	for ( it = m_Reserved.begin(); it != m_Reserved.end(); )
	{
		Buffer* pBuf = *it;
		P2PNET_PACKET_BASE* p = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();
		unsigned long iTick = timeGetTime();
		if ( p->iTransTick <= iTick )
		{
			p->iTransTick = iTick;
			if ( m_pTransmit->Send((const char*)&p->iLen, p->iLen, &p->Addr) == -1 )
			{
				break;
			}
			it = m_Reserved.erase(it);

			pBuf->release();
		}
		else
		{
			it++;
		}
	}
	// reliable packet
	for ( it = m_Reliable.begin(); it != m_Reliable.end(); it++ )
	{
		Buffer* pBuf = *it;
		P2PNET_PACKET_BASE* p = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();
		unsigned long iTick = timeGetTime();
		if ( p->iTransTick <= iTick )
		{
			p->iTransTick = iTick;
			if ( m_pTransmit->Send((const char*)&p->iLen, p->iLen, &p->Addr) == -1 )
			{
				break;
			}
			p->iTransTick = iTick + m_iAvgLatency;
		}
	}
}

void Pc::Clear()
{
	while ( !m_Reserved.empty() )
	{
		Buffer* p = m_Reserved.front(); m_Reserved.pop_front();
		p->release();
	}
	while ( !m_Reliable.empty() )
	{
		Buffer* p = m_Reliable.front(); m_Reliable.pop_front();
		p->release();
	}
	while ( !m_WaitReliable.empty() )
	{
		Buffer* p = m_WaitReliable.front(); m_WaitReliable.pop_front();
		p->release();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Pc::Sync( void )
{
	unsigned long iCurrTick = timeGetTime();
	if ( m_iSyncCnt >= MAX_SYNC_CNT && m_iSt == eLINKST_SYNC )
	{
		m_iSt		= eLINKST_UNLINK;
		m_iSyncTm	= 0;
		m_iSyncCnt	= 0;
		return;
	}

	if ( m_iSyncTm >= iCurrTick )
		return;

	Buffer* pBuf = new Buffer( sizeof(P2P_SYNC) );
	P2P_SYNC* p = (P2P_SYNC*)pBuf->reserve(sizeof(P2P_SYNC));
	memset( p, 0x00, sizeof(P2P_SYNC) );
	p->Addr.sin_family				= AF_INET;
	p->Addr.sin_port				= m_iPort;
	p->Addr.sin_addr.S_un.S_addr	= m_iAddr;

	p->iLen		= PKT_LEN(P2P_SYNC);
	p->iPackID	= eP2P_SYNC;
	p->iCellID	= 0;
	p->iFromID	= m_NetLinkManager->m_Self.m_iNID;

	//prn_sys( "send sync to (%I64u) (%s)", m_iNID, NetLink::tostr(&this->NetIF()).c_str() );
	// unreliable pkt seq 를 올리지 않기 위해서 직접 넣는다.
	m_Reserved.push_back( pBuf );
	m_iSyncCnt++;
	// next send tm
	m_iSyncTm = iCurrTick + m_iAvgLatency;
}

bool Pc::CheckAlive( void )
{
	unsigned long iCurrTick = timeGetTime();
	if ( (long)(iCurrTick - m_iReceivedPktTm) > (long)(m_NetLinkManager->m_iKeepConnection + m_iAvgLatency + MAX_KEEP_ALIVE) )
	{
		prn_sys("check alive rcv tm (%lu), curr tm (%lu) > (%lu)", m_iReceivedPktTm, iCurrTick, (long)(iCurrTick - m_iReceivedPktTm));
		m_NetLinkManager->OnClosed( this );
		return false;
	}

	return true;
}

void Pc::KeepConnection( void )
{
	unsigned long iCurrTick = timeGetTime();
	if ( m_iKeepConnection >= iCurrTick )
	{
		return;
	}

	Buffer* pBuf = new Buffer( sizeof(P2P_KEEP_CONNECTION) );
	P2P_KEEP_CONNECTION* p = (P2P_KEEP_CONNECTION*)pBuf->reserve(sizeof(P2P_KEEP_CONNECTION));
	memset( p, 0x00, sizeof(P2P_KEEP_CONNECTION) );
	p->Addr.sin_family				= AF_INET;
	p->Addr.sin_port				= m_iPort;
	p->Addr.sin_addr.S_un.S_addr	= m_iAddr;

	p->iLen		= PKT_LEN(P2P_KEEP_CONNECTION);
	p->iPackID	= eP2P_KEEPING_CONNECTION;
	p->iCellID	= 0;
	p->iFromID	= m_NetLinkManager->m_Self.m_iNID;

	//prn_sys( "send keep to (%I64u) (%s)", iNID, NetLink::Addr2Str(&this->NetIF()).c_str() );
	// unreliable pkt seq 를 올리지 않기 위해서 직접 넣는다.
	m_Reserved.push_back( pBuf );
	m_iKeepConnection = iCurrTick + m_NetLinkManager->m_iKeepConnection;
}

void Pc::Latency( unsigned long iLatency )
{
	memmove( m_Latency, &m_Latency[1], sizeof(m_Latency)-sizeof(unsigned long) );
	m_Latency[MAX_LATENCY-1] = iLatency;

	unsigned long Latency[MAX_LATENCY];
	unsigned long iSum = 0;

	memcpy( Latency, m_Latency, sizeof(Latency) );
	qsort( Latency, MAX_LATENCY, sizeof(unsigned long), SortLatency );

	// 최하,최상 제외하고
	for (int i = 1; i < MAX_LATENCY - 1; i++)
	{
		iSum += Latency[i];
	}

	m_iAvgLatency = iSum / (MAX_LATENCY - 2);
	if ( m_iAvgLatency < MIN_PACKET_RESEND_LATENCY ) m_iAvgLatency = MIN_PACKET_RESEND_LATENCY;
//	prn_sys( "latency (%lu)", m_iAvgLatency );
}

unsigned long Pc::Latency( void )
{
	return m_iAvgLatency;
}

int Pc::SortLatency(const void *arg1, const void *arg2)
{
	if ( *(unsigned long *)arg1 < *(unsigned long *)arg2 )
		return -1;
	if ( *(unsigned long *)arg1 > *(unsigned long *)arg2 )
		return 1;
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////

int Pc::OnSync( Buffer* pBuf )
{
	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();

	prn_sys("recv sync from (%I64u) (%s)", pPkt->iFromID, NetLink::tostr(&NetIF()).c_str());
	P2P_SYNC_ACK pk(pPkt->iCellID, m_NetLinkManager->m_Self.m_iNID, pPkt->iTransTick, pPkt->Addr);
	m_iAddr = pPkt->Addr.sin_addr.S_un.S_addr;
	m_iPort = pPkt->Addr.sin_port;

#if !defined(TEST_SYNC_TIMEOUT)
	m_pTransmit->Send((const char*)&pk.iLen, pk.iLen, &pPkt->Addr);
#endif

	return 1;
}

int Pc::OnSyncAck( Buffer* pBuf )
{
	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();
	prn_sys("recv sync ack from (%I64u) (%s)", pPkt->iFromID, NetLink::tostr(&NetIF()).c_str());
	// OnConnected 가 반복 호출방지
	if ( m_iSt == eLINKST_SYNC )
	{
		m_iSt				= eLINKST_LINK;
		m_iSelfAddr			= ((P2P_SYNC_ACK*)pPkt)->iSelfAddr;
		m_iSelfPort			= ((P2P_SYNC_ACK*)pPkt)->iSelfPort;
		m_iReceivedPktTm	= timeGetTime() + m_NetLinkManager->m_iKeepConnection;
		m_NetLinkManager->OnConnected( this );
	}

	return 1;
}

int Pc::OnKeepConnection( Buffer* pBuf )
{
	m_iReceivedPktTm = timeGetTime();

	P2PNET_PACKET_BASE* pPkt = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();
	prn_sys("recv (%lu) keep connection from (%I64u)", m_iReceivedPktTm, pPkt->iFromID);
	m_iAddr	= pPkt->Addr.sin_addr.S_un.S_addr;
	m_iPort	= pPkt->Addr.sin_port;
	return 1;
}