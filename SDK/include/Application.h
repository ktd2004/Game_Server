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

#include <ServerApp.h>

namespace Application
{
	/** 
		서버의 상태를 체크하는 간격을 설정한다.
		@param tDuration 체크간격(msec)
	*/
	void					CheckStateDuration( uint32 tDuration );
	/** 
		서버의 실행파일 이름을 조회한다.
	*/
	std::string				GetProgramName();
	/** 
		서버의 섹션명을 조회한다.
	*/
	std::string				GetSectionName();
	/** 
		서버의 구성파일 이름을 조회한다.
	*/
	std::string				GetSetupFileName();
	/** 
		PAgent가 서버를 실행했는지 조사한다.
	*/
	bool					IsAgentMode();
	/** 
		PAgent의 주소를 조회한다.
	*/
	std::string				GetAgentAddr();
	/** 
		PAgent의 포트를 조회한다.
	*/
	int32					GetAgentPort();
	/** 
		서버아이디를 조회한다.
	*/
	int32					GetSID();
	/** 
		서버아이디를 설정한다.
	*/
	void					SetSID(int32 iServerID);
	/** 
		서버를 종료한다.
		@param iExitCode 종료코드
	*/
	void					QuitApplication(int32 iExitCode);
	/** 
		서버종료 코드를 조회한다.
	*/
	int32					ExitCode();
	/** 
		클라이언트 접속 제한을 설정한다.
		@param bBlock 접속제한 여부
	*/
	void					SetBlockClient(bool bBlock);
	/** 
		클라이언트 접속 제한 여부를 조회한다.
	*/
	bool					GetBlockClient(void);
	/** 
		최대 클라이언트수를 설정한다.
		@param iMaxClient 최대클라이언트수
	*/
	void					SetMaxClient(uint32 iMaxClient);
	/** 
		최대 클라이언트수를 조회한다.
	*/
	uint32					GetMaxClient(void);
	/** 
		현재 접속된 클라이언트수를 설정한다.
		@param iUsers 현재 접속된 클라이언트수
	*/
	void					SetUsers(uint32 iCurrentClient);
	/** 
		현재 접속된 클라이언트수를 조회한다.
	*/
	uint32					GetUsers(void);
	/** 
		서버설정값을 조회한다.
	*/
	ServerApp::Properties*	GetProperty( void );
};