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

#include <IDBQuery.h>

/** 
	SQL실행은 편리하게 사용하기 위한 템플릿 함수 모음.

	@verbatim

	class TestCallback
	{
	public:
		TestCallback() {}

		void OnQuery( NetLinkPtr pLink, IBaseQuery* pQry, IDBResult* pResult )
		{
		}
	};

	INILoader::instance()->Load( ".\\Test.ini" );
	IDBProvider::Properties config;

	// 데이타 베이스 설정
	util::load_dbcfg("DB_Account", config);
	// 서버 접속 설정
	util::load_remotedbcfg("Test", "DB_Account", config);
	// DB 설정 출력
	util::show_dbcfg(config);

	CreateTaskScheduler( 1, 100, 0 );
	StartTaskScheduler( 1 );

	IDBProvider* pProvider = IDBProviderFactory::CreateDirect();
	if( !pProvider )
	{
		return false;
	}

	int32 rst = pProvider->Startup( config );
	if( !rst )
	{
		return false;
	}

	TestCallback inst;

	// SQL 문 실행
	IDBExecuter::Run( pProvider, 0, IDBCommand::ctSQL, "select * from test.server_config where iSeq = ?",
		1, 
		&inst, db_exec_qry_ptr(TestCallback, &TestCallback::OnTestQuery) );

	IDBExecuter::Run( pProvider, 0, IDBCommand::ctSP, "SP_SelectPlayer( ?, ? )",
		23,
		db_string("NickName"),
		&qry, db_exec_qry_ptr(TestCallback, &TestCallback::OnTestQuery) );

	// 이스케이프 문자열을 파리미터로 전달해야 할 경우에는 \\? 를 사용한다.
	IDBExecuter::Run( pProvider, 0, IDBCommand::ctSQL, "select * from test.server_config where sWorld = \\?",
		1, 
		&inst, db_exec_qry_ptr(TestCallback, &TestCallback::OnTestQuery) );
	@endverbatim
*/
struct IDBExecuter
{
	/** 
		SQL을 실행합니다.
		주의) 파라미터에 상수문자열을 전달할때는 반드시 db_string 매크로를 사용해야 합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr)
	{
		IDBExecuterQuery_v0* pQry = new IDBExecuterQuery_v0(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		주의) 파라미터에 상수문자열을 전달할때는 반드시 db_string 매크로를 사용해야 합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, 
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr)
	{
		IDBExecuterQuery_v1<T1>* pQry = new IDBExecuterQuery_v1<T1>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}
	
	/** 
		SQL을 실행합니다.
		주의) 파라미터에 상수문자열을 전달할때는 반드시 db_string 매크로를 사용해야 합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2,
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr)
	{
		IDBExecuterQuery_v2<T1, T2>* pQry = new IDBExecuterQuery_v2<T1, T2>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}



	/** 
		SQL을 실행합니다.
		주의) 파라미터에 상수문자열을 전달할때는 반드시 db_string 매크로를 사용해야 합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3,
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v3<T1, T2, T3>* pQry = new IDBExecuterQuery_v3<T1, T2, T3>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		주의) 파라미터에 상수문자열을 전달할때는 반드시 db_string 매크로를 사용해야 합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4,
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v4<T1, T2, T3, T4>* pQry = new IDBExecuterQuery_v4<T1, T2, T3, T4>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}


	/** 
		SQL을 실행합니다.
		주의) 파라미터에 상수문자열을 전달할때는 반드시 db_string 매크로를 사용해야 합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5,
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v5<T1, T2, T3, T4, T5>* pQry = new IDBExecuterQuery_v5<T1, T2, T3, T4, T5>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		주의) 파라미터에 상수문자열을 전달할때는 반드시 db_string 매크로를 사용해야 합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param arg6 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5, class T6>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6,
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v6<T1, T2, T3, T4, T5, T6>* pQry 
			= new IDBExecuterQuery_v6<T1, T2, T3, T4, T5, T6>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->arg6 = arg6;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		주의) 파라미터에 상수문자열을 전달할때는 반드시 db_string 매크로를 사용해야 합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param arg6 파라미터
		@param arg7 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7,
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v7<T1, T2, T3, T4, T5, T6, T7>* pQry 
			= new IDBExecuterQuery_v7<T1, T2, T3, T4, T5, T6, T7>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->arg6 = arg6;
		pQry->arg7 = arg7;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		주의) 파라미터에 상수문자열을 전달할때는 반드시 db_string 매크로를 사용해야 합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param arg6 파라미터
		@param arg7 파라미터
		@param arg8 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7, const T8& arg8,
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v8<T1, T2, T3, T4, T5, T6, T7, T8>* pQry 
			= new IDBExecuterQuery_v8<T1, T2, T3, T4, T5, T6, T7, T8>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->arg6 = arg6;
		pQry->arg7 = arg7;
		pQry->arg8 = arg8;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param arg6 파라미터
		@param arg7 파라미터
		@param arg8 파라미터
		@param arg9 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7, const T8& arg8, const T9& arg9,
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v9<T1, T2, T3, T4, T5, T6, T7, T8, T9>* pQry 
			= new IDBExecuterQuery_v9<T1, T2, T3, T4, T5, T6, T7, T8, T9>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->arg6 = arg6;
		pQry->arg7 = arg7;
		pQry->arg8 = arg8;
		pQry->arg9 = arg9;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param arg6 파라미터
		@param arg7 파라미터
		@param arg8 파라미터
		@param arg9 파라미터
		@param arg10 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7, const T8& arg8, const T9& arg9, const T10& arg10,
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v10<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>* pQry 
			= new IDBExecuterQuery_v10<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->arg6 = arg6;
		pQry->arg7 = arg7;
		pQry->arg8 = arg8;
		pQry->arg9 = arg9;
		pQry->arg10 = arg10;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param arg6 파라미터
		@param arg7 파라미터
		@param arg8 파라미터
		@param arg9 파라미터
		@param arg10 파라미터
		@param arg11 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7, const T8& arg8, const T9& arg9, const T10& arg10, T11& arg11, 
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v11<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>* pQry 
			= new IDBExecuterQuery_v11<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->arg6 = arg6;
		pQry->arg7 = arg7;
		pQry->arg8 = arg8;
		pQry->arg9 = arg9;
		pQry->arg10 = arg10;
		pQry->arg11 = arg11;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param arg6 파라미터
		@param arg7 파라미터
		@param arg8 파라미터
		@param arg9 파라미터
		@param arg10 파라미터
		@param arg11 파라미터
		@param arg12 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7, const T8& arg8, const T9& arg9, const T10& arg10, T11& arg11, T12& arg12, 
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v12<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>* pQry 
			= new IDBExecuterQuery_v12<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->arg6 = arg6;
		pQry->arg7 = arg7;
		pQry->arg8 = arg8;
		pQry->arg9 = arg9;
		pQry->arg10 = arg10;
		pQry->arg11 = arg11;
		pQry->arg12 = arg12;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param arg6 파라미터
		@param arg7 파라미터
		@param arg8 파라미터
		@param arg9 파라미터
		@param arg10 파라미터
		@param arg11 파라미터
		@param arg12 파라미터
		@param arg13 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7, const T8& arg8, const T9& arg9, const T10& arg10, T11& arg11, T12& arg12, T13& arg13, 
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v13<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>* pQry 
			= new IDBExecuterQuery_v13<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->arg6 = arg6;
		pQry->arg7 = arg7;
		pQry->arg8 = arg8;
		pQry->arg9 = arg9;
		pQry->arg10 = arg10;
		pQry->arg11 = arg11;
		pQry->arg12 = arg12;
		pQry->arg13 = arg13;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param arg6 파라미터
		@param arg7 파라미터
		@param arg8 파라미터
		@param arg9 파라미터
		@param arg10 파라미터
		@param arg11 파라미터
		@param arg12 파라미터
		@param arg13 파라미터
		@param arg14 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7, const T8& arg8, const T9& arg9, const T10& arg10, T11& arg11, T12& arg12, T13& arg13, T14& arg14, 
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v14<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>* pQry 
			= new IDBExecuterQuery_v14<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->arg6 = arg6;
		pQry->arg7 = arg7;
		pQry->arg8 = arg8;
		pQry->arg9 = arg9;
		pQry->arg10 = arg10;
		pQry->arg11 = arg11;
		pQry->arg12 = arg12;
		pQry->arg13 = arg13;
		pQry->arg14 = arg14;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}

	/** 
		SQL을 실행합니다.
		@param pProvider 데이타베이스 연결제공자
		@param iQid 쿼리 고유아이디
		@param e 실행할 쿼리의 종류
		@param Sql 실행할 SQL문
		@param arg1 파라미터
		@param arg1 파라미터
		@param arg3 파라미터
		@param arg4 파라미터
		@param arg5 파라미터
		@param arg6 파라미터
		@param arg7 파라미터
		@param arg8 파라미터
		@param arg9 파라미터
		@param arg10 파라미터
		@param arg11 파라미터
		@param arg12 파라미터
		@param arg13 파라미터
		@param arg14 파라미터
		@param arg15 파라미터
		@param o 결과를 전달받을 오브젝트
		@param on_qry 결과를 전달할 콜백함수로 데이타베이스 프로바이더 쓰레드에서 실행된다.
		@param on_rst 결과를 전달할 콜백함수로 메인 쓰레드에서 실행된다.
		@param on_err SQL 수행후 에레 발생시 전달할 콜백함수로 프로바이더 쓰레드에서 실행된다.
		@param spLink 콜백에 전달될 NetLink 주소값
	*/
	template<class O, class F1, class F2, class F3, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
	static int32 Run( IDBProvider* pProvider, QUID iQid, IDBCommand::DBCommandType e, const std::string Sql,
		const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7, const T8& arg8, const T9& arg9, const T10& arg10, T11& arg11, T12& arg12, T13& arg13, T14& arg14, T15& arg15, 
		const O& o, F1 on_qry, F2 on_rst, F3 on_err, NetLinkPtr spLink = nullptr )
	{
		IDBExecuterQuery_v15<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>* pQry 
			= new IDBExecuterQuery_v15<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(iQid, e, Sql);
		pQry->m_spLink = spLink;
		pQry->arg1 = arg1;
		pQry->arg2 = arg2;
		pQry->arg3 = arg3;
		pQry->arg4 = arg4;
		pQry->arg5 = arg5;
		pQry->arg6 = arg6;
		pQry->arg7 = arg7;
		pQry->arg8 = arg8;
		pQry->arg9 = arg9;
		pQry->arg10 = arg10;
		pQry->arg11 = arg11;
		pQry->arg12 = arg12;
		pQry->arg13 = arg13;
		pQry->arg14 = arg14;
		pQry->arg15 = arg15;
		pQry->m_OnQuery.assign(o, on_qry);
		pQry->m_OnResult.assign(o, on_rst);
		pQry->m_OnError.assign(o, on_err);

		if ( pProvider->Push(pQry) > 0 )
		{
			return 1;
		}
		return 0;
	}
};
