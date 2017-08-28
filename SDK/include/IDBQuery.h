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

#include <IDBEngine.h>
#include <IDBResult.h>
#include <IDBCommand.h>
#include <IDBProvider.h>

class IDBQuery;
/** 
	@def SQL 실행 결과를 전달해주는 OnQuery 콜백함수를 무시하고 싶을때 사용한다.
*/
#define db_qry_null_ptr(t)						(void (t::*)( NetLinkPtr, IDBQuery*, IDBResult* ))nullptr
/** 
	@def SQL 실행 결과를 전달해주는 OnResult 콜백함수를 무시하고 싶을때 사용한다.
*/
#define db_rst_null_ptr(t)						(void (t::*)( NetLinkPtr, IDBQuery*, IDBResult* ))nullptr
/** 
	@def SQL 실행 결과를 전달해주는 OnError 콜백함수를 무시하고 싶을때 사용한다.
*/
#define db_err_null_ptr(t)						(void (t::*)( NetLinkPtr, IDBQuery*, int32, const std::string& ))nullptr
/** 
	@def SQL 실행 결과를 전달해주는 모든 콜백함수를 모두 무시하고 싶을때 사용한다.
*/
#define db_null_ptr(t)							db_qry_null_ptr(t), db_rst_null_ptr(t), db_err_null_ptr(t)
/** 
	@def SQL 실행 결과를 전달해주는 OnQuery 콜백함수만을 호출하고 싶을때 사용한다.
*/
#define db_exec_qry_callback(t,cbk)				cbk, db_rst_null_ptr(t), db_err_null_ptr(t)
/** 
	@def SQL 실행 결과를 전달해주는 OnResult 콜백함수만을 호출하고 싶을때 사용한다.
*/
#define db_exec_rst_callback(t,cbk)				db_qry_null_ptr(t), cbk, db_err_null_ptr(t)
/** 
	@def SQL 실행 결과를 전달해주는 OnError 콜백함수만을 호출하고 싶을때 사용한다.
*/
#define db_exec_err_callback(t,cbk)				db_qry_null_ptr(t), db_rst_null_ptr(t), cbk
/** 
	@def SQL 실행 결과를 전달해주는 OnQuery, OnResult 콜백함수만을 호출하고 싶을때 사용한다.
*/
#define db_exec_qry_rst_callback(t,qry,rst)		qry,rst,db_err_null_ptr(t)
/** 
	@def SQL 실행 결과를 전달해주는 OnQuery, OnError 콜백함수만을 호출하고 싶을때 사용한다.
*/
#define db_exec_qry_err_callback(t,qry,err)		qry,db_rst_null_ptr(t),err
/** 
	@def SQL 실행 결과를 전달해주는 OnQuery, OnError 콜백함수만을 호출하고 싶을때 사용한다.
*/
#define db_exec_rst_err_callback(t,rst,err)		db_qry_null_ptr(t),rst,err
/**
	@def SQL 실행시 파라미터로 바인딩할 상수 문자열을 정의하기 위해서 사용된다.
*/
#define db_string(s)				std::string(s)

