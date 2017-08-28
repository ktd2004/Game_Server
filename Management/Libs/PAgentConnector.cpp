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
#include <Application.h>
#include <PAgentConnector.h>

PAgentConnector::PAgentConnector()
	: NetEventHandler(1)
{
}

PAgentConnector::~PAgentConnector()
{
}

void PAgentConnector::Initialize( const PAgentConnector::Properties& Prop )
{
	m_Prop = Prop;

	SetConnectedFunc( this, &PAgentConnector::OnConnected );
	SetClosedFunc   ( this, &PAgentConnector::OnConnected );

	EXPORT_RPC_NM_V3( this, &PAgentConnector::OnServerConnectResult, bool, bool, uint32 );
	EXPORT_RPC_NM_V1( this, &PAgentConnector::OnServerShutdown, uint32 );
	EXPORT_RPC_NM_V1( this, &PAgentConnector::OnServerBlockClient, bool );
	EXPORT_RPC_NM_V1( this, &PAgentConnector::OnServerMaxClient, uint32 );
}

bool PAgentConnector::Connect( void )
{
//	printf( "agent mode................%d\n", Application::IsAgentMode() );
	if ( Application::IsAgentMode() )
	{
		m_PAgent = NetEventHandler::Connect( Application::GetAgentPort(), Application::GetAgentAddr() );
	}

	if ( m_Prop.bLogTransmit )
	{
		NetLink::toaddr( m_LoggerAddr, m_Prop.sLogsAddr, m_Prop.iLogsPort );
		m_Logger = NetEventHandler::Bind( 0, m_Prop.sBindAddr );
		prn_callback( this );
	}

	return false;
}

bool PAgentConnector::OnConnected( NetLinkPtr spLink )
{
	if ( spLink == m_PAgent )
	{
		rpc_OnPAgentRegSID( m_PAgent, Application::GetSID(), ::GetCurrentProcessId() );
		rpc_OnPAgentReport( m_PAgent, 
			Application::GetSID(),
			Application::GetBlockClient(),
			Application::GetMaxClient(),
			Application::GetUsers() );
		return true;
	}

	return false;
}

bool PAgentConnector::OnClosed( NetLinkPtr spLink )
{
	if ( m_PAgent == spLink )
	{
		if ( Application::IsAgentMode() )
		{
			m_PAgent = NetEventHandler::Connect( Application::GetAgentPort(), Application::GetAgentAddr() );
		}

		return true;
	}

	return false;
}

bool PAgentConnector::OnServerConnectResult( NetLinkPtr spLink, bool bRet, bool bBlock, uint32 iMaxClient )
{
	if ( bRet )
	{
		if ( m_Prop.pHandler )
		{
			Application::SetBlockClient( bBlock );
			Application::SetMaxClient( iMaxClient );
			m_Prop.pHandler->OnBlockClient( bBlock );
			m_Prop.pHandler->OnMaxClient( iMaxClient );
		}
	}
	else
	{
		Application::QuitApplication( eNORMAL_EXIT );
	}
	return true;
}

bool PAgentConnector::OnServerShutdown( NetLinkPtr spLink, uint32 iSeq )
{
	Application::QuitApplication( eNORMAL_EXIT );
	return true;
}

bool PAgentConnector::OnServerBlockClient( NetLinkPtr spLink, bool bBlock )
{
	if ( m_Prop.pHandler )
	{
		Application::SetBlockClient( bBlock );
		m_Prop.pHandler->OnBlockClient( bBlock );
		rpc_OnPAgentReport( m_PAgent, 
			Application::GetSID(),
			Application::GetBlockClient(),
			Application::GetMaxClient(),
			Application::GetUsers() );
	}
	return true;
}

bool PAgentConnector::OnServerMaxClient( NetLinkPtr spLink, uint32 iMaxClient )
{
	if ( m_Prop.pHandler )
	{
		Application::SetMaxClient( iMaxClient );
		m_Prop.pHandler->OnMaxClient( iMaxClient );
		rpc_OnPAgentReport( m_PAgent, 
			Application::GetSID(),
			Application::GetBlockClient(),
			Application::GetMaxClient(),
			Application::GetUsers() );
	}
	return true;
}

void PAgentConnector::OnWrite( uint32 iLevel, const char* pFrmt, va_list argp )
{
	if ( iLevel == eLOG_SEND && m_Logger )
	{
		char szBuf[2048];
		int32 iLen = vsprintf_s( szBuf, sizeof(szBuf), pFrmt, argp );
		Buffer* pBuf = get_OnLogTransmit( iLevel, szBuf );
		m_Logger->Send( pBuf, &m_LoggerAddr );
	}
}
