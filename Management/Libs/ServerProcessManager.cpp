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
#include <Logger.h>
#include "ServerProcessManager.h"

//////////////////////////////////////////////////////////////////////////

ServerProcessManager::ServerProcessManager()
	: m_sPAgentName(null_string)
	, m_iPAID(0)
{
}

ServerProcessManager::ServerProcessManager( const std::string& sPAgentName, uint32 iPAID )
	: m_sPAgentName(sPAgentName)
	, m_iPAID(iPAID)
{
}

ServerProcessManager::~ServerProcessManager()
{
}

ServerProcessMap& ServerProcessManager::GetMap( void )
{
	return m_ServerProcessMap;
}

ServerProcessPtr ServerProcessManager::Add(
	uint32 iPAID,
	uint32 iSID,
	const std::string& sAlias,
	const std::string& sExecPath,
	const std::string& sExec,
	const std::string& sArgs,
	const std::string& sWorld,
	bool bAutoRestart )
{
	ServerProcessPtr spServer = new ServerProcess(
		iPAID,
		iSID,
		sAlias,
		sExecPath,
		sExec,
		sArgs,
		sWorld,
		bAutoRestart );
	return Add(spServer);
}

ServerProcessPtr ServerProcessManager::Add( ServerProcessPtr spServer )
{
	if ( !m_ServerProcessMap.insert(ServerProcessMap::value_type(spServer->m_iSID,spServer)).second )
		return nullptr;
	if ( !m_ServerProcessNameMap.insert(ServerProcessNameMap::value_type(spServer->m_sAlias,spServer)).second )
		return nullptr;
	return spServer;
}

void ServerProcessManager::Remove( uint32 iSID )
{
	notfound_map_ret( ServerProcessMap, it, m_ServerProcessMap, iSID, );
	m_ServerProcessNameMap.erase( it->second->m_sAlias );
	m_ServerProcessMap.erase( it );
}

void ServerProcessManager::Remove( const std::string& sAlias )
{
	notfound_map_ret( ServerProcessNameMap, it, m_ServerProcessNameMap, sAlias, );
	m_ServerProcessMap.erase( it->second->m_iSID );
	m_ServerProcessNameMap.erase( it );
}

ServerProcessPtr ServerProcessManager::Find( uint32 iSID )
{
	found_map_ret( ServerProcessMap, it, m_ServerProcessMap, iSID, it->second );
	return nullptr;
}

ServerProcessPtr ServerProcessManager::Find( const std::string& sAlias )
{
	found_map_ret( ServerProcessNameMap, it, m_ServerProcessNameMap, sAlias, it->second );
	return nullptr;
}
		
ServerProcessPtr ServerProcessManager::Update( ServerProcessPtr spServer )
{
	notfound_map_ret(ServerProcessMap, it, m_ServerProcessMap, spServer->m_iSID, nullptr);
	ServerProcess t = *(it->second);
	*(it->second) = *spServer.get();
	it->second->m_sAlias = t.m_sAlias;	
	it->second->m_sExecPath = t.m_sExecPath;
	it->second->m_sExec = t.m_sExec;		
	it->second->m_sArgs = t.m_sArgs;		
	it->second->m_sWorld = t.m_sWorld;
	it->second->m_bAutoStart = t.m_bAutoStart;	
	it->second->m_bBlock = t.m_bBlock;		
	it->second->m_iMaxClient = t.m_iMaxClient;
	return it->second;
}

void ServerProcessManager::Dump( void )
{
	std::string bar(40, '-');
	prn_inf( "  %s", bar.c_str() );
	prn_inf( ">> pagent name %s, id %d", cstr(m_sPAgentName), m_iPAID );
	prn_inf( "  %s", bar.c_str() );
	foreach_do( ServerProcessMap, it, m_ServerProcessMap )
	{
		prn_inf( "  + world       %s", cstr(it->second->m_sWorld) );
		prn_inf( "  + server name %s", cstr(it->second->m_sAlias) );
		prn_inf( "  + server id   %d", it->second->m_iSID );
		prn_inf( "  + process id  %d", it->second->m_iPID );
		prn_inf( "  %s", bar.c_str() );
	}
}