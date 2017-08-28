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
// mysql
#include <mysql.h>
// generic
#include <MacroFunc.h>
// dbengine
#include <DBCommand.h>
#include <IDBFactory.h>


DBCommand::DBCommand() : m_pParam(nullptr)
{}

DBCommand::~DBCommand()
{}

int32 DBCommand::SetCommand( DBCommandType eCmdType, const std::string& SQL )
{
	m_eType = eCmdType;
	m_SQL = SQL;
	return 0;
}

const std::string& DBCommand::GetCommand( void )
{
	return m_SQL;
}

IDBCommand::DBCommandType DBCommand::GetCommandType( void )
{
	return m_eType;
}

int32 DBCommand::AddParam( const DBValue& v, bool escape )
{
	// ProxyProvider 에서 네트웍으로 파라미터를 받을때는 ProxyQuery 에 선언된
	// 파라미터 컨테이너를 m_pParam 에 할당한다.
	if ( !m_pParam )
	{
		if ( escape && v.m_Size > 0 )
		{
			char* p = new char[(v.m_Size*2)+1];
			long len = mysql_escape_string( p, (const char*)v.Get(), v.Size() );
			DBValue t(p, len);
			safe_array_delete(p);
			m_InParam.push_back(t);
		}
		else
		{
			m_InParam.push_back(v);
		}
		return (int32)m_InParam.size() - 1;
	}

	return -1;
}

int32 DBCommand::AddParam( std::vector<DBValue>& v )
{
	m_pParam = &v;
	return (int32)m_pParam->size() - 1;
}

std::vector<DBValue>& DBCommand::GetParams( void )
{
	if ( m_pParam )
	{
		return *m_pParam;
	}
	else
	{
		return m_InParam;
	}
}

int32 DBCommand::GetParamCount( void )
{ 
	if ( !m_pParam )
	{
		return (int32)m_InParam.size();
	}
	else
	{
		return (int32)m_pParam->size();
	}
}

int32 DBCommand::GetEastimateBytes( void )
{
	size_t iSum = 0;
	std::vector<DBValue>& t = GetParams();
	for ( size_t i = 0; i < t.size(); i++ )
	{
		iSum += t[i].Size();
	}
	return (int32)iSum;
}
/// 

IDBCommand* IDBCommandFactory::Create( void )
{
	return new DBCommand;
}