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
	쓰레드간 이벤트 발생 및 감지 클래스
*/
class Signal
{
protected:
	HANDLE				m_hSignal;			///< 이벤트핸들

	Signal( const Signal& r ) {}

public:
	Signal();
	virtual ~Signal();

public:
	/**
		이벤트 발생을 대기한다.
		@param tTimeout 대기시간(ms)
	*/
	bool	Wait( unsigned int tTimeout = INFINITE );
	/**
		이벤트를 발생 시킵니다.
	*/
	void	Wakeup( void );
};

/** 
	RAII(Resource Acquisition Is Initialization)  패턴으로 구현된 시그널 발생 클래스
*/
class Sigset
{
private:
	Signal*			m_Sg;			///< 이벤트 발생/감지 클래스오브젝트 포인터.

	// disable copy constructor and assignment
	Sigset(const Sigset&) {}
	void operator =	(const Sigset&) {}

public:
	Sigset(Signal& sg) : m_Sg(&sg) {}
	Sigset(Signal* sg) : m_Sg(sg) {}
	/**
		Sigset 객체가 삭제되면 이벤트가 발생한다.
	*/
	virtual ~Sigset() { if ( m_Sg ) m_Sg->Wakeup(); }
	/**
		이벤트 발생을 취소한다.
	*/
	void Cancel( void ) { m_Sg = nullptr; }
};