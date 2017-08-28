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

#include <DataTypes.h>
#include <DBResult.h>

class DBValue;
struct IDBRow;
/** 
	SQL �������� ��ȸ�� ����Ÿ�� �����ϱ� ���ؼ� ����ϴ� Ŭ����

	@verbatim
	class TestCallback
	{
	public:
		TestCallback() {}

		void OnQuery( NetLinkPtr pLink, IBaseQuery* pQry, IDBResult* pResult )
		{
			IDBRecordset* pRst = spResult->FetchRecordset();
			if( pRst )
			{
				IDBRow* pRow = nullptr;
				while( pRow = pRst->FetchRow() )
				{
					QueryResultHelper data( pRow );
					m_PlayerInfo.m_sNickName = data[0];
					m_PlayerInfo.m_iGender = stouint32( data[1] );
					m_PlayerInfo.m_sBirth = data[2];
				}
			}

			return 0;
		}
	};

	TestCallback inst;

	// SQL �� ����
	IDBExecuter::Run( pProvider, 0, IDBCommand::ctSQL, "select * from test.account where iUID = ?",
		1, 
		&inst, db_exec_qry_ptr(TestCallback, &TestCallback::OnQuery) );

	@endverbatim
*/
class QueryResultHelper
{
	StaticArray<DBValue>& m_Cols;		///< �൥��Ÿ�� �÷�����Ÿ�� ��� �ִ� �����̳ʸ� ����
public:
	QueryResultHelper(IDBRow* row);
	virtual ~QueryResultHelper();
	/** 
		�÷�����Ÿ�� �ε��� ��ȣ�� �����Ѵ�.
		@param i �÷� �ε��� ��ȣ
	*/
	const std::string& operator [] (int32 i) const;
};
