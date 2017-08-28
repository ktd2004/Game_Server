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

#include <IDBProvider.h>

class Query;
/** 
	DBEngine 추상클래스.
*/
class IDBEngine
{
public:

	IDBEngine() {}
	virtual ~IDBEngine() {}
	/** 
		데이타 베이스 연결
		@param rProp 데이타베이스 설정 정보
	*/
	virtual int32		Connect( const IDBProvider::Properties& rProp ) = 0;
	/** 
		데이타 베이스 종료
	*/
	virtual void		Close() = 0;
	/** 
		데이타 베이스 연결 여부 확인
	*/
	virtual bool		IsConnected() = 0;
	/** 
		SQL 실행
		@param qry 실행할 쿼리 오브젝트
		@return : 0 이외에는 실패
	*/
	virtual int32		Execute( Query* qry ) = 0;
	/** 
		SQL 수행 결과 조회
	*/
	virtual const char*	ErrorMsg( void ) = 0;
};