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
#include <ThreadFactory.h>

/** 
	쓰레드 생성, 실행, 종료 처리를 수행하느 클래스
*/
class ThreadImpl : public Thread
{
private:
	HANDLE		m_hThread;		///< 쓰레드 핸들
	DWORD		m_iThreadId;	///< 쓰레드 ID

public:
	ThreadImpl();
	virtual ~ThreadImpl();

public:
	/** 
		쓰레드를 정지상태로 실행한다.
		@param pRunCore 쓰레드에서 pRunCore의 Running() 함수를 호출한다.
		@param iBindCPU 쓰레드의 선호도
	*/
	bool				Start( ThreadRunnable *pRunCore, unsigned short iBindCPU ) override;
	/** 
		쓰레드를 동작 시킨다.
	*/
	bool				Resume( void ) override;
	/** 
		쓰레드를 종료 시킨다.
	*/
	bool				Terminate( void ) override;
	/** 
		쓰레드가 종료되기를 기다린다.
	*/
	bool				Wait( void ) override;
	/** 
		쓰레드의 우선순위를 설정한다.
	*/
	bool				SetPriority( int iPrio ) override;
	/** 
		쓰레드의 선호도를 조회한다.
	*/
	unsigned short		GetBindCPU( void ) override;
	/** 
		쓰레드의 선호도를 설정한다.
	*/
	void				SetBindCPU( unsigned short iCPU ) override;
	/** 
		쓰레드의 선호도 마스크를 조회한다.
	*/
	DWORD				GetAffinity( void ) override;
	/** 
		쓰레드의 선호도 마스크를 설정한다.
	*/
	bool				SetAffinity( DWORD dwMask ) override;
	/** 
		쓰레드의 핸들을 조회한다.
	*/
	inline HANDLE		GetHandle( void ) override { return m_hThread; }
	/** 
		쓰레드의 아이디를 조회한다.
	*/
	inline DWORD		GetThreadId( void ) override { return m_iThreadId; }

private:
	//< Thread Main
	static unsigned int __stdcall __ThreadEntryPoint( void *arg );
};
