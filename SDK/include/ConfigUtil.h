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
#include <IDBProvider.h>

/** 
	데이타 베이스 구성파일 로딩 유틸리티 모음.
*/
namespace util
{
	/** 
		구성파일의 데이타베이스 섹션에서 데이타베이스 직접 연결을 위해 필요한 설정값 로딩한다.
		내부에서 INILoader를 사용하여 구성 정보를 로딩하도록 구현되어 있다.
		@param sDBSection 데이타베이스섹션명
		@param config 데이타베이스 구정정보를 채워줄 구조체
	*/
	void load_dbcfg(const std::string& sDBSection, IDBProvider::Properties& config);
	/** 
		구성파일의 데이타베이스 섹션에서 데이타베이스 원격 연결을 위해 필요한 설정값 로딩한다.
		내부에서 INILoader를 사용하여 구성 정보를 로딩하도록 구현되어 있다.
		@param sDBSection 데이타베이스섹션명
		@param config 데이타베이스 구정정보를 채워줄 구조체
	*/
	void load_remotedbcfg(const std::string& sDBSection, IDBProvider::Properties& config);
	/** 
		데이타베이스 직접 연결 구조체의 정보를 출력한다.
		@param config 데이타베이스 구정정보
	*/
	void show_dbcfg(IDBProvider::Properties& config);
	/** 
		데이타베이스 원격 연결 구조체의 정보를 출력한다.
		@param config 데이타베이스 구정정보
	*/
	void show_remotedbcfg(IDBProvider::Properties& config);
}