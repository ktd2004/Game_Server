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
#include "ManagementProtocol.h"

//////////////////////////////////////////////////////////////////////////
Buffer& operator<<( Buffer& pk, const ServerProcess& r )
{
	pk << r.m_iSeq
		<< r.m_iPAID
		<< r.m_iSID
		<< r.m_iPID
		<< r.m_sAlias
		<< r.m_sExecPath
		<< r.m_sExec
		<< r.m_sArgs
		<< r.m_sWorld
		<< r.m_iState
		<< r.m_bAutoStart
		<< r.m_bBlock
		<< r.m_iUsers
		<< r.m_iMaxClient
		<< r.m_bSavePerf;
	return pk;
}

Buffer& operator>>( Buffer& pk, ServerProcess& r )
{
	pk >> r.m_iSeq
		>> r.m_iPAID
		>> r.m_iSID
		>> r.m_iPID
		>> r.m_sAlias
		>> r.m_sExecPath
		>> r.m_sExec
		>> r.m_sArgs
		>> r.m_sWorld
		>> r.m_iState
		>> r.m_bAutoStart
		>> r.m_bBlock
		>> r.m_iUsers
		>> r.m_iMaxClient
		>> r.m_bSavePerf;
	return pk;
}

//////////////////////////////////////////////////////////////////////////
Buffer& operator<<( Buffer& pk, const IDBProvider::Properties& r )
{
	pk << r.sProviderName
		<< r.stDatabase.sHost
		<< r.stDatabase.sPort
		<< r.stDatabase.bAutoReconnect
		<< r.stDatabase.sCategory
		<< r.stDatabase.sUid
		<< r.stDatabase.sPassword
		<< r.stDatabase.sCharset;

	return pk;
}

Buffer& operator>>( Buffer& pk, IDBProvider::Properties& r )
{
	pk >> r.sProviderName
		>> r.stDatabase.sHost
		>> r.stDatabase.sPort
		>> r.stDatabase.bAutoReconnect
		>> r.stDatabase.sCategory
		>> r.stDatabase.sUid
		>> r.stDatabase.sPassword
		>> r.stDatabase.sCharset;
	return pk;
}