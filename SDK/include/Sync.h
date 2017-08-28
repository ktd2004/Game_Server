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
	�����尣�� ���ȹ�� �� ������� Ŭ����
*/
class Sync
{
private:
	volatile DWORD			m_iLocker;		///< ���ȹ�� ������ID
	volatile unsigned long	m_iLockCnt;		///< ���ȹ�� Ƚ��
	CRITICAL_SECTION		m_CS;			///< ���

	Sync(const Sync&) {}
	void operator =	(const Sync&) {}

public:
	Sync();
	virtual ~Sync();
	/** ��� */
	bool Lock( void );
	/** ������� */
	bool Unlock( void );
};

/** 
	RAII(Resource Acquisition Is Initialization)  �������� ������ ��� ȹ�� �� ������� Ŭ����
*/
class Guard
{
private:
	Sync*			m_Sy;			///< ���/������� Ŭ����������Ʈ ������.

	Guard(const Guard&) {}
	void operator =	(const Guard&) {}

public:
	/**
		Guard ��ü�� �����Ǹ� ���ȹ�� �Ѵ�.
	*/
	Guard(Sync& cs) : m_Sy(&cs) { m_Sy->Lock(); }
	/**
		Guard ��ü�� �����Ǹ� ���ȹ�� �Ѵ�.
	*/
	Guard(Sync* cs) : m_Sy(cs) { m_Sy->Lock(); }
	/**
		Guard ��ü�� �����Ǹ� ������� �Ѵ�.
	*/
	virtual ~Guard() { m_Sy->Unlock(); }
};