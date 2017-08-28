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

#include <DBValue.h>

/** 
	SQL 샐행후 컬럼의 정보 저장하는 클래스
*/
struct IDBField
{
	/** 
		컬럼의 이름을 조회
	*/
	virtual const std::string&			GetFieldName( void ) = 0;
	/** 
		컬럼의 자료형
	*/
	virtual const DBValue::ValueType	GetValueType( void ) = 0;
};

/** 
	SQL 실행후 행데이타를 저장하고 있는 클래스
*/
struct IDBRow
{
	/** 
		SQL 실행후 행데이타를 저장하고 있는 컨테이너를 참조한다.
	*/
	virtual StaticArray<DBValue>&	GetStaticArray( void ) = 0;
	/** 
		SQL 실행후 조회된 컬럼의 갯수를 조회한다.
	*/
	virtual int32					GetColumnCount( void ) = 0;
	/** 
		SQL 실행후 조회된 행데이타의 컬럼정보를 얻어온다.
	*/
	virtual DBValue&				FetchCol( int32 i ) = 0;
};

/** 
	SQL 실행후 조회된 복수개의 행데이타를 저장하고 있는 레코드셋 클래스
*/
struct IDBRecordset
{
	/** 
		SQL 실행으로 영향을 받은 행의 갯수를 설정한다.
		@param iAffectedRows SQL 실행으로 영향을 받은 행의 갯수
	*/
	virtual void		OnAffectedRows( int64 iAffectedRows ) = 0;
	/** 
		SQL 실행으로 조회된 컬럼정보를 설정한다.
		@param Vt 컬럼의 자료형
		@param sName 컬럼의 이름
	*/
	virtual void		OnField( DBValue::ValueType Vt, const std::string& sName ) = 0;
	/** 
		SQL 실행으로 조회된 행데이타를 저장할 공간을 생성한다.
	*/
	virtual void		OnRow( void ) = 0;
	/** 
		SQL 실행으로 조회된 컬럼데이타를 저장할 공간을 생성한다.
	*/
	virtual void		OnCol( const DBValue& v ) = 0;
	/** 
		SQL 실행으로 조회된 컬럼정보를 갯수를 조회한다.
	*/
	virtual int32		GetFieldCount( void ) = 0;
	/** 
		SQL 실행으로 영향을 받은 행의 갯수를 조회한다.
	*/
	virtual int64		GetAffectedRows( void ) = 0;
	/** 
		SQL 실행으로 저장된 모든 데이타의 크기를 조회한다.
	*/
	virtual int32		GetEastimateBytes( void ) = 0;
	/** 
		SQL 실행으로 조회된 컬럼데이타를 저장할 공간을 생성한다.
	*/
	virtual IDBRow*		FetchRow( void ) = 0;
	/** 
		현재 참조 중인 행번호를 제일 처음으로 되돌린다.
	*/
	virtual void		Rewind( void ) = 0;
	/** 
		SQL 실행으로 조회된 컬럼정보를 참조한다.
		@param iInx 참조할 컬럼정보의 위치
	*/
	virtual IDBField*	GetField( int32 iInx ) = 0; 
};

/** 
	SQL 실행후 조회된 복수개의 레코드셋를 저장하고 있는 클래스
*/
class IDBResult
{
public:
	IDBResult() {}
	virtual ~IDBResult() {}
	/** 
		SQL 실행으로 조회된 데이타를 저장할 레코드셋 공간을 생성한다.
	*/
	virtual void			OnRecordset() = 0;
	/** 
		SQL 실행으로 영향을 받은 행의 갯수를 설정한다.
		@param iAffectedRows SQL 실행으로 영향을 받은 행의 갯수
	*/
	virtual void			OnAffectedRows(int64 iAffectedRows) = 0;
	/** 
		SQL 실행으로 조회된 컬럼정보를 설정한다.
		@param Vt 컬럼의 자료형
		@param sName 컬럼의 이름
	*/
	virtual void			OnField(DBValue::ValueType Vt, const std::string& sName) = 0;
	/** 
		SQL 실행으로 조회된 행데이타를 저장할 공간을 생성한다.
	*/
	virtual void			OnRow() = 0;
	/** 
		SQL 실행으로 조회된 컬럼데이타를 저장할 공간을 생성한다.
	*/
	virtual void			OnCol(const DBValue& v) = 0;
	/** 
		레코드셋의 갯수를 조회한다.
	*/
	virtual int32			GetRecordsetCount() = 0;
	/** 
		현재 참조 중인 레코드셋을 제일 처음으로 되돌린다.
	*/
	virtual void			Rewind() = 0;
	/** 
		SQL 실행 결과로 영향을 받은 모든 행의 갯수를 조회한다.
	*/
	virtual int64			GetAffectedRows( void ) = 0;
	/** 
		SQL 실행으로 저장된 모든 데이타의 크기를 조회한다.
	*/
	virtual int32			GetEastimateBytes() = 0;
	/** 
		레코드셋을 꺼내온다.
	*/
	virtual IDBRecordset*	FetchRecordset() = 0;
};