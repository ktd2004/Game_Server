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
	SQL 실행으로 조회된 데이타를 접근하기 위해서 사용하는 클래스

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

	// SQL 문 실행
	IDBExecuter::Run( pProvider, 0, IDBCommand::ctSQL, "select * from test.account where iUID = ?",
		1, 
		&inst, db_exec_qry_ptr(TestCallback, &TestCallback::OnQuery) );

	@endverbatim
*/
class QueryResultHelper
{
	StaticArray<DBValue>& m_Cols;		///< 행데이타를 컬럼데이타를 담고 있는 컨테이너를 참조
public:
	QueryResultHelper(IDBRow* row);
	virtual ~QueryResultHelper();
	/** 
		컬럼데이타를 인덱스 번호를 접근한다.
		@param i 컬럼 인덱스 번호
	*/
	const std::string& operator [] (int32 i) const;
};
