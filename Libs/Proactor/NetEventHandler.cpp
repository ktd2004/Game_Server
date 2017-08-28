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
#include <Performance.h>
#include <Packet.h>
#include <NetEventHandler.h>
#include <TaskEventMsg.h>
#include <Errorcodes.h>
#include <Logger.h>
#include "ProactorImpl.h"


namespace
{
	Proactor_Counter& g_proactor_counter = PROACTOR_COUNTER;
}

//<
NetEventHandler::NetEventHandler( unsigned int iTaskSchedulerID )
	: m_pTaskScheduler(nullptr)
	, m_pProactor(nullptr)
	, m_iTaskSchedulerID(iTaskSchedulerID)
{
	m_pTaskScheduler = TaskSchedulerFactory::Get(iTaskSchedulerID);
}

NetEventHandler::~NetEventHandler()
{
}


void NetEventHandler::NotifyTask( unsigned int iTaskSchedulerID )
{
	m_pTaskScheduler = TaskSchedulerFactory::Get( iTaskSchedulerID );
}

TaskScheduler* NetEventHandler::NotifyTask( void )
{
	return m_pTaskScheduler;
}


size_t NetEventHandler::Parse( NetLinkPtr spLink, Buffer* pBuf, Buffer::QType& lst )
{
	if ( !Packet::parse(pBuf, lst, spLink->MaxBufSize()) )
	{
		while( !lst.empty() )
		{
			Buffer* p = lst.front(); lst.pop_front();
			p->release();
		}
		spLink->Kick( eKICK_INVALID_LENGTH, __FUNCTION__);
		SetLastError( PROACTOR_PACKET_PARSE_FAILED );
		return 0;
	}

	return lst.size();
}

//<
class NetAcceptedEvent : public TaskEventMsg
{
public:
	NetFunc_V1& m_Fn;
	NetLinkPtr	m_spLink;
	Proactor*	m_pProactor;

	NetAcceptedEvent(NetLinkPtr spLink, NetFunc_V1& fn, Proactor* pProactor)
		: m_spLink(spLink)
		, m_Fn(fn)
		, m_pProactor(pProactor)
	{
		locked_inc( g_proactor_counter.iNetAcceptEvents );
	}
	virtual ~NetAcceptedEvent()
	{
		locked_dec( g_proactor_counter.iNetAcceptEvents );
	}
	//<
	void OnHandle( void )
	{
		m_pProactor->RegLink( m_spLink );
		if ( m_Fn )
			m_Fn(m_spLink);
	}
};

void NetEventHandler::OnNetAccepted( NetLinkPtr spLink )
{
	if (spLink == nullptr)
		return;
	TCPLink* pLink = static_cast<TCPLink*>(spLink.get());
	if ( pLink->GetCreatedSocketCount() <= 1 )
	{
		prn_sys("accept count (0), reserved accept");
		pLink->Prepare();
	}
	if ( m_pTaskScheduler )
		m_pTaskScheduler->Post( new NetAcceptedEvent(spLink, m_OnAccepted, m_pProactor) );
}

//<
class NetConnectedEvent : public TaskEventMsg
{
public:
	NetFunc_V1& m_Fn;
	NetLinkPtr	m_spLink;
	Proactor*	m_pProactor;

	NetConnectedEvent(NetLinkPtr spLink, NetFunc_V1& fn, Proactor* pProactor)
		: m_spLink(spLink)
		, m_Fn(fn)
		, m_pProactor(pProactor)
	{
		locked_inc( g_proactor_counter.iNetConnectEvents );
	}
	virtual ~NetConnectedEvent()
	{
		locked_dec( g_proactor_counter.iNetConnectEvents );
	}
	//<
	void OnHandle( void )
	{
		m_pProactor->RegLink( m_spLink );
		if ( m_Fn )
			m_Fn(m_spLink);
	}
};

void NetEventHandler::OnNetConnected( NetLinkPtr spLink )
{
	if (spLink == nullptr)
		return;

	if ( m_pTaskScheduler )
		m_pTaskScheduler->Post( new NetConnectedEvent(spLink, m_OnConnected, m_pProactor) );
}

//<
class NetReceivedEvent : public TaskEventMsg
{
public:
	NetFunc_V2&		m_Fn;
	NetLinkPtr		m_spLink;
	Buffer::QType	m_Que;
	SOCKADDR_IN		m_RemoteAddr;

