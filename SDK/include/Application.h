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
		������ ���¸� üũ�ϴ� ������ �����Ѵ�.
		@param tDuration üũ����(msec)
	*/
	void					CheckStateDuration( uint32 tDuration );
	/** 
		������ �������� �̸��� ��ȸ�Ѵ�.
	*/
	std::string				GetProgramName();
	/** 
		������ ���Ǹ��� ��ȸ�Ѵ�.
	*/
	std::string				GetSectionName();
	/** 
		������ �������� �̸��� ��ȸ�Ѵ�.
	*/
	std::string				GetSetupFileName();
	/** 
		PAgent�� ������ �����ߴ��� �����Ѵ�.
	*/
	bool					IsAgentMode();
	/** 
		PAgent�� �ּҸ� ��ȸ�Ѵ�.
	*/
	std::string				GetAgentAddr();
	/** 
		PAgent�� ��Ʈ�� ��ȸ�Ѵ�.
	*/
	int32					GetAgentPort();
	/** 
		�������̵� ��ȸ�Ѵ�.
	*/
	int32					GetSID();
	/** 
		�������̵� �����Ѵ�.
	*/
	void					SetSID(int32 iServerID);
	/** 
		������ �����Ѵ�.
		@param iExitCode �����ڵ�
	*/
	void					QuitApplication(int32 iExitCode);
	/** 
		�������� �ڵ带 ��ȸ�Ѵ�.
	*/
	int32					ExitCode();
	/** 
		Ŭ���̾�Ʈ ���� ������ �����Ѵ�.
		@param bBlock �������� ����
	*/
	void					SetBlockClient(bool bBlock);
	/** 
		Ŭ���̾�Ʈ ���� ���� ���θ� ��ȸ�Ѵ�.
	*/
	bool					GetBlockClient(void);
	/** 
		�ִ� Ŭ���̾�Ʈ���� �����Ѵ�.
		@param iMaxClient �ִ�Ŭ���̾�Ʈ��
	*/
	void					SetMaxClient(uint32 iMaxClient);
	/** 
		�ִ� Ŭ���̾�Ʈ���� ��ȸ�Ѵ�.
	*/
	uint32					GetMaxClient(void);
	/** 
		���� ���ӵ� Ŭ���̾�Ʈ���� �����Ѵ�.
		@param iUsers ���� ���ӵ� Ŭ���̾�Ʈ��
	*/
	void					SetUsers(uint32 iCurrentClient);
	/** 
		���� ���ӵ� Ŭ���̾�Ʈ���� ��ȸ�Ѵ�.
	*/
	uint32					GetUsers(void);
	/** 
		������������ ��ȸ�Ѵ�.
	*/
	ServerApp::Properties*	GetProperty( void );
};