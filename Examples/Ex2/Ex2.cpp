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
	������/��Ŷ ������ ������ �����ϰ� ���� pc ���� ������/��Ŷ ������ ������ ����
*/

// p2p ��Ʈ�� �̺�Ʈ �ڵ鷯
struct NetHandler : public P2PAgentHandler
{
	// ���� pc �� Ȧ��Ī�� �Ϸ�Ǹ� ȣ��ȴ�.
	// spLink �� Ȧ��Ī�� �Ϸ�� ���� pc
	bool OnConnected( NetLinkPtr spLink ) override
	{
		printf( "���� pc �� Ȧ��Ī�� �Ϸ���. \n" );
		return true;
	}

	// ���� pc �� ������ ����Ǹ� ȣ��ȴ�.
	// spLink�� ������ ����� ���� pc
	bool OnClosed( NetLinkPtr spLink ) override
	{
		printf( "���� pc �� ������ ������. \n" );
		return true;
	}

	// ���� pc ���� ��Ŷ�� �����ߴ�.
	// spLink�� ��Ŷ�� ������ ���� pc
	// pBuf �� ��Ŷ�� ��� �ִ� ����
	bool OnReceived( NetLinkPtr spLink, Buffer* pBuf ) override
	{
		printf( "���� pc���� ��Ŷ ����, size = %d \n", pBuf->length() );
		return true;
	}
};

// RPC.NET�� Proactor�� ������ ������/��Ŷ ������ ���� ����
#define RPC_NET_PROACTOR

int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc == 1 )
	{
		printf( "������/��Ŷ ������ ������ �����ϰ� ���� pc ���� ������/��Ŷ ������\n������ �����ϴ� ����\n" );
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
		// ����ID: 100, IP,Port: 127.0.0.1,65522�� ������/��Ŷ ������ ������ �����Ѵ�.
		Network_IF nif(100, "127.0.0.1", 65522);
#if !defined(RPC_NET_PROACTOR)
		pAgent = RendezvousAgentFactory::Create( nif );
#else
		pAgent = RendezvousExAgentFactory::Create( nif, 1, 1 );
#endif
		if ( !pAgent ) return 0;
		printf( "������/��Ŷ������ ����ID: %u, ��Ʈ������: %s\n", pAgent->Self().m_iNID, NetLink::tostr(&pAgent->Self()).c_str() );

#if !defined(RPC_NET_PROACTOR)
		// ��Ŷ ó���� ���������� �����Ѵ�.
		while ( 1 )
		{
			pAgent->Process( 100 );
		}
#else
		getchar();
		// ������/��Ŷ������ ������ ���� ó���Ѵ�.
		RendezvousExAgentFactory::Stop();
#endif

	}
	else
	{
		// �ڽ��� ��Ʈ�� ������ �����Ѵ�.
		Network_IF nif(atoi(argv[1]), "127.0.0.1", atoi(argv[2]));
		// ������/��Ŷ ������ ������ ��Ʈ�� ������ �����Ѵ�.
		Network_IF rendezvous_nif(100, "127.0.0.1", 65522 );
		// ������/��Ŷ������ ������ �����Ѵ�.
		pAgent = RendezvousAgentFactory::Join( nif, &Hdr, rendezvous_nif );
		if ( !pAgent ) return 0;

		printf( "����ID: %u, ��Ʈ������: %s\n", pAgent->Self().m_iNID, NetLink::tostr(&pAgent->Self()).c_str() );

		unsigned long iSendTick = timeGetTime() + 1000 * 10;
		while (1)
		{
			// 10�ʿ� �ѹ��� reliable ��Ŷ�� �����̸� ������ ������ ��� ���� pc �� �����Ѵ�.
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

			// ��Ŷ ó���� ���������� �����Ѵ�.
			pAgent->Process(1);
		}

	}

	return 0;
}

