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
#include <DBEngine.h>
#include "ServerConfigManager.h"

///< Query 

class SelectServerConfig : public Query
{
	std::string			 m_sWorld;
	std::string          m_sSection;
	std::string          m_sSID;
	std::string          m_sFileName;
public:
	functional_v1<void, IDBResult*> m_Callback;
public:
	SelectServerConfig(const_string& sWorld, const_string& sSID, const_string& sSection )
		: Query(0, IDBCommand::ctSP)
		, m_sWorld(sWorld)
		, m_sSection(sSection)
		, m_sSID(sSID)
	{
		m_Sql = "SP_SelectServerConfig( ?, ?, ? )";
	}


	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( m_sWorld );
		pCmd->AddParam( toint32(m_sSID.c_str()) );
		pCmd->AddParam( m_sSection );
	}

	void OnQuery( IDBResult* spResult )
	{
		if ( m_Callback )
		{
			m_Callback(spResult);
		}
	}
};

class CreateServerConfig : public Query
{
	int32			m_iSeq;
	std::string		m_sWorld;
	std::string		m_sSID;
	std::string		m_sSection;
	std::string		m_sKey;
	std::string		m_sValue;
	std::string		m_sComment;

public:
	CreateServerConfig( 
		int32 iSeq,
		const_string& sWorld,
		const_string& sSID,
		const_string& sSection,
		const_string& sKey,
		const_string& sValue,
		const_string& sComment = "")
		: Query(0, IDBCommand::ctSP)
		, m_iSeq(iSeq)
		, m_sWorld(sWorld)
		, m_sSID(sSID)
		, m_sSection(sSection)
		, m_sKey(sKey)
		, m_sValue(sValue)
		, m_sComment(sComment)
	{
		m_Sql = "SP_CreateServerConfig( ?, ?, ?, ?, ?, ?, ? )";
	}

	void OnSetParam(IDBCommand* pCmd)
	{
		pCmd->AddParam(m_iSeq);
		pCmd->AddParam(m_sWorld);
		pCmd->AddParam(toint32(m_sSID.c_str()));
		pCmd->AddParam(m_sSection);
		pCmd->AddParam(m_sKey);
		pCmd->AddParam(m_sValue, true);
		pCmd->AddParam(m_sComment, true);
	}

	void OnQuery( IDBResult* spResult )
	{
		IDBRecordset* pRst = spResult->FetchRecordset();
		if( pRst )
		{
			IDBRow* pRow = nullptr;
			while( pRow = pRst->FetchRow() )
			{
				QueryResultHelper data( pRow );
				if ( toint32(cstr(data[0])) != 1 )
				{
					prn_err( "query error %s", cstr(m_Sql) );
				}
			}
		}
	}
};


class SelectServerProcess : public Query
{
	std::string			 m_sWorld;
	std::string          m_sSection;
public:
	functional_v1<void, IDBResult*> m_Callback;
public:
	SelectServerProcess(const_string& sWorld, const_string& sSection)
		: Query(0, IDBCommand::ctSQL)
		, m_sWorld(sWorld)
		, m_sSection(sSection)
	{
		m_Sql  = " SELECT SUBSTRING(section, 1, LENGTH(?)) as section, sid ";
		m_Sql += " FROM server_config WHERE world = ?";
		m_Sql += " AND section like ?";
		m_Sql += " group by SUBSTRING(section, 1, LENGTH(?)), sid";
	}


	void OnSetParam(IDBCommand* pCmd)
	{
		pCmd->AddParam(m_sSection);
		pCmd->AddParam(m_sWorld);
		pCmd->AddParam(m_sSection + "%");
		pCmd->AddParam(m_sSection);
	}

	void OnQuery(IDBResult* spResult)
	{
		if (m_Callback)
		{
			m_Callback(spResult);
		}
	}
};

ServerConfigManager::ServerConfigManager()
{

}

ServerConfigManager::~ServerConfigManager()
{

}

bool ServerConfigManager::Connect( IDBProvider::Properties& DBProp )
{
	m_spDb = IDBEngineFactory::CreateMySQL();

	int32 rst = m_spDb->Connect( DBProp );
	if ( rst != 0 )
	{
		prn_err( "%s", m_spDb->ErrorMsg() );
		return false;
	}

	return true;
}

