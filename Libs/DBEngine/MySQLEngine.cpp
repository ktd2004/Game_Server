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
#if defined(WIN32) || defined(_M_X64)
#include <winsock2.h>
#endif
#include <mysql.h>
#include <errmsg.h>

// dbengine
#include <IDBEngine.h>
#include <Query.h>
#include <DBValue.h>
#include <DBCommand.h>
#include <IDBResult.h>
#include <IDBFactory.h>


/////////////////////////////////////////////////////////////////////////////////

struct MySQLStmt
{
	MYSQL&					m_Mysql;
	MYSQL_RES*				m_MetaResultPtr;
	MYSQL_FIELD*			m_FieldsPtr;
	size_t					m_iParamCount;
	size_t					m_iNumOfFields;
	Query*					m_QryPtr;
	DBCommand				m_Cmd;
	std::string				m_Sql;

	MySQLStmt(MYSQL& Mysql, Query* q)
		: m_Mysql(Mysql)
		, m_FieldsPtr(nullptr)
		, m_QryPtr(q)
		, m_iParamCount(0)
		, m_iNumOfFields(0)
		, m_MetaResultPtr(nullptr)
	{
		Initialize(m_Mysql, m_QryPtr);
	}

	~MySQLStmt()
	{
		Finalize();
	}

	size_t BindVariable( std::string& sSrc, const std::string& sParam, size_t iOffset )
	{
		static const char BIND_PARAM = '?';
		static const char NULLCHAR	= '\0';
		static const char ESCAPE_CHAR = '\\';
		static const char SINGLE_QUATATION = '\'';

		bool bQuat = false;
		for ( size_t i = iOffset; i < sSrc.size()+1; ++i )
		{
			if ( sSrc[i] == NULLCHAR )
			{
				return std::string::npos;
			}
			else if ( sSrc[i] == ESCAPE_CHAR )
			{
				// \? 는 특수처리 해야 한다.
				if ( !bQuat && sSrc[i+1] == BIND_PARAM )
				{
					sSrc.erase( i, 1 );
					char* p = new char[(sParam.size()*2)+1];
					long iBytes = mysql_escape_string( p, (const char*)sParam.c_str(), sParam.size() );
					sSrc.replace( i, 1, p );
					safe_array_delete(p);
					iOffset = i + iBytes;
					break;
				}
			}
			else if ( sSrc[i] == SINGLE_QUATATION )
			{
				bQuat = bQuat ? false : true;
			}
			else if ( sSrc[i] == BIND_PARAM && !bQuat)
			{
				sSrc.replace( i, 1, sParam );
				iOffset = i + sParam.size();
				break;
			}
		}

		return iOffset;
	}

	inline void Initialize(MYSQL& mysql, Query* pQry)
	{
		pQry->MakeCommand( &m_Cmd );
		int32 iCount = m_Cmd.GetParamCount();
		std::vector<DBValue>& Params = m_Cmd.GetParams();

		switch( m_Cmd.GetCommandType() )
		{
		case IDBCommand::ctSP :
			m_Sql = "Call " + m_Cmd.GetCommand();
			break;
		default :
			m_Sql = m_Cmd.GetCommand();
			break;
		}

		size_t iOffset = 0;
		for ( int32 i = 0; i < iCount && iOffset != std::string::npos; i++ )
		{
			switch (Params[i].m_ValueType)
			{
			case DBValue::vtString :
			case DBValue::vtBinary :	// 바이너리 포멧은 escape 처리해야 한다.
				iOffset = BindVariable( m_Sql, "'" + Params[i].ToString() + "'", iOffset );
				break;
			default :
				iOffset = BindVariable( m_Sql, Params[i].ToString(), iOffset );
				break;
			}
		}
		m_Sql += ";";

		//		printf( "exec (%s)\n", cstr(m_Sql) );
	}
/////////////////////////////////////////////////////////////////////////////////

	inline void Finalize()
	{
		if ( m_MetaResultPtr )
		{
			mysql_free_result(m_MetaResultPtr);
			m_MetaResultPtr = nullptr;
		}
	}

/////////////////////////////////////////////////////////////////////////////////

