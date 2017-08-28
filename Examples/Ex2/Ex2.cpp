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

#include <P2PEx.h>

/**
	랑데뷰/패킷 릴레이 서버를 생성하고 원격 pc 에서 랑데뷰/패킷 릴레이 서버에 조인
*/

// p2p 네트웍 이벤트 핸들러
struct NetHandler : public P2PAgentHandler
{
	// 원격 pc 와 홀편칭이 완료되면 호출된다.
	// spLink 는 홀펀칭이 완료된 원격 pc
	bool OnConnected( NetLinkPtr spLink ) override
	{
		printf( "원격 pc 와 홀펀칭이 완료됬다. \n" );
		return true;
	}

	// 원격 pc 와 연결이 종료되면 호출된다.
	// spLink는 연결이 종료된 원격 pc
	bool OnClosed( NetLinkPtr spLink ) override
	{
		printf( "원격 pc 와 연결이 종료됬다. \n" );
		return true;
	}

	// 원격 pc 에서 패킷을 수신했다.
	// spLink는 패킷을 전송한 원격 pc
	// pBuf 는 패킷을 담고 있는 버퍼
	bool OnReceived( NetLinkPtr spLink, Buffer* pBuf ) override
	{
		printf( "원격 pc에서 패킷 수신, size = %d \n", pBuf->length() );
		return true;
	}
};

// RPC.NET의 Proactor로 구현한 랑데뷰/패킷 릴레이 서버 생성
#define RPC_NET_PROACTOR

int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc == 1 )
	{
		printf( "랑데뷰/패킷 릴레이 서버를 생성하고 원격 pc 에서 랑데뷰/패킷 릴레이\n서버에 조인하는 예제\n" );
		printf( "usage)\n" );
		printf( "ex2 server or my_id my_port\n" );
		printf( "server : ex2 server\n" );
		printf( "client : ex2 101 25001\n" );
		return 0;
	}

	P2PAgent* pAgent = nullptr;
	NetHandler Hdr;

	if ( strcmp(argv[1], "server") == 0 )
	{
		// 고유ID: 100, IP,Port: 127.0.0.1,65522로 랑데뷰/패킷 릴레이 서버를 생성한다.
		Network_IF nif(100, "127.0.0.1", 65522);
#if !defined(RPC_NET_PROACTOR)
		pAgent = RendezvousAgentFactory::Create( nif );
#else
		pAgent = RendezvousExAgentFactory::Create( nif, 1, 1 );
#endif
		if ( !pAgent ) return 0;
		printf( "랑데뷰/패킷릴레이 고유ID: %u, 네트웍정보: %s\n", pAgent->Self().m_iNID, NetLink::tostr(&pAgent->Self()).c_str() );

#if !defined(RPC_NET_PROACTOR)
		// 패킷 처리및 연결유지를 수행한다.
		while ( 1 )
		{
			pAgent->Process( 100 );
		}
#else
		getchar();
		// 랑데뷰/패킷릴레이 서버를 종료 처리한다.
		RendezvousExAgentFactory::Stop();
#endif

	}
	else
	{
		// 자신의 네트웍 정보를 설정한다.
		Network_IF nif(atoi(argv[1]), "127.0.0.1", atoi(argv[2]));
		// 랑데뷰/패킷 릴레이 서버에 네트웍 정보를 설정한다.
		Network_IF rendezvous_nif(100, "127.0.0.1", 65522 );
		// 랑데뷰/패킷릴레이 서버에 접속한다.
		pAgent = RendezvousAgentFactory::Join( nif, &Hdr, rendezvous_nif );
		if ( !pAgent ) return 0;

		printf( "고유ID: %u, 네트웍정보: %s\n", pAgent->Self().m_iNID, NetLink::tostr(&pAgent->Self()).c_str() );

		unsigned long iSendTick = timeGetTime() + 1000 * 10;
		while (1)
		{
			// 10초에 한번씩 reliable 패킷을 릴레이를 서버를 제외한 모든 원격 pc 에 전송한다.
			NetLinkMap::iterator it;
			for ( it = pAgent->GetNetLinkMap().begin(); it != pAgent->GetNetLinkMap().end(); ++it )
			{
				if ( it->second->NetST() == eLINKST_LINK &&
					 pAgent->Relay() != it->second &&
					 iSendTick < timeGetTime() )
				{
					printf( "send %u\n", it->second->NetIF().m_iNID );
					char szBuf[1024];
					int iLen = sprintf_s(szBuf, 1024, "this is test");
					it->second->Send( szBuf, iLen+1, true );
 					iSendTick = timeGetTime() + 1000 * 10;
				}
			}

			// 패킷 처리및 연결유지를 수행한다.
			pAgent->Process(1);
		}

	}

	return 0;
}

