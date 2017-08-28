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

#include "RelayPcManagerEx.h"
#include <NetEventHandler.h>
#include <RendezvousExAgent.h>
#include <Sync.h>

namespace
{
	//< p2p 패킷을 송수신하는 Wrapper 클래스
	class PktSender : public PktTransmit
	{
	public:
		NetLinkPtr		m_spLink;
	public:
		PktSender(NetLinkPtr spLink) 
		{
			m_spLink = spLink;
		}
		virtual ~PktSender()
		{
		}

		void MaxBufferSize( unsigned int  iMaxBufferSize )
		{
			m_spLink->MaxBufSize(iMaxBufferSize);
		}

		unsigned int MaxBufferSize( void )
		{
			return (unsigned int)m_spLink->MaxBufSize();
		}

		int Send( const char* pszBuf, unsigned int iLen, SOCKADDR_IN* to )
		{
			return m_spLink->Send(pszBuf, iLen, to) == true ? 1 : 0;
		}

		bool IsReceived( unsigned long iWait )
		{
			return false;
		}

		int	RecvFrom( char* pszBuf, SOCKADDR_IN* from )
		{
			return 0;
		}
	};

	//< 네트웍 이벤트를 처리하는 클래스
	class P2pReceiver : public NetEventHandler
	{
		typedef std::list<RelayPcManagerExPtr>	RelayPcManagerExList;
	public:
		Sync						m_Sync;
		RelayPcManagerExList		m_List;

	public:
		P2pReceiver( unsigned int iTaskSchedulerID )
			: NetEventHandler( iTaskSchedulerID )
		{
			SetReceivedFunc( this, &P2pReceiver::OnReceived );
		}

		RelayPcManagerEx* Create( const Network_IF& nif, unsigned long iKeepConnection )
		{
			std::string sIP = NetLink::nif2ip(&nif);
			unsigned short iPort = NetLink::nif2port(&nif);
			NetLinkPtr spUDP = Bind( iPort, sIP );
			PktSender* pSender = new PktSender( spUDP );
			RelayPcManagerEx* pRelayPcManager = new RelayPcManagerEx( &nif, pSender, iKeepConnection );
			{
				Guard lock(m_Sync);
				m_List.push_back( pRelayPcManager );
				spUDP->UserData( pRelayPcManager );
			}
			return pRelayPcManager;
		}

		bool OnReceived(NetLinkPtr spLink, Buffer* pBuf )
		{
			RelayPcManagerExPtr spRelayPcManager = (RelayPcManagerEx*)spLink->UserData();
			if ( spRelayPcManager )
			{
				Buffer* pCopy = new Buffer(pBuf->length()+sizeof(SOCKADDR_IN));
				P2PNET_PACKET_BASE* pk = (P2PNET_PACKET_BASE*)pCopy->reserve(pBuf->length()+sizeof(SOCKADDR_IN));
				pk->Addr = *(SOCKADDR_IN*)spLink.param();
				memcpy( &pk->iLen, pBuf->rd_ptr(), pBuf->length() );
				spRelayPcManager->m_Received.push_back( pCopy );
				spRelayPcManager->Process( 0 );
			}

			return true;
		}

		void EventTick( void )
		{
			Guard lock(m_Sync);
			foreach_do( RelayPcManagerExList, it, m_List )
			{
				(*it)->Process(0);
			}
		}
	};

	static P2pReceiver* g_Receiver = nullptr;
	Sync g_Sync;
};

P2PAgent* RendezvousExAgentFactory::Create( const Network_IF& rSelf, unsigned int iTaskSchedulerID, unsigned short iBindCPU, unsigned int iMaxBufferSize, unsigned long iKeepConnection )
{
	Guard lock(g_Sync);
	if ( g_Receiver == nullptr )
	{
		TaskScheduler* pTsch = CREATE_TASKSCEDULER( iTaskSchedulerID, iKeepConnection, iBindCPU );
		if ( pTsch )
		{
			START_TASKSCHEDULER( 1 );
			g_Receiver = new P2pReceiver( 1 );
			g_Receiver->Start( 1, 1 );
			CREATE_TASK( 1, g_Receiver, &P2pReceiver::EventTick, 1000 );
		}
	}

	RelayPcManagerEx* pMgr = g_Receiver->Create( rSelf, iKeepConnection );

	return pMgr;
}

void RendezvousExAgentFactory::Stop( void )
{
	Guard lock(g_Sync);
	if ( g_Receiver != nullptr )
	{
		g_Receiver->Stop();
		STOP_TASKSCHEDULER(0);
	}
}