	NetReceivedEvent(NetLinkPtr& spLink, NetFunc_V2& fn, Buffer::QType& rQue)
		: m_spLink(spLink.get())
		, m_Fn(fn)
	{
		m_Que.merge(rQue);
		if ( spLink.param() )
		{
			memcpy( &m_RemoteAddr, spLink.param(), sizeof(SOCKADDR_IN) );
		}
		locked_inc( g_proactor_counter.iNetReceiveEvents );
	}
	NetReceivedEvent(NetLinkPtr& spLink, NetFunc_V2& fn, Buffer* pBuf)
		: m_spLink(spLink.get())
		, m_Fn(fn)
	{
		m_Que.push_back(pBuf);
		if (spLink.param())
		{
			memcpy(&m_RemoteAddr, spLink.param(), sizeof(SOCKADDR_IN));
		}
		locked_inc(g_proactor_counter.iNetReceiveEvents);
	}
	virtual ~NetReceivedEvent()
	{
		Clear();
		locked_dec( g_proactor_counter.iNetReceiveEvents );
	}
	//<
	void OnHandle( void )
	{
		m_spLink.param( &m_RemoteAddr );
		if ( m_Fn )
		{
			while ( !m_Que.empty() )
			{
				Buffer* p = m_Que.front(); m_Que.pop_front();
				m_Fn(m_spLink, p);
				p->release();
			}
		}
	}

	void Clear()
	{
		while ( !m_Que.empty() )
		{
			Buffer* p = m_Que.front(); m_Que.pop_front();
			p->release();
		}
	}
};


class NetFunctorEvent : public TaskEventMsg
{
public:
	NetFunc_V2&		m_Fn;
	NetLinkPtr		m_spLink;
	Buffer*			m_pBuf;
	SOCKADDR_IN		m_RemoteAddr;

	NetFunctorEvent(NetLinkPtr& spLink, NetFunc_V2& fn, Buffer* pBuf)
		: m_spLink(spLink.get())
		, m_Fn(fn)
		, m_pBuf(pBuf)
	{
		if ( spLink.param() )
		{
			memcpy( &m_RemoteAddr, spLink.param(), sizeof(SOCKADDR_IN) );
		}
		locked_inc( g_proactor_counter.iNetFunctorEvents );
	}
	virtual ~NetFunctorEvent()
	{
		m_pBuf->release();
		locked_dec( g_proactor_counter.iNetFunctorEvents );
	}
	//<
	void OnHandle( void )
	{
		m_spLink.param( &m_RemoteAddr );
		if ( m_Fn )
		{
			m_Fn(m_spLink, m_pBuf);
		}
	}
};

//<
class RemoteCallMsg : public TaskEventMsg
{
public:
	NetLinkPtr		m_spLink;
	Buffer*			m_pBuf;
	rpc_base*		m_pProc;
	SOCKADDR_IN		m_RemoteAddr;
public:
	RemoteCallMsg(NetLinkPtr& spLink, rpc_base* pProc, Buffer* pBuf)
		: m_spLink(spLink.get())
		, m_pProc(pProc)
		, m_pBuf(pBuf)
	{
		if ( spLink.param() )
		{
			memcpy( &m_RemoteAddr, spLink.param(), sizeof(SOCKADDR_IN) );
		}
		locked_inc( g_proactor_counter.iNetRemoteCallEvents );
	}
	virtual ~RemoteCallMsg()
	{
		m_pBuf->release();
		locked_dec( g_proactor_counter.iNetRemoteCallEvents );
	}
	void OnHandle( void )
	{
		m_spLink.param( &m_RemoteAddr );
		m_pProc->stream( m_pBuf );
		m_pProc->call( m_spLink );
	}
};

void NetEventHandler::OnNetReceived( NetLinkPtr spLink, Buffer* pBuf )
{
	if (spLink == nullptr)
		return;

	Buffer::QType lst;

	if ( Parse(spLink, pBuf, lst) )
	{
		if (m_pTaskScheduler == nullptr)
		{
			while ( !lst.empty() )
			{
				Buffer* p = lst.front(); lst.pop_front();
				p->release();
			}
			return;
		}

		
		
		while (!lst.empty())
		{
			Buffer* p = lst.front(); lst.pop_front();
			unsigned int iProcID = Packet::id(*p);
			unsigned int iSize = Packet::length(*p);

			NetFunc_V2* pNetFn = m_Functor.Find(iProcID);
			if (pNetFn)
			{
				p->rd_ptr(sizeof(Packet::header));
				m_pTaskScheduler->Post(new NetFunctorEvent(spLink, *pNetFn, p));
			}
			else
			{
				RemoteProcedureMap::iterator it = m_RemoteProceduerMap.find(iProcID);
				if (it != m_RemoteProceduerMap.end())
				{
					p->rd_ptr(sizeof(Packet::header));
					m_pTaskScheduler->Post(new RemoteCallMsg(spLink, it->second, p));
				}
				else if (m_OnReceived)
				{
					m_pTaskScheduler->Post(new NetReceivedEvent(spLink, m_OnReceived, p));
				}
				else
				{
					p->release();
				}
			}
		}
	}
}

