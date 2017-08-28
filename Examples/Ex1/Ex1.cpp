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
#include <P2P.h>

#include <Mmsystem.h>

/**
	원격 pc 와 홀펀칭을 수행하며 홀펀칭 완료후 1초 간격으로 reliable 패킷을 전송하는 예제
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

#include <Buffer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc == 1 )
	{
		printf( "원격 pc 와 홀펀칭을 수행하며 홀펀칭 완료후 1초 간격으로 reliable 패킷을\n전송하는 예제\n" );
		printf( "usage) send p2p packet\n" );
		printf( "ex1 server or client\n" );
		return 0;
	}

	NetHandler	Hdr;
	P2PAgent* pAgent = nullptr;
	if ( strcmp(argv[1], "server") == 0 )
	{
		// 고유ID: 1, IP,Port: 127.0.0.1,25533로 P2PAgent 를 생성한다.
		Network_IF nif(1, "127.0.0.1", 25533);
		pAgent = P2PAgentFactory::Create( nif, &Hdr );
		if ( !pAgent ) return 0;

		printf( "고유ID: %u, 네트웍정보: %s\n", pAgent->Self().m_iNID, NetLink::tostr(&pAgent->Self()).c_str() );

		// 패킷 처리및 연결유지를 수행한다.
		while (1)
		{
			pAgent->Process( 100 );
		}
	}
	else
	{
		// 고유ID: 2, IP,Port: 127.0.0.1,25534로 P2PAgent 를 생성한다.
		Network_IF nif(2, "127.0.0.1", 25534);
		pAgent = P2PAgentFactory::Create( nif, &Hdr );
		if ( !pAgent ) return 0;

		printf( "고유ID: %u, 네트웍정보: %s\n", pAgent->Self().m_iNID, NetLink::tostr(&pAgent->Self()).c_str() );

		// 고유ID: 1, IP,Port: 127.0.0.1,25534로 P2PAgent로 홀펀칭을 시도한다.
		Network_IF peer_nif(1, "127.0.0.1", 25533);
		NetLink* pLink = pAgent->Connect( peer_nif );

		unsigned long iSendTick = 0;
		while (1)
		{
			// 홀펀칭이 완료되면 1초에 한번씩 원격 pc 로 패킷을 전송한다.
			if ( pLink && pLink->NetST() == eLINKST_LINK && iSendTick <= timeGetTime() )
			{
				char szBuf[1024];
				int iLen = sprintf_s(szBuf, 1024, "this is test" );
				pLink->Send( szBuf, iLen+1, true );
				iSendTick = timeGetTime() + 1000;

			}

			// 패킷 처리및 연결유지를 수행한다.
			pAgent->Process( 100 );
		}
	}

	// P2PAgent를 삭제한다.
	delete pAgent;

	return 0;
}