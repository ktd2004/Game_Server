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
//<
#include <Packet.h>
#include <Logger.h>
#include "NetUtil.h"
#include "TCPEvent.h"
#include "ProactorImpl.h"

//<
ProactorImpl::ProactorImpl()
: m_hIOCP(IOCP::New())
, m_iUniqueID(0)
, m_iID(0)
, m_bOnShutdown(false)
{
}

ProactorImpl::~ProactorImpl()
{
	IOCP::Delete( m_hIOCP );
}

void ProactorImpl::Running( void )
{
	while (true)
	{
		DWORD			dwTransferredBytes = 0;
		ULONG_PTR		key = NULL;
		LPOVERLAPPED	ov = NULL;
		DWORD			err = NOERROR;

		if (!GetQueuedCompletionStatus( 
					m_hIOCP,
					&dwTransferredBytes,
					&key,
					&ov, INFINITE) )
		{
			err = GetLastError();
		}

// 		istrue_do( NOERROR != err, \
// 			prn_err("error code(%d) transferedbytes(%d) key(%p) ov(%p)", err, dwTransferredBytes, key, ov) );

		isfalse_continue(key);
		istrue_do( key == eProactor_FIN, break );
		isfalse_continue(ov);

		IOContext* pCtx = IOContext::Cast( ov );
		istrue_do( pCtx, pCtx->OnHandle(dwTransferredBytes, err) );
	}
}

bool ProactorImpl::Listen( const Proactor::Property& rProp )
{
	TCPLinkPtr spListen = new TCPLink( this );
	isfalse_ret( spListen, false );
	spListen->MaxBufSize( rProp.iMaxBufSize );
	if (rProp.fnOnCreated != nullptr)
	{
		std::function<void(NetLinkPtr, bool)>& fn = *rProp.fnOnCreated;
		if (fn)
		{
			fn(spListen, true);
		}
	}

	// 네트웍 이벤트를 전달할 오브젝트를 Accept Link 에 담아 놓으면
	// 클라 접속시 NetLink::Accept() 함수에서 Accept Link 가 쳐다보는 오브젝트로
	// 이벤트를 전달하도록 설정한다.
	spListen->m_pNetCallback = rProp.pNetCallback ? rProp.pNetCallback : (NetCallback*)this;
	isfalse_ret( spListen->Listen(rProp), false );

	long i = 0, iCnt = 0;
	for ( i = 0; i < rProp.iCreatedSockets; i++ )
	{
		if ( !spListen->Prepare() )
			break;
		iCnt++;
	}

	if ( iCnt )
	{
		RegLink( spListen );
		return true;
	}

	if (rProp.fnOnCreated != nullptr)
	{
		std::function<void(NetLinkPtr, bool)>& fn = *rProp.fnOnCreated;
		if (fn)
		{
			fn(spListen, true);
		}
	}

	return false;
}

NetLinkPtr ProactorImpl::Connect( const Proactor::Property& rProp )
{
	TCPLinkPtr spLink = new TCPLink( this );
	isfalse_ret( spLink, nullptr );
	spLink->MaxBufSize( rProp.iMaxBufSize );
	if (rProp.fnOnCreated != nullptr)
	{
		std::function<void(NetLinkPtr, bool)>& fn = *rProp.fnOnCreated;
		if (fn)
		{
			fn(spLink, true);
		}
	}

	spLink->m_pNetCallback = rProp.pNetCallback ? rProp.pNetCallback : (NetCallback*)this;

	if (spLink->Connect(rProp) == false)
	{
		if (rProp.fnOnCreated != nullptr)
		{
			std::function<void(NetLinkPtr, bool)>& fn = *rProp.fnOnCreated;
			if (fn)
			{
				fn(spLink, true);
			}
		}
		return nullptr;
	}

	RegLink( spLink );

	return spLink;
}

NetLinkPtr ProactorImpl::Bind( const Proactor::Property& rProp )
{
	UDPLinkPtr spLink = new UDPLink( this );
	isfalse_ret(spLink, nullptr);
	spLink->MaxBufSize( rProp.iMaxBufSize );
	if (rProp.fnOnCreated != nullptr)
	{
		std::function<void(NetLinkPtr, bool)>& fn = *rProp.fnOnCreated;
		if (fn)
		{
			fn(spLink, true);
		}
	}

	spLink->m_pNetCallback = rProp.pNetCallback ? rProp.pNetCallback : (NetCallback*)this;

	if (spLink->Bind(rProp) == false)
	{
		if (rProp.fnOnCreated != nullptr)
		{
			std::function<void(NetLinkPtr, bool)>& fn = *rProp.fnOnCreated;
			if (fn)
			{
				fn(spLink, true);
			}
		}

		return nullptr;
	}

	RegLink( spLink );

	return spLink;
}


void ProactorImpl::CloseListener( void )
{
	Guard lock(m_Sync);
	foreach_do( NetLinkMap, it, m_ListenMap )
	{
		it->second->Kick( eKICK_SHUTDOWN, __FUNCTION__ );
	}
	m_ListenMap.clear();
}

void ProactorImpl::KickAll( unsigned short iKickReason )
{
	Guard lock(m_Sync);
	foreach_do( NetLinkMap, it, m_RemoteMap )
	{
		if ( it->second->NetST() != eLINKST_ClOSING && it->second->NetST() != eLINKST_FIN )
		{
			it->second->Kick( iKickReason, __FUNCTION__ );
		}
	}
}

void ProactorImpl::Finalize()
{
	foreach_do( ProactorThreadList, it, m_Threads )
	{
		PostQueuedCompletionStatus(m_hIOCP, 0, eProactor_FIN, 0);
	}
}

