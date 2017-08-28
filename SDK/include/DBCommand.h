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
#include <IDBCommand.h>
/** 
	SQL 구문에 대한 정보를 저장하는 클래스
*/
class DBCommand 
	: public IDBCommand
{
	DBCommandType			m_eType;		///< 실행할 SQL 의 종류
	std::string				m_SQL;			///< SQL 구문
	std::vector<DBValue>	m_InParam;		///< SQL에 바인드할 파라미터를 저장할 컨테이너
	std::vector<DBValue>*	m_pParam;		///< ProxyProvider 내부에서 생성한 파라미터 컨테이너를 설정하기 위해서 사용
public:
	DBCommand();
	virtual ~DBCommand();
	/** 
		실행할 SQL를 저장한다.
		@param eCmdType SQL 종류
		@param SQL 실행할 SQL
	*/
	int32						SetCommand( DBCommandType eCmdType, const std::string& SQL ) override;
	/** 
		실행할 SQL에 전달할 파라미터를 설정한다.
		@param v 파라미터
		@param escape 파라미터가 문자열일 경우 이스케이프 문자열 처리를 한다.
	*/
	int32						AddParam( const DBValue& v, bool escape = false ) override;
	/** 
		실행할 SQL에 전달할 파라미터 컨테이너를 설정한다.
		@param v 파라미터 컨테이너
	*/
	int32						AddParam( std::vector<DBValue>& v ) override;
	/** 
		파라미터의 갯수를 조회한다.
	*/
	int32						GetParamCount( void ) override;
	/** 
		저장된 파라미터 데이타의 바이트수를 조회한다.
	*/
	int32						GetEastimateBytes( void ) override;
	/** 
		저장된 파라미터 컨테이너를 참조한다.
	*/
	std::vector<DBValue>&		GetParams( void ) override;
	/** 
		저장된 SQL 구문를 참조한다.
	*/
	const std::string&			GetCommand( void ) override;
	/** 
		저장된 SQL 구문의 종류를 조회한다.
	*/
	IDBCommand::DBCommandType	GetCommandType( void ) override;
};