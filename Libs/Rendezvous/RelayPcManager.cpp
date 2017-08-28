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
#include "RelayPc.h"
#include "RelayPcManager.h"

RelayPcManager::RelayPcManager( const Network_IF* pNIF, PktTransmit* pTransmit, unsigned long iKeepConnection )
: NetLinkManager( pNIF, nullptr, pTransmit, iKeepConnection )
{
}

RelayPcManager::~RelayPcManager()
{
}

NetLink* RelayPcManager::OnCreate()
{
	return (NetLink*)new RelayPc( this, m_pTransmit );
}

void RelayPcManager::Process( unsigned long iWait )
{
	ReceivePkt( iWait );
	NetLinkManager::Process( iWait );
}

bool RelayPcManager::OnConnected( NetLinkPtr spLink )
{
	P2P_JOIN* p = nullptr;
	for ( NetLinkMap::iterator it = m_NetLinkMap.begin(); it != m_NetLinkMap.end(); ++it )
	{
		RelayPc* pc = (RelayPc*)it->second.get();
		if ( spLink != (NetLink*)pc )
		{	// �̹� ������ ��� �������� �˸���.
			prn_sys("send join (%I64u) to others (%s)", spLink->NetIF().m_iNID, NetLink::tostr(&pc->NetIF()).c_str());
			Buffer* pBuf = new Buffer( sizeof(P2P_JOIN) );
			p = (P2P_JOIN*)pBuf->reserve(sizeof(P2P_JOIN));
			memset( p, 0x00, sizeof(P2P_JOIN) );
			p->iLen		= PKT_LEN(P2P_JOIN);
			p->iPackID	= eP2P_JOIN;
			p->iCellID	= 0;
			p->iFromID	= m_Self.m_iNID;
			memcpy( &p->NIF,  &spLink->NetIF(), sizeof(Network_IF) );

			pc->PushControl( pBuf );
		}

		// ���� ������ ��� ���� ������ �˸���.
		prn_sys("send members (%I64u) to self (%s)", pc->NetIF().m_iNID, NetLink::tostr(&pc->NetIF()).c_str());
		Buffer* pBuf = new Buffer( sizeof(P2P_JOIN) );
		p = (P2P_JOIN*)pBuf->reserve(sizeof(P2P_JOIN));
		memset( p, 0x00, sizeof(P2P_JOIN) );
		p->iLen		= PKT_LEN(P2P_JOIN);
		p->iPackID	= eP2P_JOIN;
		p->iCellID	= 0;
		p->iFromID	= m_Self.m_iNID;
		memcpy( &p->NIF,  &pc->NetIF(), sizeof(Network_IF) );

		((RelayPc*)spLink.get())->PushControl( pBuf );
	}

	return true;
}

bool RelayPcManager::OnClosed( NetLinkPtr spLink )
{
	// relay ������ NetPc���� ������ ���¸� �˼� ���� ������ �����伭������ ���������� ����
	P2P_LEAVE* p = nullptr;
	for ( NetLinkMap::iterator it = m_NetLinkMap.begin(); it != m_NetLinkMap.end(); ++it )
	{
		RelayPc* pc = (RelayPc*)it->second.get();
		if ( spLink != (NetLink*)pc )
		{	// �̹� ������ ��� �������� �˸���.
			prn_sys("send leave (%I64u) to others (%s)", spLink->NetIF().m_iNID, NetLink::tostr(&pc->NetIF()).c_str());
			Buffer* pBuf = new Buffer( sizeof(P2P_LEAVE) );
			p = (P2P_LEAVE*)pBuf->reserve(sizeof(P2P_LEAVE));
			memset( p, 0x00, sizeof(P2P_LEAVE) );
			p->iLen		= PKT_LEN(P2P_LEAVE);
			p->iPackID	= eP2P_LEAVE;
			p->iCellID	= 0;
			p->iFromID	= m_Self.m_iNID;
			memcpy( &p->NIF, &spLink->NetIF(), sizeof(Network_IF) );
			pc->PushControl( pBuf );
		}
	}
	return true;
}

bool RelayPcManager::OnReceived( NetLinkPtr spLink, Buffer* pBuf )
{
	// ��Ŷ�� ������ �ϸ� �ȴ�.
	P2PNET_PACKET_BASE* p = (P2PNET_PACKET_BASE*)pBuf->rd_ptr();

	NetLinkMap::iterator it = m_NetLinkMap.find( p->iToID );
	if ( it != m_NetLinkMap.end() )
	{
		RelayPc* pc = (RelayPc*)it->second.get();
		// LYJ ���⵵ ���ľ� �ȴ�.
		// Pc::Process() ���� ��Ŷ�� �����Ҷ� ��Ŷ�� ���� tick�� �����Ѵ�.
		// �����̿����� �׳� �����ؾ� �ȴ�.
		NetLink::toaddr( p->Addr, pc->NetIF() );
		prn_sys("received relay from (%I64u) to (%I64u)(%s)", p->iFromID, p->iToID, NetLink::tostr(&pc->NetIF()).c_str());
		m_pTransmit->Send((const char*)&p->iLen, p->iLen, &p->Addr);
	}

	return true;
}