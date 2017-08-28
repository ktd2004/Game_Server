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
#include <ProcessOption.h>

/** 
	프로세스의 실행, 종료 상태를 감시하는 이벤트 핸들러.
*/
struct ProcessHandler
{
	/** 
		프로세스가 실행될때 호출되는 이벤트 핸들러.
		@param spProcess 실행된 프로세스의 객체
	*/
	virtual void	OnLaunchProcess( ProcessPtr spProcess ) = 0;
	/** 
		프로세스가 종료된후 호출되는 이벤트 핸들러.
		@param spProcess 종료된 프로세스의 객체
	*/
	virtual void	OnExitProcess( ProcessPtr spProcess ) = 0;
};