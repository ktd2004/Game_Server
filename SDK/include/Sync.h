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

/** 
	쓰레드간의 잠금획득 및 잠금해제 클래스
*/
class Sync
{
private:
	volatile DWORD			m_iLocker;		///< 잠금획득 쓰레드ID
	volatile unsigned long	m_iLockCnt;		///< 잠금획득 횟수
	CRITICAL_SECTION		m_CS;			///< 잠금

	Sync(const Sync&) {}
	void operator =	(const Sync&) {}

public:
	Sync();
	virtual ~Sync();
	/** 잠금 */
	bool Lock( void );
	/** 잠금해제 */
	bool Unlock( void );
};

/** 
	RAII(Resource Acquisition Is Initialization)  패턴으로 구현된 잠금 획득 및 잠금해제 클래스
*/
class Guard
{
private:
	Sync*			m_Sy;			///< 잠금/잠금해제 클래스오브젝트 포인터.

	Guard(const Guard&) {}
	void operator =	(const Guard&) {}

public:
	/**
		Guard 객체가 생성되면 잠금획득 한다.
	*/
	Guard(Sync& cs) : m_Sy(&cs) { m_Sy->Lock(); }
	/**
		Guard 객체가 생성되면 잠금획득 한다.
	*/
	Guard(Sync* cs) : m_Sy(cs) { m_Sy->Lock(); }
	/**
		Guard 객체가 삭제되면 잠금해제 한다.
	*/
	virtual ~Guard() { m_Sy->Unlock(); }
};