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
	���μ����� ����, �����ϰų� ���¸� �����ϴ� �����Լ��� �����ϴ� ���ӽ����̽�.
*/
namespace ProcessFactory
{
	/** 
		���μ��� ���¸� �����ϴ� �ֱ�� �½�ũ�����ٷ��� �����Ѵ�.
		@param iTaskSchedulerID �½�ũ�����ٷ� ID
		@param iDuration ���μ��� ���¸� �����ϴ� �ֱ�(ms)
	*/
	bool			Start( unsigned int iTaskSchedulerID, unsigned long iDuration );
	/**
		���μ����� �������� ���μ��� �����ʿ� ����Ѵ�.
		@param Opt ���μ��� ���� �ɼ�
		@param pHandler ���μ��� ���°� ��������� ȣ��� �̺�Ʈ �ڵ鷯
	*/
	ProcessPtr		Spawn( const ProcessOption& Opt, ProcessHandler* pHandler, interface_ref* pUserData = nullptr );
	/**
		���μ����� �����Ѵ�.
		@param Opt ���μ��� ���� �ɼ�
	*/
	ProcessPtr		Spawn( const ProcessOption& Opt );
	/** 
		���μ����� �������� ���μ��� �����ʿ��� �����Ѵ�.
		@param iProcessId ���μ��� ID
	*/
	bool			Terminate( DWORD iProcessId );
	/** 
		���μ����� ���´�.
		@param iProcessId ���μ��� ID
	*/
	ProcessPtr		GetProcess( DWORD iProcessId );
	/** 
		���μ����� ���¸� �����Ѵ�.
		@param iProcessId ���μ��� ID
		@param pHandler ���μ��� ���°� ��������� ȣ��� �̺�Ʈ �ڵ鷯
		@param pUserData ���μ��� ��ü�� ������ ����� ����Ÿ
	*/
	ProcessPtr		Monitor( DWORD iProcessId, ProcessHandler* pHandler, interface_ref* pUserData );
	/** 
		���μ����� �������� �̸��� ���´�.
	*/
	std::string		GetCurrentProcessName( void );
	/** 
		���μ����� ȯ�溯������ ������ Key ���ڿ��� ��ȸ�Ͽ� Key���ڿ��� ������ �� ���´�.(Key = Val)
		@param sKey ȯ�溯������ ��ȸ�� Key ���ڿ�
	*/
	std::string		GetEnvVariable( const std::string& sKey );
	/** 
		���μ����� ȯ�溯���� ������ Key�� ���� �����Ѵ�.( Key = Val )
		@param sKey ȯ�溯���� Key ���ڿ�
		@param sVal ȯ�溯���� ������ ��
	*/
	void			SetEnvVariable( const std::string& sKey, const std::string& sVal );
	/** 
		���μ����� ȯ�溯���� ���´�.
	*/
	std::string		GetEnvString( void );
	/** 
		���μ����� ȯ�溯���� �ֿܼ� ����Ѵ�.
	*/
	void			DumpEnvString( const std::string& sEnv );
	/** 
		���μ����� ȯ�溯���� �ֿܼ� ����Ѵ�.
	*/
	void			DumpEnvString( char* pEnv );
};