/** 
	IDBExecuter에 의해 생성되는 쿼리오브젝트들의 최상위 클래스.
*/
class IDBQuery : public Query
{
public:
	functional_v3< void, NetLinkPtr, IDBQuery*, IDBResult* >	m_OnQuery;
	functional_v3< void, NetLinkPtr, IDBQuery*, IDBResult* >	m_OnResult;
	functional_v4< void, NetLinkPtr, IDBQuery*, int32, const std::string& > m_OnError;
	NetLinkPtr	m_spLink;

public:
	IDBQuery( QUID iQid, IDBCommand::DBCommandType e )
		: Query( iQid, e )
	{
	}
	void OnQuery( IDBResult* pResult )
	{
		if ( m_OnQuery )
		{
			m_OnQuery( m_spLink, this, m_spResult );
		}
	}
	void OnResult( void )
	{
		if ( m_OnResult )
		{
			m_OnResult( m_spLink, this, m_spResult );
		}
	}
	void OnError( int32 iErrorCode, const std::string& sErrorMsg )
	{
		if ( m_OnError )
		{
			m_OnError( m_spLink, this, iErrorCode, sErrorMsg );
		}
		else
		{
			prn_err( "code(%d), msg(%s)", iErrorCode, cstr(sErrorMsg) );
		}
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
class IDBExecuterQuery_v0 : public IDBQuery
{
public:
	IDBExecuterQuery_v0( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1>
class IDBExecuterQuery_v1 : public IDBQuery
{
public:
	T1			arg1;
public:
	IDBExecuterQuery_v1( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2>
class IDBExecuterQuery_v2 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
public:
	IDBExecuterQuery_v2( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3>
class IDBExecuterQuery_v3 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
public:
	IDBExecuterQuery_v3( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4>
class IDBExecuterQuery_v4 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
public:
	IDBExecuterQuery_v4( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5>
class IDBExecuterQuery_v5 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
public:
	IDBExecuterQuery_v5( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class IDBExecuterQuery_v6 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
	T6			arg6;
public:
	IDBExecuterQuery_v6( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
		pCmd->AddParam( arg6 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class IDBExecuterQuery_v7 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
	T6			arg6;
	T7			arg7;
public:
	IDBExecuterQuery_v7( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
		pCmd->AddParam( arg6 );
		pCmd->AddParam( arg7 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class IDBExecuterQuery_v8 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
	T6			arg6;
	T7			arg7;
	T8			arg8;
public:
	IDBExecuterQuery_v8( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
		pCmd->AddParam( arg6 );
		pCmd->AddParam( arg7 );
		pCmd->AddParam( arg8 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class IDBExecuterQuery_v9 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
	T6			arg6;
	T7			arg7;
	T8			arg8;
	T9			arg9;
public:
	IDBExecuterQuery_v9( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
		pCmd->AddParam( arg6 );
		pCmd->AddParam( arg7 );
		pCmd->AddParam( arg8 );
		pCmd->AddParam( arg9 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class IDBExecuterQuery_v10 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
	T6			arg6;
	T7			arg7;
	T8			arg8;
	T9			arg9;
	T10			arg10;
public:
	IDBExecuterQuery_v10( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
		pCmd->AddParam( arg6 );
		pCmd->AddParam( arg7 );
		pCmd->AddParam( arg8 );
		pCmd->AddParam( arg9 );
		pCmd->AddParam( arg10 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class IDBExecuterQuery_v11 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
	T6			arg6;
	T7			arg7;
	T8			arg8;
	T9			arg9;
	T10			arg10;
	T11			arg11;
public:
	IDBExecuterQuery_v11( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
		pCmd->AddParam( arg6 );
		pCmd->AddParam( arg7 );
		pCmd->AddParam( arg8 );
		pCmd->AddParam( arg9 );
		pCmd->AddParam( arg10 );
		pCmd->AddParam( arg11 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class IDBExecuterQuery_v12 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
	T6			arg6;
	T7			arg7;
	T8			arg8;
	T9			arg9;
	T10			arg10;
	T11			arg11;
	T12			arg12;
public:
	IDBExecuterQuery_v12( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
		pCmd->AddParam( arg6 );
		pCmd->AddParam( arg7 );
		pCmd->AddParam( arg8 );
		pCmd->AddParam( arg9 );
		pCmd->AddParam( arg10 );
		pCmd->AddParam( arg11 );
		pCmd->AddParam( arg12 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class IDBExecuterQuery_v13 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
	T6			arg6;
	T7			arg7;
	T8			arg8;
	T9			arg9;
	T10			arg10;
	T11			arg11;
	T12			arg12;
	T13			arg13;
public:
	IDBExecuterQuery_v13( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
		pCmd->AddParam( arg6 );
		pCmd->AddParam( arg7 );
		pCmd->AddParam( arg8 );
		pCmd->AddParam( arg9 );
		pCmd->AddParam( arg10 );
		pCmd->AddParam( arg11 );
		pCmd->AddParam( arg12 );
		pCmd->AddParam( arg13 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class IDBExecuterQuery_v14 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
	T6			arg6;
	T7			arg7;
	T8			arg8;
	T9			arg9;
	T10			arg10;
	T11			arg11;
	T12			arg12;
	T13			arg13;
	T14			arg14;
public:
	IDBExecuterQuery_v14( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
		pCmd->AddParam( arg6 );
		pCmd->AddParam( arg7 );
		pCmd->AddParam( arg8 );
		pCmd->AddParam( arg9 );
		pCmd->AddParam( arg10 );
		pCmd->AddParam( arg11 );
		pCmd->AddParam( arg12 );
		pCmd->AddParam( arg13 );
		pCmd->AddParam( arg14 );
	}
};

//////////////////////////////////////////////////////////////////////////
/** 
	IDBExecuter에 의해 실행되는 쿼리오브젝트 클래스.
*/
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class IDBExecuterQuery_v15 : public IDBQuery
{
public:
	T1			arg1;
	T2			arg2;
	T3			arg3;
	T4			arg4;
	T5			arg5;
	T6			arg6;
	T7			arg7;
	T8			arg8;
	T9			arg9;
	T10			arg10;
	T11			arg11;
	T12			arg12;
	T13			arg13;
	T14			arg14;
	T15			arg15;
public:
	IDBExecuterQuery_v15( QUID iQid, IDBCommand::DBCommandType e, const std::string& Sql )
		: IDBQuery( iQid, e )
	{
		m_Sql = Sql;
	}

	void OnSetParam( IDBCommand* pCmd )
	{
		pCmd->AddParam( arg1 );
		pCmd->AddParam( arg2 );
		pCmd->AddParam( arg3 );
		pCmd->AddParam( arg4 );
		pCmd->AddParam( arg5 );
		pCmd->AddParam( arg6 );
		pCmd->AddParam( arg7 );
		pCmd->AddParam( arg8 );
		pCmd->AddParam( arg9 );
		pCmd->AddParam( arg10 );
		pCmd->AddParam( arg11 );
		pCmd->AddParam( arg12 );
		pCmd->AddParam( arg13 );
		pCmd->AddParam( arg14 );
		pCmd->AddParam( arg15 );
	}
};