//<
class NetClosedEvent : public TaskEventMsg
{
public:
	NetFunc_V1& m_Fn;
	NetLinkPtr	m_spLink;
	Proactor*	m_pProactor;

	NetClosedEvent(NetLinkPtr spLink, NetFunc_V1& fn, Proactor* pProactor)
		: m_spLink(spLink)
		, m_Fn(fn)
		, m_pProactor(pProactor)
	{
		locked_inc( g_proactor_counter.iNetClosedEvents );
	}
	virtual ~NetClosedEvent()
	{
		locked_dec( g_proactor_counter.iNetClosedEvents );
	}
	//<
	void OnHandle( void )
	{
		m_pProactor->UnregLink( m_spLink );
		if ( m_Fn )
			m_Fn(m_spLink);
		m_spLink->Close();
	}
};

void NetEventHandler::OnNetClosed( NetLinkPtr spLink )
{
	if (spLink == nullptr)
		return;

	if ( m_pTaskScheduler )
		m_pTaskScheduler->Post( new NetClosedEvent(spLink, m_OnClosed, m_pProactor) );
}

bool NetEventHandler::Start( unsigned int iPriority, unsigned short iThreadCnt )
{
	if ( m_pProactor )
		return false;

	if ( !m_pTaskScheduler )
	{
		m_pTaskScheduler = TaskSchedulerFactory::Get(m_iTaskSchedulerID);
	}

	m_pProactor = ProactorFactory::Create( iPriority, iThreadCnt );
	return ProactorFactory::Start( m_pProactor->ID() );
}

void NetEventHandler::Stop( void )
{
	if ( m_pProactor )
	{
		while (true)
		{
			if ( ProactorFactory::Stop(m_pProactor->ID()) )
				break;
			::Sleep( 10 );
		}
	}
}

bool NetEventHandler::Listen(unsigned short iPort, const std::string& sHost, long iCreatedSockets,
	size_t iMaxBufSize, std::function<void(NetLinkPtr, bool)>* fnOnCreated)
{
	if ( !m_pProactor )
		return nullptr;

	Proactor::Property	propCfg;
	propCfg.sHost = sHost;
	propCfg.iPort = iPort;
	propCfg.pNetCallback = this;
	propCfg.iCreatedSockets = iCreatedSockets;
	propCfg.iMaxBufSize = iMaxBufSize;
	propCfg.fnOnCreated = fnOnCreated;

	return m_pProactor->Listen( propCfg );
}

NetLinkPtr NetEventHandler::Connect( unsigned short iPort, const std::string& sHost,
	size_t iMaxBufSize, std::function<void(NetLinkPtr, bool)>* fnOnCreated)
{
	if ( !m_pProactor )
		return nullptr;

	Proactor::Property	propCfg;
	propCfg.sHost = sHost;
	propCfg.iPort = iPort;
	propCfg.pNetCallback = this;
	propCfg.iMaxBufSize = iMaxBufSize;
	propCfg.fnOnCreated = fnOnCreated;

	return m_pProactor->Connect( propCfg );
}

NetLinkPtr NetEventHandler::Bind( unsigned short iPort, const std::string& sHost,
	size_t iMaxBufSize, std::function<void(NetLinkPtr, bool)>* fnOnCreated)
{
	if ( !m_pProactor )
		return nullptr;

	Proactor::Property	propCfg;
	propCfg.sHost = sHost;
	propCfg.iPort = iPort;
	propCfg.pNetCallback = this;
	propCfg.iMaxBufSize = iMaxBufSize;
	propCfg.fnOnCreated = fnOnCreated;

	return m_pProactor->Bind( propCfg );
}

size_t NetEventHandler::GetCount( unsigned short iLinkType )
{
	if ( m_pProactor )
	{
		return m_pProactor->GetCount( iLinkType );
	}

	return 0;
}

bool NetEventHandler::RegRemoteProcedure( unsigned int iRpcID, rpc_base* pProc )
{
	if ( m_RemoteProceduerMap.count(iRpcID) == 0 )
	{
		m_RemoteProceduerMap.insert( RemoteProcedureMap::value_type(iRpcID, pProc) );
		locked_inc( g_proactor_counter.iRemoteProcedures );
		return true;
	}
	else
	{
		prn_err( "duplicated rpc id (%d)", iRpcID );
		SetLastError( PROACTOR_DUPLICATE_REMOTE_PROCEDURE_ID );
		safe_delete(pProc);
		return false;
	}
}

Proactor* NetEventHandler::GetProactor( void )
{
	return m_pProactor;
}
