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
	������ ����, ����, ���� ó���� �����ϴ� Ŭ����
*/
class ThreadImpl : public Thread
{
private:
	HANDLE		m_hThread;		///< ������ �ڵ�
	DWORD		m_iThreadId;	///< ������ ID

public:
	ThreadImpl();
	virtual ~ThreadImpl();

public:
	/** 
		�����带 �������·� �����Ѵ�.
		@param pRunCore �����忡�� pRunCore�� Running() �Լ��� ȣ���Ѵ�.
		@param iBindCPU �������� ��ȣ��
	*/
	bool				Start( ThreadRunnable *pRunCore, unsigned short iBindCPU ) override;
	/** 
		�����带 ���� ��Ų��.
	*/
	bool				Resume( void ) override;
	/** 
		�����带 ���� ��Ų��.
	*/
	bool				Terminate( void ) override;
	/** 
		�����尡 ����Ǳ⸦ ��ٸ���.
	*/
	bool				Wait( void ) override;
	/** 
		�������� �켱������ �����Ѵ�.
	*/
	bool				SetPriority( int iPrio ) override;
	/** 
		�������� ��ȣ���� ��ȸ�Ѵ�.
	*/
	unsigned short		GetBindCPU( void ) override;
	/** 
		�������� ��ȣ���� �����Ѵ�.
	*/
	void				SetBindCPU( unsigned short iCPU ) override;
	/** 
		�������� ��ȣ�� ����ũ�� ��ȸ�Ѵ�.
	*/
	DWORD				GetAffinity( void ) override;
	/** 
		�������� ��ȣ�� ����ũ�� �����Ѵ�.
	*/
	bool				SetAffinity( DWORD dwMask ) override;
	/** 
		�������� �ڵ��� ��ȸ�Ѵ�.
	*/
	inline HANDLE		GetHandle( void ) override { return m_hThread; }
	/** 
		�������� ���̵� ��ȸ�Ѵ�.
	*/
	inline DWORD		GetThreadId( void ) override { return m_iThreadId; }

private:
	//< Thread Main
	static unsigned int __stdcall __ThreadEntryPoint( void *arg );
};
