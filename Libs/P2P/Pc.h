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

#include <NetLink.h>
#include <DataTypes.h>
#include "P2PPkt.h"
#include "PktTransmit.h"
#include "NetLinkManager.h"

//<
class Pc : public NetLink
{
public:
	//<
	NetLinkManager*		m_NetLinkManager;
	PktTransmit*		m_pTransmit;				// udp packet transmit
	P2PNET_PACKET_LIST	m_Reliable,					// send reliable packet list
						m_Reserved,					// send unreliable packet
						m_WaitReliable;				// wait reliable packet
	unsigned long		m_Latency[MAX_LATENCY];		// latency
	unsigned long		m_iSyncTm;					// sync time
	unsigned short		m_iSyncCnt;					// sync count
	unsigned long		m_iSelfAddr;				// self addr ( assigned by sync ack )
	unsigned short		m_iSelfPort;				// self port ( assigned by sync ack )
	unsigned long		m_iKeepConnection;			// keep connection interval
	unsigned long		m_iReceivedPktTm;			// packet received time
	unsigned long		m_iAvgLatency;				// avg latency

	//<
	PKTSEQ				m_iUnreliablePktSeq,		// unreliable packet seq
						m_iReliablePktSeq,			// reliable packet seq
						m_iWishReliablePktSeq,		// wish reliable packet seq
						m_iControlPktSeq,			// control packet seq
						m_iWishControlPktSeq;		// wish control packet seq

public:
	Pc();
	Pc(NetLinkManager* pNetLinkMgr, PktTransmit* pTransmit);
	virtual ~Pc();
	void Init(NetLinkManager* pNetLinkMgr, PktTransmit* pTransmit);

public:
	//< NetLink
	bool			Send( const char* pPkt, unsigned int iLen, bool bReliable = true ) override;
	bool			Send( Buffer* pBuf, bool bReliable = true ) override;

	//< Pc
	void			Clear( void );
	void			Sync( void );
	void			KeepConnection( void );
	bool			CheckAlive( void );
	void			Latency( unsigned long iLatency );
	unsigned long	Latency( void );
	static int		SortLatency( const void *arg1, const void *arg2 );

	int				OnSync( Buffer* pBuf );
	int				OnSyncAck( Buffer* pBuf );
	int				OnKeepConnection( Buffer* pBuf );

	virtual void	Process( void );
	virtual int		OnReceived( Buffer* pBuf ) = 0;
};