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
	�����尣 �̺�Ʈ �߻� �� ���� Ŭ����
*/
class Signal
{
protected:
	HANDLE				m_hSignal;			///< �̺�Ʈ�ڵ�

	Signal( const Signal& r ) {}

public:
	Signal();
	virtual ~Signal();

public:
	/**
		�̺�Ʈ �߻��� ����Ѵ�.
		@param tTimeout ���ð�(ms)
	*/
	bool	Wait( unsigned int tTimeout = INFINITE );
	/**
		�̺�Ʈ�� �߻� ��ŵ�ϴ�.
	*/
	void	Wakeup( void );
};

/** 
	RAII(Resource Acquisition Is Initialization)  �������� ������ �ñ׳� �߻� Ŭ����
*/
class Sigset
{
private:
	Signal*			m_Sg;			///< �̺�Ʈ �߻�/���� Ŭ����������Ʈ ������.

	// disable copy constructor and assignment
	Sigset(const Sigset&) {}
	void operator =	(const Sigset&) {}

public:
	Sigset(Signal& sg) : m_Sg(&sg) {}
	Sigset(Signal* sg) : m_Sg(sg) {}
	/**
		Sigset ��ü�� �����Ǹ� �̺�Ʈ�� �߻��Ѵ�.
	*/
	virtual ~Sigset() { if ( m_Sg ) m_Sg->Wakeup(); }
	/**
		�̺�Ʈ �߻��� ����Ѵ�.
	*/
	void Cancel( void ) { m_Sg = nullptr; }
};