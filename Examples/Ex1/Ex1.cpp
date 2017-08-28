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
	���� pc �� Ȧ��Ī�� �����ϸ� Ȧ��Ī �Ϸ��� 1�� �������� reliable ��Ŷ�� �����ϴ� ����
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

#include <Buffer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc == 1 )
	{
		printf( "���� pc �� Ȧ��Ī�� �����ϸ� Ȧ��Ī �Ϸ��� 1�� �������� reliable ��Ŷ��\n�����ϴ� ����\n" );
		printf( "usage) send p2p packet\n" );
		printf( "ex1 server or client\n" );
		return 0;
	}

	NetHandler	Hdr;
	P2PAgent* pAgent = nullptr;
	if ( strcmp(argv[1], "server") == 0 )
	{
		// ����ID: 1, IP,Port: 127.0.0.1,25533�� P2PAgent �� �����Ѵ�.
		Network_IF nif(1, "127.0.0.1", 25533);
		pAgent = P2PAgentFactory::Create( nif, &Hdr );
		if ( !pAgent ) return 0;

		printf( "����ID: %u, ��Ʈ������: %s\n", pAgent->Self().m_iNID, NetLink::tostr(&pAgent->Self()).c_str() );

		// ��Ŷ ó���� ���������� �����Ѵ�.
		while (1)
		{
			pAgent->Process( 100 );
		}
	}
	else
	{
		// ����ID: 2, IP,Port: 127.0.0.1,25534�� P2PAgent �� �����Ѵ�.
		Network_IF nif(2, "127.0.0.1", 25534);
		pAgent = P2PAgentFactory::Create( nif, &Hdr );
		if ( !pAgent ) return 0;

		printf( "����ID: %u, ��Ʈ������: %s\n", pAgent->Self().m_iNID, NetLink::tostr(&pAgent->Self()).c_str() );

		// ����ID: 1, IP,Port: 127.0.0.1,25534�� P2PAgent�� Ȧ��Ī�� �õ��Ѵ�.
		Network_IF peer_nif(1, "127.0.0.1", 25533);
		NetLink* pLink = pAgent->Connect( peer_nif );

		unsigned long iSendTick = 0;
		while (1)
		{
			// Ȧ��Ī�� �Ϸ�Ǹ� 1�ʿ� �ѹ��� ���� pc �� ��Ŷ�� �����Ѵ�.
			if ( pLink && pLink->NetST() == eLINKST_LINK && iSendTick <= timeGetTime() )
			{
				char szBuf[1024];
				int iLen = sprintf_s(szBuf, 1024, "this is test" );
				pLink->Send( szBuf, iLen+1, true );
				iSendTick = timeGetTime() + 1000;

			}

			// ��Ŷ ó���� ���������� �����Ѵ�.
			pAgent->Process( 100 );
		}
	}

	// P2PAgent�� �����Ѵ�.
	delete pAgent;

	return 0;
}