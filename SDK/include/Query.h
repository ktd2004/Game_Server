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

#include <Ptr.h>
#include <FastList.h>
#include <IDBResult.h>
#include <IDBCommand.h>

class IDBCommand;
class IDBResult;

/** 
	SQL 실행 정보를 저장하고 있는 클래스.

	IDBProvider 오브젝트에 전달되어 SQL을 실행되고 결과 값이 Query 오브젝트에 저장된다.
*/
class Query
	: public listnode(Query)
{
public:
#ifdef USE_UTIL_LIST_AT_QUERY_QUEUE
	typedef util::list<Query> QType;
#else
	typedef std::list<Query*> QType;
#endif

	typedef IDBCommand::DBCommandType QueryType;
	uint64				m_iOid;			///< 쿼리오브젝트의 고유ID
	spRef<IDBResult>	m_spResult;		///< SQL 실행 결과

protected:
	const QUID		m_iQid;				///< SQL의 고유아이디
	std::string		m_Sql;				///< 실행할 SQL
	QueryType		m_eCommandType;		///< SQL의 종류
	int32			m_iErrorCode;		///< 에러코드
	std::string		m_sErrorMsg;		///< 에러메세지
	bool			m_bSyncQry;			///< 동기화 여부

public:
	Query();
	Query(QUID iQid, IDBCommand::DBCommandType eCmd = IDBCommand::ctSP );
	virtual ~Query();

	/** 
		실행할 SQL을 설정한다.
		@param sql 실행할 SQL 구문
	*/
	void				SetStmt(const std::string& sql);
	/** 
		실행할 SQL을 조회한다.
	*/
	const std::string&	GetStmt( void );
	/** 
		실행할 SQL의 고유아이디를 얻어온다.
	*/
	QUID				GetQid( void );
	/** 
		DBProvider에서 쿼리오브젝트의 SQL에 파리미터값을 바인드 하기 위해서 호출된다. 
		@param pCmd 파리미터값을 바인드하기 위해 OnSetParam 에 전달된 DBCommand 오브젝트
	*/
	void				MakeCommand( IDBCommand* pCmd );
	/** 
		SQL 구문에 오류가 발생했는지 여부
	*/
	bool				IsError( void );
	/** 
		SQL 구문에 발생한 오류코드 조회
	*/
	int32				GetError( void );
	/** 
		SQL 구문에 발생한 오류메세지를 조회
	*/
	const std::string&	GetErrorMsg( void );
	/** 
		동기화 SQL로 설정한다.
	*/
	void				Sync( void );
	/** 
		동기화 SQL인지 조사한다.
	*/
	bool				IsSync( void );
	/** 
		SQL에 파리미터값을 바인드 하기 위해서 DBProvider에서 쿼리오브젝트의 OnSetParam 함수가 호출된다.
		@param pCmd 파리미터값을 바인드하기 위해 OnSetParam 에 전달된 DBCommand 오브젝트
	*/
	virtual void	OnSetParam( IDBCommand* pCmd ) {}
	/** 
		SQL실행 결과를 전달하기 위해서 DBProvider에서 호출된다.
		@param pResult SQL 실행 결과를 담고 있는 오브젝트
	*/
	virtual void	OnQuery( IDBResult* pResult ) {}
	/** 
		SQL구문의 오류가 발생했을때 DBProvider에서 호출된다.
		@param iErrorCode SQL 구문 오류코드
		@param sErrorMsg SQL 구문 오류메세지
	*/
	virtual void	OnError( int32 iErrorCode, const std::string& sErrorMsg );
	/** 
		SQL실행 결과를 전달하기 위해서 DBProvider에 연결된 테스크스케줄러에서 호출된다.
		@param pResult SQL 실행 결과를 담고 있는 오브젝트
	*/
	virtual void	OnResult( void ) {}
};