	inline int32 Execute()
	{
		int32 iRet = 0;
		IDBResult* DBResultPtr = IDBResultFactory::Create();
		if ( mysql_query(&m_Mysql, m_Sql.c_str()) > 0 )
		{
			goto label_error;
		}

		while ( 1 )
		{
			// 결과를 얻어온다.
			m_MetaResultPtr = mysql_store_result(&m_Mysql);

			if (m_MetaResultPtr == nullptr)
			{	// except select
				int64 iAffectedRows =  mysql_affected_rows(&m_Mysql);
				if ( iAffectedRows > 0 )
				{
					DBResultPtr->OnRecordset();
					DBResultPtr->OnAffectedRows( iAffectedRows );
				}
			}
			else
			{	// select
				if ( 0 != FetchRecordset(DBResultPtr) )
				{
					goto label_error;
				}
				mysql_free_result(m_MetaResultPtr);
				m_MetaResultPtr = nullptr;
			}

			// 멀리 레코드셋을 리턴할 경우 다음 셋으로 이동시킨다.
			if ( mysql_next_result(&m_Mysql) < 0 )
			{
				break;
			}
		}

		m_QryPtr->m_spResult = DBResultPtr;
		m_QryPtr->OnQuery( DBResultPtr );
		return iRet;

label_error :
		iRet = mysql_errno(&m_Mysql);
		m_QryPtr->OnError( iRet, mysql_error(&m_Mysql) );
		delete DBResultPtr;
		Finalize();
		return iRet;
	}

	inline int32 FetchRecordset(IDBResult* DBResultPtr)
	{
		m_iNumOfFields = mysql_num_fields(m_MetaResultPtr);
		m_FieldsPtr = mysql_fetch_fields(m_MetaResultPtr);

		DBResultPtr->OnRecordset();
		for ( size_t i = 0; i < m_iNumOfFields; i++ )
		{
			MYSQL_FIELD& f = m_FieldsPtr[i]; 
			DBResultPtr->OnField( DBValue::vtString, f.name );
		}

		int64 iAffectedRows = mysql_num_rows( m_MetaResultPtr );

		MYSQL_ROW row = nullptr;
		while ((row = mysql_fetch_row(m_MetaResultPtr)) != nullptr)
		{
			unsigned long* pLengths = mysql_fetch_lengths(m_MetaResultPtr);
			DBResultPtr->OnRow();
			for ( size_t i = 0; i < m_iNumOfFields; i++ )
			{
				DBValue v;
				if ( row[i] )
				{
					v.Set(row[i], (size_t)pLengths[i]);
					DBResultPtr->OnCol( v );
				}
				else
				{
					DBResultPtr->OnCol( null_string );
				}
			}
		}

		DBResultPtr->OnAffectedRows( iAffectedRows );

		return 0;
	}
};

class MySQLConnection
	: public IDBEngine
{
private:
	MYSQL			m_DB;
	bool			m_bConnected;
public:
	MySQLConnection()
		: m_bConnected(false)
	{}
	virtual ~MySQLConnection()
	{ 
		Close();
	}

	bool SetCharacterSet(const IDBProvider::Properties& rProp)
	{
		stringlist lst;
		lst.push_back("set character_set_client=");
		lst.push_back("set character_set_connection=");
		lst.push_back("set character_set_database=");
		lst.push_back("set character_set_results=");
		lst.push_back("set character_set_server=");
		lst.push_back("set names ");

		std::string sql;
		foreach_do(stringlist, it, lst)
		{
			sql = *it + rProp.stDatabase.sCharset + ";";
			if ( 0 != mysql_query(&m_DB, sql.c_str()) )
			{
			}
		}

		return true;
	}

	int32 Connect( const IDBProvider::Properties& rProp )
	{
// 		if ( 0 == mysql_thread_init() )
// 		{ goto label_error; }

		if (&m_DB != mysql_init(&m_DB) )
		{
			goto label_error;
		}

		if ( rProp.stDatabase.bAutoReconnect )
		{
			my_bool b =1;
			if ( 0 != mysql_options(&m_DB, MYSQL_OPT_RECONNECT, (const char*)(&b)) )
			{
				goto label_error;
			}
		}

		if ( 0 != mysql_options(&m_DB, MYSQL_SET_CHARSET_NAME, rProp.stDatabase.sCharset.c_str()) )
		{ goto label_error; }

		if ( &m_DB == mysql_real_connect(
			&m_DB,
			rProp.stDatabase.sHost.c_str(),
			rProp.stDatabase.sUid.c_str(),
			rProp.stDatabase.sPassword.c_str(),
			rProp.stDatabase.sCategory.c_str(),
			toint32(rProp.stDatabase.sPort.c_str()),
			"",
			CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) )
		{
			m_bConnected = true;
			SetCharacterSet( rProp );
		}

label_error:
		return mysql_errno(&m_DB); // error 가 없는 경우 0 이다.
	}

	void Close()
	{
		if ( m_bConnected )
		{
			mysql_close(&m_DB);
			m_bConnected = false;
		}
	}

	bool IsConnected()
	{
		return m_bConnected;
	}

	// 정상 수행되면 0 리턴
	int32 Execute(Query* pQry)
	{
		MySQLStmt Stmt(m_DB, pQry);
		return Stmt.Execute();
	}

	const char* ErrorMsg( void )
	{
		return mysql_error(&m_DB);
	}
};

///
IDBEngine* IDBEngineFactory::CreateMySQL()
{ return new MySQLConnection; }