HANDLE ProactorImpl::GetHandle()
{
	return m_hIOCP;
}

NetLinkMap& ProactorImpl::GetNetLinkMap( unsigned short iLinkType )
{
	switch ( iLinkType )
	{
	case eLINKTYPE_TCP_LISTEN  : return m_ListenMap;
	case eLINKTYPE_TCP_REMOTE  : return m_RemoteMap;
	case eLINKTYPE_TCP_CONNECT : return m_ConnectMap;
	case eLINKTYPE_UDP         : return m_UDPMap;
	default : return m_RemoteMap;
	}
}

bool ProactorImpl::RegLink( NetLinkPtr spLink )
{
	Guard lock(m_Sync);
	switch( spLink->LinkType() )
	{
	case eLINKTYPE_TCP_LISTEN  :
		{
			found_map_ret( NetLinkMap, it, m_ListenMap, spLink->m_iNID, false );
			prn_sys("reg listen link");
			return m_ListenMap.insert( NetLinkMap::value_type(spLink->m_iNID, spLink) ).second;
		}
	case eLINKTYPE_TCP_REMOTE  :
		{
			found_map_ret( NetLinkMap, it, m_RemoteMap, spLink->m_iNID, false );
			prn_sys("reg remote link");
			return m_RemoteMap.insert( NetLinkMap::value_type(spLink->m_iNID, spLink) ).second;
		}
	case eLINKTYPE_TCP_CONNECT :
		{
			found_map_ret( NetLinkMap, it, m_ConnectMap, spLink->m_iNID, false );
			prn_sys("reg connect link");
			return m_ConnectMap.insert( NetLinkMap::value_type(spLink->m_iNID, spLink) ).second;
		}
	case eLINKTYPE_UDP         :
		{
			found_map_ret( NetLinkMap, it, m_UDPMap, spLink->m_iNID, false );
			prn_sys("reg udp link");
			return m_UDPMap.insert( NetLinkMap::value_type(spLink->m_iNID, spLink) ).second;
		}
	default :
		return false;
	}
}

bool ProactorImpl::UnregLink( NetLinkPtr spLink )
{
	Guard lock(m_Sync);
	switch (spLink->LinkType())
	{
		case eLINKTYPE_TCP_LISTEN:
		{
			notfound_map_ret(NetLinkMap, it, m_ListenMap, spLink->m_iNID, false);
			prn_sys("unreg listen link");
			m_ListenMap.erase(it);
		}
		break;
		case eLINKTYPE_TCP_REMOTE:
		{
			notfound_map_ret(NetLinkMap, it, m_RemoteMap, spLink->m_iNID, false);
			prn_sys("unreg remote link");
			m_RemoteMap.erase(it);
		}
		break;
		case eLINKTYPE_TCP_CONNECT:
		{
			notfound_map_ret(NetLinkMap, it, m_ConnectMap, spLink->m_iNID, false);
			prn_sys("unreg connect link");
			m_ConnectMap.erase(it);
		}
		break;
		case eLINKTYPE_UDP:
		{
			notfound_map_ret(NetLinkMap, it, m_UDPMap, spLink->m_iNID, false);
			prn_sys("unreg connect link");
			m_UDPMap.erase(it);
		}
		break;
		default:
		{
			prn_err("unreg failed, link type unknonw(%u)", spLink->LinkType());
		}
		return false;
	}
	return true;
}


size_t ProactorImpl::GetCount( unsigned short iLinkType )
{
	switch ( iLinkType )
	{
	case eLINKTYPE_TCP_LISTEN  : return m_ListenMap.size();
	case eLINKTYPE_TCP_REMOTE  : return m_RemoteMap.size();
	case eLINKTYPE_TCP_CONNECT : return m_ConnectMap.size();
	case eLINKTYPE_UDP         : return m_UDPMap.size();
	default : return 0;
	}
}


bool ProactorImpl::OnKickAll( void )
{
	if( m_bOnShutdown == false )
	{
		m_bOnShutdown = true;
 		CloseListener();
 		KickAll(eKICK_SHUTDOWN);
		return GetCount(eLINKTYPE_TCP_REMOTE) == 0;
	}
	else
	{
		KickAll(eKICK_SHUTDOWN);
		return GetCount(eLINKTYPE_TCP_REMOTE) == 0;
	}
}

void ProactorImpl::OnNetAccepted( NetLinkPtr spLink )
{
	RegLink( spLink );
	TCPLink* pLink = static_cast<TCPLink*>(spLink.get());
	if ( pLink->GetCreatedSocketCount() <= 1 )
	{
		prn_sys( "accept count (0), reserved accept" );
		pLink->Prepare();
	}
}

void ProactorImpl::OnNetConnected( NetLinkPtr spLink )
{
	RegLink( spLink );
}

void ProactorImpl::OnNetReceived( NetLinkPtr spLink, Buffer* pBuf )
{
	pBuf->reset();
}

void ProactorImpl::OnNetClosed( NetLinkPtr spLink )
{
	UnregLink( spLink );
	spLink->Close();
}

bool ProactorImpl::Create( unsigned short iBindCPU, unsigned short iThreadCnt )
{
	for ( unsigned short i = 0; i < iThreadCnt; i++ )
	{
		Thread* pThr = ThreadFactory::Start( this, &ProactorImpl::Running, iBindCPU );
		if ( !pThr )
			return false;
		m_Threads.push_back(pThr);
	}

	return true;
}

void ProactorImpl::Resume( void )
{
	foreach_do( ProactorThreadList, it, m_Threads )
	{
		(*it)->Resume();
	}
}

void ProactorImpl::Wait( void )
{
	foreach_do( ProactorThreadList, it, m_Threads )
	{
		(*it)->Wait();
	}
}