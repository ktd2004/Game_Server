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
	SQL������ ���ϰ� ����ϱ� ���� ���ø� �Լ� ����.

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

	// ����Ÿ ���̽� ����
	util::load_dbcfg("DB_Account", config);
	// ���� ���� ����
	util::load_remotedbcfg("Test", "DB_Account", config);
	// DB ���� ���
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

	// SQL �� ����
	IDBExecuter::Run( pProvider, 0, IDBCommand::ctSQL, "select * from test.server_config where iSeq = ?",
		1, 
		&inst, db_exec_qry_ptr(TestCallback, &TestCallback::OnTestQuery) );

	IDBExecuter::Run( pProvider, 0, IDBCommand::ctSP, "SP_SelectPlayer( ?, ? )",
		23,
		db_string("NickName"),
		&qry, db_exec_qry_ptr(TestCallback, &TestCallback::OnTestQuery) );

	// �̽������� ���ڿ��� �ĸ����ͷ� �����ؾ� �� ��쿡�� \\? �� ����Ѵ�.
	IDBExecuter::Run( pProvider, 0, IDBCommand::ctSQL, "select * from test.server_config where sWorld = \\?",
		1, 
		&inst, db_exec_qry_ptr(TestCallback, &TestCallback::OnTestQuery) );
	@endverbatim
*/
struct IDBExecuter
{
	/** 
		SQL�� �����մϴ�.
		����) �Ķ���Ϳ� ������ڿ��� �����Ҷ��� �ݵ�� db_string ��ũ�θ� ����ؾ� �մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		����) �Ķ���Ϳ� ������ڿ��� �����Ҷ��� �ݵ�� db_string ��ũ�θ� ����ؾ� �մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		����) �Ķ���Ϳ� ������ڿ��� �����Ҷ��� �ݵ�� db_string ��ũ�θ� ����ؾ� �մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		����) �Ķ���Ϳ� ������ڿ��� �����Ҷ��� �ݵ�� db_string ��ũ�θ� ����ؾ� �մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		����) �Ķ���Ϳ� ������ڿ��� �����Ҷ��� �ݵ�� db_string ��ũ�θ� ����ؾ� �մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		����) �Ķ���Ϳ� ������ڿ��� �����Ҷ��� �ݵ�� db_string ��ũ�θ� ����ؾ� �մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		����) �Ķ���Ϳ� ������ڿ��� �����Ҷ��� �ݵ�� db_string ��ũ�θ� ����ؾ� �մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param arg6 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		����) �Ķ���Ϳ� ������ڿ��� �����Ҷ��� �ݵ�� db_string ��ũ�θ� ����ؾ� �մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param arg6 �Ķ����
		@param arg7 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		����) �Ķ���Ϳ� ������ڿ��� �����Ҷ��� �ݵ�� db_string ��ũ�θ� ����ؾ� �մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param arg6 �Ķ����
		@param arg7 �Ķ����
		@param arg8 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param arg6 �Ķ����
		@param arg7 �Ķ����
		@param arg8 �Ķ����
		@param arg9 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param arg6 �Ķ����
		@param arg7 �Ķ����
		@param arg8 �Ķ����
		@param arg9 �Ķ����
		@param arg10 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param arg6 �Ķ����
		@param arg7 �Ķ����
		@param arg8 �Ķ����
		@param arg9 �Ķ����
		@param arg10 �Ķ����
		@param arg11 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param arg6 �Ķ����
		@param arg7 �Ķ����
		@param arg8 �Ķ����
		@param arg9 �Ķ����
		@param arg10 �Ķ����
		@param arg11 �Ķ����
		@param arg12 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param arg6 �Ķ����
		@param arg7 �Ķ����
		@param arg8 �Ķ����
		@param arg9 �Ķ����
		@param arg10 �Ķ����
		@param arg11 �Ķ����
		@param arg12 �Ķ����
		@param arg13 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param arg6 �Ķ����
		@param arg7 �Ķ����
		@param arg8 �Ķ����
		@param arg9 �Ķ����
		@param arg10 �Ķ����
		@param arg11 �Ķ����
		@param arg12 �Ķ����
		@param arg13 �Ķ����
		@param arg14 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
		SQL�� �����մϴ�.
		@param pProvider ����Ÿ���̽� ����������
		@param iQid ���� �������̵�
		@param e ������ ������ ����
		@param Sql ������ SQL��
		@param arg1 �Ķ����
		@param arg1 �Ķ����
		@param arg3 �Ķ����
		@param arg4 �Ķ����
		@param arg5 �Ķ����
		@param arg6 �Ķ����
		@param arg7 �Ķ����
		@param arg8 �Ķ����
		@param arg9 �Ķ����
		@param arg10 �Ķ����
		@param arg11 �Ķ����
		@param arg12 �Ķ����
		@param arg13 �Ķ����
		@param arg14 �Ķ����
		@param arg15 �Ķ����
		@param o ����� ���޹��� ������Ʈ
		@param on_qry ����� ������ �ݹ��Լ��� ����Ÿ���̽� ���ι��̴� �����忡�� ����ȴ�.
		@param on_rst ����� ������ �ݹ��Լ��� ���� �����忡�� ����ȴ�.
		@param on_err SQL ������ ���� �߻��� ������ �ݹ��Լ��� ���ι��̴� �����忡�� ����ȴ�.
		@param spLink �ݹ鿡 ���޵� NetLink �ּҰ�
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
