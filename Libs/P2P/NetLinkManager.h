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
#pragma once

//<
#include "PktTransmit.h"
#include "P2PPkt.h"

//<
#include <P2PAgent.h>

//< created by p2p agent factory
class NetLinkManager : public P2PAgent
{
	friend class Pc;
protected:
	P2PAgentHandler*	m_Handler;			// notify
	PktTransmit*		m_pTransmit;		// udp packet transmit
	NetLinkMap			m_NetLinkMap;		// established link
	NetLink*			m_Relay;			// relay link
	unsigned long		m_iKeepConnection;	// check alive
	Network_IF			m_Self;				// self network information

public:
	P2PNET_PACKET_LIST	m_Received;			// received packet list

public:
	NetLinkManager( const Network_IF* pNIF, P2PAgentHandler* pHandler, PktTransmit* pTransmit, unsigned long iKeepConnection );
	virtual ~NetLinkManager();

	//< P2PAgent
	NetLinkMap&			GetNetLinkMap( void ) override { return m_NetLinkMap; }
	Network_IF&			Self( void ) override { return m_Self; }
	NetLink*			Connect( const Network_IF& rNIF ) override;
	bool				Close( NetLink* pLink ) override;
	NetLink*			Find( const Network_IF& rNIF ) override;
	void				Clear( void ) override;
	virtual void		Process( unsigned long iWait ) override;
	NetLink*			Relay( void ) override;

	//< NetLinkManager
	void				ReceivePkt( unsigned long iWait );
	P2PAgentHandler*	Handler( void );
	P2PAgentHandler*	Handler( P2PAgentHandler* pHandler );
	NetLink*			Relay( const Network_IF& rNIF );

	virtual bool		OnConnected( NetLinkPtr spLink );
	virtual bool		OnClosed( NetLinkPtr spLink );
	virtual bool		OnReceived( NetLinkPtr spLink, Buffer* pBuf );
	virtual NetLink*	OnCreate( void ) = 0;
};