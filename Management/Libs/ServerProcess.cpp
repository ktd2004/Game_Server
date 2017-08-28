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
#include "ServerProcess.h"

ServerProcess::ServerProcess()
	: m_iPAID(0)
	, m_iSID(0)
	, m_sAlias(null_string)
	, m_sExecPath(".\\")
	, m_sExec(null_string)
	, m_sArgs(null_string)
	, m_sWorld(null_string)
	, m_bAutoStart(false)
	, m_iPID(0)
	, m_bBlock(false)
	, m_iMaxClient(0)
	, m_bSavePerf(false)
	, m_iSeq(0)
	, m_iState(eINIT)
	, m_iUsers(0)
{
}

ServerProcess::ServerProcess(
	uint32 iPAID,
	uint32 iSID,
	const std::string& sAlias,
	const std::string& sExecPath,
	const std::string& sExec,
	const std::string& sArgs,
	const std::string& sWorld,
	bool bAutoRestart )
	: m_iPAID(iPAID)
	, m_iSID(iSID)
	, m_sAlias(sAlias)
	, m_sExecPath(sExecPath)
	, m_sExec(sExec)
	, m_sArgs(sArgs)
	, m_sWorld(sWorld)
	, m_bAutoStart(bAutoRestart)
	, m_iPID(0)
	, m_bBlock(false)
	, m_iMaxClient(0)
	, m_bSavePerf(false)
	, m_iSeq(0)
	, m_iState(eINIT)
	, m_iUsers(0)
{
}

ServerProcess::ServerProcess( const ServerProcess& r )
	: m_iPAID(r.m_iPAID)
	, m_iSID(r.m_iSID)
	, m_sAlias(r.m_sAlias)
	, m_sExecPath(r.m_sExecPath)
	, m_sExec(r.m_sExec)
	, m_sArgs(r.m_sArgs)
	, m_sWorld(r.m_sWorld)
	, m_bAutoStart(r.m_bAutoStart)
	, m_iPID(r.m_iPID)
	, m_bBlock(r.m_bBlock)
	, m_iMaxClient(r.m_iMaxClient)
	, m_bSavePerf(r.m_bSavePerf)
	, m_iSeq(r.m_iSeq)
	, m_iState(r.m_iState)
	, m_iUsers(r.m_iUsers)
{

}

ServerProcess::~ServerProcess()
{
}

ServerProcess& ServerProcess::operator=( const ServerProcess& r )
{
	m_iPAID = r.m_iPAID;
	m_iSID = r.m_iSID;
	m_sAlias = r.m_sAlias;
	m_sExecPath = r.m_sExecPath;
	m_sExec = r.m_sExec;
	m_sArgs = r.m_sArgs;
	m_sWorld = r.m_sWorld;
	m_bAutoStart = r.m_bAutoStart;
	m_iPID = r.m_iPID;
	m_bBlock = r.m_bBlock;
	m_iMaxClient = r.m_iMaxClient;
	m_bSavePerf = r.m_bSavePerf;
	m_iSeq = r.m_iSeq;
	m_iState = r.m_iState;
	m_iUsers = r.m_iUsers;
	return *this;
}
