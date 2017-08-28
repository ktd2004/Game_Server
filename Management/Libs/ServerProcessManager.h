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
#include "ServerProcess.h"

//////////////////////////////////////////////////////////////////////////
typedef std::map<uint32, ServerProcessPtr>		ServerProcessMap;
typedef std::map<std::string, ServerProcessPtr>	ServerProcessNameMap;

PTR(ServerProcessManager);
class ServerProcessManager : public single_ref
{
private:
	ServerProcessMap		m_ServerProcessMap;
	ServerProcessNameMap	m_ServerProcessNameMap;
	NetLinkPtr				m_Agent;

public:
	std::string				m_sPAgentName;
	uint32					m_iPAID;

public:
	ServerProcessManager();
	ServerProcessManager(const std::string& sPAgentName, uint32 iPAID);
	virtual ~ServerProcessManager();

public:
	void					SetAgent( NetLinkPtr spAgent ) { m_Agent = spAgent; }
	NetLinkPtr				GetAgent( void ) { return m_Agent; }
	ServerProcessMap&		GetMap( void );
	ServerProcessPtr		Add(
		uint32 iPAID,
		uint32 iSID,
		const std::string& sAlias,
		const std::string& sExecPath,
		const std::string& sExec, 
		const std::string& sArgs,
		const std::string& sWorld,
		bool bAutoRestart=false );
	ServerProcessPtr		Add( ServerProcessPtr spServer );
	void					Remove( uint32 iSID );
	void					Remove( const std::string& sAlias );
	ServerProcessPtr		Find( uint32 iSID );
	ServerProcessPtr		Find( const std::string& sAlias );
	ServerProcessPtr		Update( ServerProcessPtr spServer );
	void					Dump( void );
};