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

#include "ServerProcessManager.h"

//////////////////////////////////////////////////////////////////////////
typedef std::map<uint32, ServerProcessManagerPtr>		PAgentManagerMap;
typedef std::map<std::string, ServerProcessManagerPtr>	PAgentManagerNameMap;

class PAgentManager
{
private:
	PAgentManagerMap		m_PAgentManagerMap;
	PAgentManagerNameMap	m_PAgentManagerNameMap;

private:
	PAgentManager();
	PAgentManager( const PAgentManager& copy );
	virtual ~PAgentManager();

public:
	static PAgentManager*	instance();
	bool					Load( void );
	ServerProcessMap*		GetServerProcessMap( uint32 iPAID );
	PAgentManagerMap&		GetPAgentManagerMap( void );
	ServerProcessPtr		Add(
		const std::string& sPAgentName,
		uint32 iPAID,
		uint32 iSID,
		const std::string& sAlias,
		const std::string& sExecPath,
		const std::string& sExec, 
		const std::string& sArgs,
		const std::string& sWorld,
		bool bAutoRestart=false );
	ServerProcessPtr		Add( const std::string& sPAgentName, ServerProcessPtr spServer );
	void					Remove( uint32 iPAID, uint32 iSID );
	void					Remove( const std::string& sPAgentName, const std::string& sAlias );
	ServerProcessPtr		Find( uint32 iPAID, uint32 iSID );
	ServerProcessPtr		Find( const std::string& sPAgentName, const std::string& sAlias );
	ServerProcessManagerPtr	GetServerProcessManager( uint32 iPAID );
	ServerProcessManagerPtr	GetServerProcessManager( const std::string& sPAgentName );
	void					Dump( void );
};