bool ServerConfigManager::SaveDB(
	const std::string& sFileName,
	const std::string& sWorld,
	bool bDebug )
{
	if ( !m_INISeq.Load(sFileName) )
	{
		return false;
	}

	INISequence::INIMap& INI = m_INISeq.m_INIMap;
	for (INISequence::INIMap::iterator it = INI.begin(); it != INI.end(); ++it )
	{
		INISequence::Record r = it->second;
		if ( bDebug )
		{
			prn_inf( "[%02d] [%s] [%s] [%s] %s=%s %s", 
				it->first,
				cstr(sWorld),
				cstr(r.sSID),
				cstr(r.sSection),
				cstr(r.sKey),
				cstr(r.sValue),
				r.sComment.size() ? cstr(r.sComment) : "null" );
		}
		else
		{
			CreateServerConfig* pQry = new CreateServerConfig(
				it->first,
				sWorld,
				r.sSID,
				r.sSection,
				r.sKey,
				r.sValue,
				r.sComment);

			m_spDb->Execute( pQry );
			safe_delete( pQry );
		}
	}

	return true;
}

bool ServerConfigManager::SaveDB(
	const std::string sWorld,
	const std::string& sSID,
	const std::string& sSection,
	const std::string& sKey,
	const std::string& sValue,
	bool bDebug )
{
	if (m_spDb)
	{
		if ( bDebug )
		{
			prn_inf( "[%02d] [%s] [%s] [%s] %s=%s %s", 
				-1,
				cstr(sWorld),
				cstr(sSID),
				cstr(sSection),
				cstr(sKey),
				cstr(sValue) );
		}
		else
		{
			CreateServerConfig* pQry = new CreateServerConfig(
				-1,
				sWorld,
				cstr(sSID),
				cstr(sSection),
				cstr(sKey),
				cstr(sValue));
			m_spDb->Execute( pQry );
			safe_delete(pQry);
		}
	}

	return true;
}

bool ServerConfigManager::SaveFile(
	const std::string& sFileName,
	const std::string& sWorld,
	const std::string& sSID,
	const std::string& sSection )
{
	SelectServerConfig* q = new SelectServerConfig(sWorld, sSID, sSection);
	q->m_Callback.assign( this, &ServerConfigManager::OnResultSet );
	m_spDb->Execute( q );
	safe_delete(q);

	if ( !m_sResult.empty() )
	{
		FILE* fp = nullptr;
		if ( !sFileName.empty() )
		{
			fp = fopen(cstr(sFileName), "w" );
			if (fp == nullptr)
			{
				prn_err( "%s", errmsg );
				m_sResult = null_string;
				return false;
			}

			fprintf( fp, "%s", cstr(m_sResult) );
			fclose(fp);
		}

		m_sResult = null_string;
		return true;
	}

	return false;
}

void ServerConfigManager::OnResultSet( IDBResult* pResult )
{
	IDBRecordset* pRst = pResult->FetchRecordset();
	if( pRst )
	{
		IDBRow* pRow = nullptr;
		int32 iPos = 0;
		std::string sBeforeSection;
		while( pRow = pRst->FetchRow() )
		{
			QueryResultHelper data( pRow );
			if ( sBeforeSection.empty() )
			{
				// first row - create section
				m_sResult += "[" + data[1] + "]\n"; 
				sBeforeSection = data[1];
			}
			else if ( sBeforeSection != data[1] )
			{
				m_sResult += "\n";
				m_sResult += "[" + data[1] + "]\n"; 
				sBeforeSection = data[1];
			}
			m_sResult += data[2] + "=" + data[3] + "\n";
		}
	}
}


void ServerConfigManager::OnResultSetServerProcess(IDBResult* pResult)
{
	IDBRecordset* pRst = pResult->FetchRecordset();
	if (pRst)
	{
		IDBRow* pRow = nullptr;
		int32 iPos = 0;
		while (pRow = pRst->FetchRow())
		{
			QueryResultHelper data(pRow);
			m_sResult += data[0] + ": [" + data[1] + "]\n";
		}
	}
}

bool ServerConfigManager::DisplayConfig(
	const std::string sWorld,
	const std::string sSID,
	const std::string sSection )
{
	if ( sSection == "ClientPort" || sSection == "ServerPort" || sSID.size() > 0)
	{
		SelectServerConfig* q = new SelectServerConfig(sWorld, sSID, sSection);
		q->m_Callback.assign(this, &ServerConfigManager::OnResultSet);
		m_spDb->Execute(q);
		safe_delete(q);
		if (!m_sResult.empty())
		{
			prn_inf("\n%s", cstr(m_sResult));
			m_sResult = null_string;
			return true;
		}
	}
	else
	{
		SelectServerProcess* q = new SelectServerProcess(sWorld, sSection);
		q->m_Callback.assign(this, &ServerConfigManager::OnResultSetServerProcess);
		m_spDb->Execute(q);
		safe_delete(q);
		if (!m_sResult.empty())
		{
			prn_inf("\n%s", cstr(m_sResult));
			m_sResult = null_string;
			return true;
		}
	}
	
	return false;
}