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
#include <NetFunction.h>

NetFunction::NetFunction()
{
}

NetFunction::~NetFunction()
{
	foreach_do ( NetEventFuncMap, it, m_NetFuncMap )
	{ safe_delete(it->second); }
	m_NetFuncMap.clear();
}

bool NetFunction::Reg( unsigned int iProtocol, NetFunc_V2* fn )
{
	NetEventFuncMap::iterator it = m_NetFuncMap.find(iProtocol);
	if ( it == m_NetFuncMap.end() )
	{
		m_NetFuncMap.insert( std::make_pair(iProtocol, fn) );
	}
	else
	{
		safe_delete(it->second);
		it->second = fn;
	}
	return true;
}

bool NetFunction::Call( unsigned int iProtocol, NetLinkPtr spLink, Buffer* pBuf )
{
	NetEventFuncMap::iterator it = m_NetFuncMap.find( iProtocol );
	if ( it != m_NetFuncMap.end() )
	{
		return (*it->second)(spLink, pBuf);
	}

	return false;
}

NetFunc_V2* NetFunction::Find( unsigned int iProtocol )
{
	found_map_ret(NetEventFuncMap, it, m_NetFuncMap, iProtocol, it->second );
	return nullptr;
}