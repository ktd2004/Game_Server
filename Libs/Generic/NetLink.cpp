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
#include "stdafx.h"
//<
#include <NetLink.h>

Network_IF::Network_IF()
	: m_iNID(0)
	, m_iAddr(0)
	, m_iPort(0)
{
}

Network_IF::Network_IF( TNID ID )
	: m_iNID(ID)
	, m_iAddr(0)
	, m_iPort(0)
{
}

Network_IF::Network_IF( TNID ID, const std::string& sHost, unsigned short iPort )
	: m_iNID(ID)
{
	this->m_iAddr = NetLink::ip2addr( sHost );	// network address
	this->m_iPort = htons( iPort );				// network port
}

//<
NetLink::NetLink()
	: m_iSt(eLINKST_NONE)
	, m_pUserData(nullptr)
	, m_iMaxBufSize(4096)
	, m_iLinkType(eLINKTYPE_NONE)
{
}

NetLink::NetLink(const NetLink& r)
	: m_iSt(eLINKST_NONE)
	, m_pUserData(nullptr)
	, m_iMaxBufSize(4096)
	, m_iLinkType(eLINKTYPE_NONE)
{
}

NetLink::~NetLink()
{
	if ( m_pUserData && 0 == m_pUserData->decref() )
	{
		interface_ref* pTmpUserData = (interface_ref*)locked_exchange_pointer(m_pUserData, nullptr);
		safe_delete(pTmpUserData);
	}
}

void NetLink::NetIF( const Network_IF& nif )
{
	this->m_iNID		= nif.m_iNID;
	this->m_iAddr		= nif.m_iAddr;
	this->m_iPort		= nif.m_iPort;
}

void NetLink::NetST( const eLinkST st )
{
	locked_exchange( m_iSt, st );
}

void NetLink::UserData( interface_ref* pUserData, unsigned short iUserDataType )
{
	if ( pUserData )
	{
		pUserData->incref();
	}

	m_iUserDataType = iUserDataType;
	if ( m_pUserData && 0 == m_pUserData->decref() )
	{
		interface_ref* pTmpUserData = (interface_ref*)locked_exchange_pointer(m_pUserData, nullptr);
		safe_delete(pTmpUserData);
	}

	locked_exchange_pointer(m_pUserData, pUserData);
}

std::string NetLink::nif2ip( const Network_IF* pNIF )
{
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = pNIF->m_iAddr;
	return inet_ntoa(addr.sin_addr);
}

unsigned short NetLink::nif2port( const Network_IF* pNIF )
{
	return ntohs(pNIF->m_iPort);
}

unsigned long NetLink::ip2addr( const std::string& sHost )
{
	SOCKADDR_IN addr;
	if ( sHost.empty() )
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(sHost.c_str());
		if(addr.sin_addr.s_addr == INADDR_NONE)
		{
			LPHOSTENT lpHosten = gethostbyname(sHost.c_str());
			isnull_ret(lpHosten, 0);
			addr.sin_addr.s_addr  = ((LPIN_ADDR)lpHosten->h_addr)->s_addr;
		}
	}

	return addr.sin_addr.s_addr;
}


unsigned short NetLink::port2addr( unsigned short iPort )
{
	return htons(iPort);
}

void NetLink::toaddr( Network_IF* pNIF, const std::string& sHost, unsigned short iPort )
{
	pNIF->m_iAddr = NetLink::ip2addr( sHost.c_str() );	// network address
	pNIF->m_iPort = htons( iPort );						// network port
}

void NetLink::toaddr( SOCKADDR_IN& addr, const std::string& sHost, unsigned short iPort )
{
	addr.sin_family	= AF_INET;
	addr.sin_port = htons(iPort);
	addr.sin_addr.s_addr  = NetLink::ip2addr( sHost );
}

void NetLink::toaddr( SOCKADDR_IN& addr, Network_IF& nif )
{
	addr.sin_family				= AF_INET;
	addr.sin_port				= nif.m_iPort;
	addr.sin_addr.S_un.S_addr	= nif.m_iAddr;
}

std::string NetLink::tostr( Network_IF* pNIF )
{
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = pNIF->m_iAddr;
	char szBuf[25]; sprintf_s( szBuf, sizeof(szBuf), "%s:%u", inet_ntoa(addr.sin_addr), ntohs(pNIF->m_iPort) );
	return szBuf;
}

std::string NetLink::tostr( SOCKADDR_IN* addr )
{
	char szBuf[25]; sprintf_s( szBuf, sizeof(szBuf), "%s:%u", inet_ntoa(addr->sin_addr), ntohs(addr->sin_port) );
	return szBuf;
}
