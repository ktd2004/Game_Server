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
// dbengine
#include <Query.h>

Query::Query()
: m_iOid(0)
, m_iQid(0)
, m_iErrorCode(0)
, m_bSyncQry(false)
{
}

Query::Query( QUID iQid, IDBCommand::DBCommandType eCmd )
: m_iOid(0)
, m_iQid(iQid)
, m_eCommandType(eCmd)
, m_iErrorCode(0)
, m_bSyncQry(false)
{
}

Query::~Query()
{
}


void Query::SetStmt( const std::string& SQL )
{
	m_Sql = SQL;
}

const std::string& Query::GetStmt( void )
{
	return m_Sql;
}

QUID Query::GetQid()
{
	return m_iQid;
}


void Query::MakeCommand( IDBCommand* pCmd )
{
	pCmd->SetCommand( m_eCommandType, m_Sql );
	OnSetParam( pCmd );
}

bool Query::IsError( void )
{
	return m_iErrorCode != 0;
}


int32 Query::GetError( void )
{
	return m_iErrorCode;
}

const std::string& Query::GetErrorMsg( void )
{
	return m_sErrorMsg;
}

void Query::Sync( void )
{
	m_bSyncQry = true;
}

bool Query::IsSync( void )
{
	return m_bSyncQry;
}

void Query::OnError( int32 iErrorCode, const std::string& sErrorMsg )
{
	m_iErrorCode = iErrorCode;
	m_sErrorMsg = sErrorMsg;
}
