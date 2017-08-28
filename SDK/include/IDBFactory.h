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

#include <IDBEngine.h>
#include <IDBResult.h>
#include <IDBCommand.h>
#include <IDBProvider.h>

/** 
	MySQL 오브젝트 생성.
*/
struct IDBEngineFactory
{
	static IDBEngine* CreateMySQL();
};

/** 
	SQL 수행 결과값이 저장된 오브젝트 생성.
*/
struct IDBResultFactory
{
	static IDBResult* Create();
};

/** 
	DBProvider 생성.
*/
struct IDBProviderFactory
{
	/** 
		데이타베이스 직접 연결
	*/
	static IDBProvider* CreateDirect();
	/** 
		원격 데이타베이스 연결
	*/
	static IDBProvider* CreateRemote();
	/** 
		원격지의 SQL 실행 요청을 처리하는 Proxy
	*/
	static IDBProvider* CreateProxy();
};

/** 
	DBCommand 생성.
*/
struct IDBCommandFactory
{
	static IDBCommand* Create();
};