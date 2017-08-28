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
#include <ProcessHandler.h>

/** 
	프로세스를 실행, 종료하거나 상태를 감시하는 전역함수를 제공하는 네임스페이스.
*/
namespace ProcessFactory
{
	/** 
		프로세스 상태를 감시하는 주기와 태스크스케줄러를 지정한다.
		@param iTaskSchedulerID 태스크스케줄러 ID
		@param iDuration 프로세스 상태를 감지하는 주기(ms)
	*/
	bool			Start( unsigned int iTaskSchedulerID, unsigned long iDuration );
	/**
		프로세스를 실행한후 프로세스 관리맵에 등록한다.
		@param Opt 프로세스 실행 옵션
		@param pHandler 프로세스 상태가 변경됬을때 호출될 이벤트 핸들러
	*/
	ProcessPtr		Spawn( const ProcessOption& Opt, ProcessHandler* pHandler, interface_ref* pUserData = nullptr );
	/**
		프로세스를 실행한다.
		@param Opt 프로세스 실행 옵션
	*/
	ProcessPtr		Spawn( const ProcessOption& Opt );
	/** 
		프로세스를 종료한후 프로세스 관리맵에서 제거한다.
		@param iProcessId 프로세스 ID
	*/
	bool			Terminate( DWORD iProcessId );
	/** 
		프로세스를 얻어온다.
		@param iProcessId 프로세스 ID
	*/
	ProcessPtr		GetProcess( DWORD iProcessId );
	/** 
		프로세스를 상태를 감시한다.
		@param iProcessId 프로세스 ID
		@param pHandler 프로세스 상태가 변경됬을때 호출될 이벤트 핸들러
		@param pUserData 프로세스 객체에 저장할 사용자 데이타
	*/
	ProcessPtr		Monitor( DWORD iProcessId, ProcessHandler* pHandler, interface_ref* pUserData );
	/** 
		프로세스의 실행파일 이름을 얻어온다.
	*/
	std::string		GetCurrentProcessName( void );
	/** 
		프로세스의 환경변수에서 지정된 Key 문자열을 조회하여 Key문자열에 설정된 값 얻어온다.(Key = Val)
		@param sKey 환경변수에서 조회할 Key 문자열
	*/
	std::string		GetEnvVariable( const std::string& sKey );
	/** 
		프로세스의 환경변수에 지정된 Key에 값을 설정한다.( Key = Val )
		@param sKey 환경변수의 Key 문자열
		@param sVal 환경변수에 설정할 값
	*/
	void			SetEnvVariable( const std::string& sKey, const std::string& sVal );
	/** 
		프로세스의 환경변수를 얻어온다.
	*/
	std::string		GetEnvString( void );
	/** 
		프로세스의 환경변수를 콘솔에 출력한다.
	*/
	void			DumpEnvString( const std::string& sEnv );
	/** 
		프로세스의 환경변수를 콘솔에 출력한다.
	*/
	void			DumpEnvString( char* pEnv );
};