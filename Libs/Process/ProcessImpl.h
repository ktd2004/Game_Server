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
#include <Ptr.h>
#include <ProcessFactory.h>

/** 
	���μ����� ���� ������ �����ϴ� ��ü Ŭ����.
*/
class ProcessImpl : public Process
{
public:
	DWORD					m_iExitCode;
	PROCESS_INFORMATION		m_ProcessInfo;
	interface_ref*			m_pUserData;
	ProcessHandler*			m_pHandler;
	ProcessOption			m_Opt;
	unsigned int			m_iRestartCnt;
	bool					m_bRestart;

public:
	ProcessImpl();
	ProcessImpl(const ProcessImpl& r) {}
	virtual ~ProcessImpl();
	/** 
		���μ����� �����Ѵ�.
		@param opt ���μ��� ���� �ɼ�
		@verbatim
		ProcessOption	opt;
		opt.Path( "C:\\WINDOWS\\system32" );
		opt.Exec( "notepad.exe" );
		opt.CommandLine( "" );
		opt.WorkingDirectory( ".\\" );
		opt.CreationFlag( CREATE_NEW_CONSOLE );
		opt.Environment( "this=is" );
		opt.MaxRestartCnt( 3 );

		ProcessFactory::Spawn( opt, nullptr );
		@endverbatim
	*/
	bool				Spawn( const ProcessOption& opt, ProcessHandler* pHandler ) override;
	/** 
		���μ����� ���� �������� ��ȸ�Ѵ�.
	*/
	bool				IsRunning( void ) override;
	/** 
		���μ����� �����ϱ⸦ ����Ѵ�.
		@param iTm ���μ����� ���Ḧ ����ϴ� �ð�(ms)
	*/
	int					Wait( unsigned long iTm ) override;
	/** 
		���μ����� �����Ѵ�.
	*/
	bool				Terminate( void ) override;
	/** 
		���μ����� �ڵ��� ��ȸ�Ѵ�.
	*/
	HANDLE				GetHandle( void ) override;
	/** 
		���μ����� �ڵ��� �����Ѵ�.
	*/
	void				SetHandle( HANDLE hProcess );
	/** 
		���μ����� ID�� ��ȸ�Ѵ�.
	*/
	unsigned long		GetProcessId( void ) override;
	/** 
		���μ����� ID�� �����Ѵ�.
	*/
	void				SetProcessId( unsigned long iProcessId );
	/** 
		���μ����� �����ڵ带 ���´�.
	*/
	DWORD				ExitCode( void ) override;
	/** 
		���μ����� ����� ��ü�� �����Ѵ�.
		@param pUserData ���μ����� ������ ����� ��ü
	*/
	void				UserData(interface_ref* pUserData) override;
	/** 
		���μ����� ����� ��ü�� ��ȸ�Ѵ�.
	*/
	interface_ref*		UserData(void) override;
	/** 
		���μ����� ����� ���θ� ��ȸ�Ѵ�.
	*/
	bool				IsRestart( void ) override;
	/** 
		���μ����� ����� ���θ� �����Ѵ�.
	*/
	void				SetRestart( bool bRestart ) override;
	/** 
		���μ����� ����� Ƚ���� ��ȸ�Ѵ�.
	*/
	unsigned int		GetRestartCount( void ) override;
};
