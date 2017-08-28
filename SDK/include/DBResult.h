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

// dbengine
#include <DataTypes.h>
#include <DBValue.h>
#include <IDBResult.h>


/////////////////////////////////////////////////////////////////////////////////
PTR(DBField);
/** 
	SQL 샐행후 컬럼의 정보 저장하는 클래스
*/
class DBField
	: public single_ref
	, public IDBField
{
	std::string	m_sName;		///< 컬럼 이름
	DBValue::ValueType	m_Vt;	///< 컬럼의 자료형
public:
	DBField(DBValue::ValueType vt, const std::string& sName);
	virtual ~DBField();
	/** 
		컬럼의 이름을 조회
	*/
	const std::string& GetFieldName( void ) override;
	/** 
		컬럼의 자료형
	*/
	const DBValue::ValueType GetValueType() override;
};

/////////////////////////////////////////////////////////////////////////////////

PTR(DBRow);
/** 
	SQL 실행후 행데이타를 저장하고 있는 클래스
*/
class DBRow
	: public single_ref
	, public IDBRow
{
	friend class DBRecordset;
	StaticArray<DBValue> m_Cols;		///< SQL 실행후 컬럼데이타를 저장하는 컨테이너
public:
	DBRow(size_t iSize);
	virtual ~DBRow();
	/** 
		SQL 실행후 행데이타를 저장하고 있는 컨테이너를 참조한다.
	*/
	StaticArray<DBValue>&	GetStaticArray( void ) override;
	/** 
		SQL 실행후 조회된 컬럼의 갯수를 조회한다.
	*/
	int32					GetColumnCount( void ) override;
	/** 
		SQL 실행후 조회된 행데이타의 컬럼정보를 얻어온다.
	*/
	DBValue&				FetchCol(int32 i) override;
};

/////////////////////////////////////////////////////////////////////////////////

PTR(DBRecordset)
/** 
	SQL 실행후 조회된 복수개의 행데이타를 저장하고 있는 레코드셋 클래스
*/
class DBRecordset
	: public single_ref
	, public IDBRecordset
{
	int64					m_iAffectedRows;	///< SQL 실행에 영향을 받은 행의 갯수
	std::vector<DBFieldPtr> m_vFields;			///< SQL 실행으로 조회된 컬럼정보
	std::vector<DBRowPtr>	m_vRow;				///< SQL 실행으로 조회된 행데이타 저장할 컨테이너
	int32					m_iCol;				///< SQL 실행으로 조회된 컬럼데이타를 저장할 컬럼번호
	int32					m_iRow;				///< SQL 실행으로 조회된 컬럼데이타를 저장할 행번호
	int32					m_iFetchRow;		///< 현재 참조중인 행번호
	int32					m_iEastimateBytes;	///< SQL 실행으로 저장된 모든 데이타의 크기
public:
	DBRecordset();
	virtual ~DBRecordset();
	/** 
		SQL 실행으로 영향을 받은 행의 갯수를 설정한다.
		@param iAffectedRows SQL 실행으로 영향을 받은 행의 갯수
	*/
	void			OnAffectedRows( int64 iAffectedRows ) override;
	/** 
		SQL 실행으로 조회된 컬럼정보를 설정한다.
		@param Vt 컬럼의 자료형
		@param sName 컬럼의 이름
	*/
	void			OnField( DBValue::ValueType Vt, const std::string& sName ) override;
	/** 
		SQL 실행으로 조회된 행데이타를 저장할 공간을 생성한다.
	*/
	void			OnRow( void ) override;
	/** 
		SQL 실행으로 조회된 컬럼데이타를 저장할 공간을 생성한다.
	*/
	void			OnCol( const DBValue& v ) override;
	/** 
		SQL 실행으로 조회된 컬럼데이타를 저장할 공간을 생성한다.
	*/
	IDBRow*			FetchRow( void ) override;
	/** 
		SQL 실행으로 조회된 컬럼정보를 참조한다.
		@param iInx 참조할 컬럼정보의 위치
	*/
	IDBField*		GetField( int32 iInx ) override;
	/** 
		SQL 실행으로 조회된 컬럼정보를 갯수를 조회한다.
	*/
	int32			GetFieldCount( void ) override;
	/** 
		현재 참조 중인 행번호를 제일 처음으로 되돌린다.
	*/
	void			Rewind( void ) override;
	/** 
		SQL 실행으로 영향을 받은 행의 갯수를 조회한다.
	*/
	int64			GetAffectedRows( void ) override;
	/** 
		SQL 실행으로 저장된 모든 데이타의 크기를 조회한다.
	*/
	int32			GetEastimateBytes( void ) override;
};

/////////////////////////////////////////////////////////////////////////////////
PTR(DBResult);
/** 
	SQL 실행후 조회된 복수개의 레코드셋를 저장하고 있는 클래스
*/
class DBResult
	: public single_ref
	, public IDBResult
{
	std::vector<DBRecordsetPtr> m_vRecordset;			///< SQL 실행후 결과를 저장하고 있는 레코드셋 컨데이터
	int32						m_iRecordset;			///< SQL 실행으로 조회된 레코드셋를 저장할 번호
	int32						m_iFetchRecordset;		///< 현재 참조 중인 레코드셋 번호
public:
	DBResult();
	virtual ~DBResult();
	/** 
		SQL 실행으로 조회된 데이타를 저장할 레코드셋 공간을 생성한다.
	*/
	void			OnRecordset( void ) override;
	/** 
		SQL 실행으로 영향을 받은 행의 갯수를 설정한다.
		@param iAffectedRows SQL 실행으로 영향을 받은 행의 갯수
	*/
	void			OnAffectedRows(int64 iAffectedRows) override;
	/** 
		SQL 실행으로 조회된 컬럼정보를 설정한다.
		@param Vt 컬럼의 자료형
		@param sName 컬럼의 이름
	*/
	void			OnField(DBValue::ValueType Vt, const std::string& sName) override;
	/** 
		SQL 실행으로 조회된 행데이타를 저장할 공간을 생성한다.
	*/
	void			OnRow( void ) override;
	/** 
		SQL 실행으로 조회된 컬럼데이타를 저장할 공간을 생성한다.
	*/
	void			OnCol(const DBValue& v) override;
	/** 
		레코드셋의 갯수를 조회한다.
	*/
	int32			GetRecordsetCount( void ) override;
	/** 
		레코드셋을 꺼내온다.
	*/
	IDBRecordset*	FetchRecordset( void ) override;
	/** 
		현재 참조 중인 레코드셋을 제일 처음으로 되돌린다.
	*/
	void			Rewind( void ) override;
	/** 
		SQL 실행 결과로 영향을 받은 모든 행의 갯수를 조회한다.
	*/
	int64			GetAffectedRows( void ) override;
	/** 
		SQL 실행으로 저장된 모든 데이타의 크기를 조회한다.
	*/
	int32			GetEastimateBytes( void ) override;
};