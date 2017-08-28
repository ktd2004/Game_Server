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

class DBValue;
/** 
	SQL구문 종류와 DBEngine에서 실행할 SQL문 및 파라미터를 설정하는 추상클래스.
*/
class IDBCommand
{
public:
	IDBCommand() {}
	virtual ~IDBCommand() {}

	/** 
		SQL구문 종류
	*/
	enum DBCommandType
	{
		ctSQL = 1,		///< SQL 문자열
		ctSP = 2,		///< SP 호출
	};
	/** 
		SQL구문 및 SQL의 종류를 설정한다.
		@param eCmdType SQL의 종류
		@param SQL SQL구문
	*/
	virtual int32						SetCommand( DBCommandType eCmdType, const std::string& SQL ) = 0;
	/** 
		SQL구문 파라미터를 설정한다.
		@param v 파라미터값
		@param escape 특수문사 처리를 할지 여부
	*/
	virtual int32						AddParam( const DBValue& v, bool escape = false ) = 0;
	/** 
		SQL구문 파라미터를 설정한다.
		@param v 파라미터값
	*/
	virtual int32						AddParam( std::vector<DBValue>& v ) = 0;
	/** 
		파라미터의 개수를 조회한다.
	*/
	virtual int32						GetParamCount( void ) = 0;
	/** 
		SQL을 실행한후 조회된 결과값의 Bytes 수를 계산한다.
	*/
	virtual int32						GetEastimateBytes( void ) = 0;
	/** 
		SQL구문을 얻어온다.
	*/
	virtual const std::string&			GetCommand( void ) = 0;
	/** 
		SQL구문의 종류를 얻어온다.
	*/
	virtual DBCommandType				GetCommandType( void ) = 0;
	/** 
		파라미터의 값이 저장된 컨테이너를 얻어온다.
	*/
	virtual const std::vector<DBValue>& GetParams( void ) = 0;
};
