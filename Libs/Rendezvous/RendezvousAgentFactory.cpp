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

#include "RelayPc.h"
#include "RelayPcManager.h"
#include <DiagramStream.h>
#include <NetPcManager.h>
#include <RendezvousAgent.h>
#include <Logger.h>

initializer_begin
{
	WSADATA WSAData;
	if( WSAStartup( MAKEWORD(2, 2), &WSAData) != 0 )
	{
		prn_err( "WSAStartup error(%d)", WSAGetLastError() );
	}
}
end_initializer;

P2PAgent* RendezvousAgentFactory::Create( const Network_IF& rSelf, unsigned int iMaxBufferSize, unsigned long iKeepConnection )
{
	DiagramStream* pTransmit = new DiagramStream;
	std::string sIP			= NetLink::nif2ip( &rSelf );
	unsigned short iPort	= NetLink::nif2port( &rSelf );
	if ( !pTransmit->Open(sIP, iPort, iMaxBufferSize) )
	{
		delete pTransmit;
		return nullptr;
	}

	RelayPcManager* pMgr = new RelayPcManager(&rSelf, pTransmit, iKeepConnection);
	return pMgr;
}

P2PAgent* RendezvousAgentFactory::Join( const Network_IF& rSelf, P2PAgentHandler* pHandler,
	 const Network_IF& rRendezvous, unsigned int iMaxBufferSize, unsigned long iKeepConnection )
{
	DiagramStream* pTransmit	= new DiagramStream;
	std::string sIP			= NetLink::nif2ip( &rSelf );
	unsigned short iPort	= NetLink::nif2port( &rSelf );
	if ( !pTransmit->Open(sIP, iPort, iMaxBufferSize) )
	{
		delete pTransmit;
		return nullptr;
	}

	NetPcManager* pMgr = new NetPcManager(&rSelf, pHandler, pTransmit, iKeepConnection);
	pMgr->Relay( rRendezvous );

	return pMgr;
}