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
//<
#include <string>
#include <Ptr.h>

/** 
	���μ����� ���, ������ �Ű�����, �ܼ��� ���� ����, ��׶��� ���� ����, ����� Ƚ������ ���μ��� ���� �ɼ��� �����Ѵ�.

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
class ProcessOption
{
public:
	std::string			m_sExecPath;		///< ���μ����� ���
	std::string			m_sExec;			///< ���μ����� �̸�
	std::string			m_sCommandLine;		///< ���μ����� ������ �Ű�����
	std::string			m_sDirectory;		///< ���μ����� ���� ���丮
	std::string			m_sEnvironment;		///< ���μ����� ȯ�溯��
	unsigned long		m_iCreationFlag;	///< ���μ��� ���� �ɼ�
	int					m_iInheritHandle;	///< �ڵ� ��� ����
	bool				m_bBackground;		///< ��׶���� ���� ����
	unsigned short		m_iMaxRestartCnt;	///< ����� Ƚ��

public:
	ProcessOption();
	ProcessOption( const ProcessOption& r );
	virtual ~ProcessOption();
	/** 
		���μ����� ��θ� �����Ѵ�.
		@param sExecPath ���μ����� ���
	*/
	void			Path( const std::string& sExecPath );
	/** 
		���μ����� ��θ� ��ȸ�Ѵ�.
	*/
	char*			Path( void );
	/** 
		���μ����� �̸��� �����Ѵ�.
		@param sExecPath ���μ����� ���
	*/
	void			Exec( const std::string& sExec );
	/** 
		���μ����� ��θ� ��ȸ�Ѵ�.
	*/
	char*			Exec( void );
	/** 
		���μ����� ������ �ƱԸ�Ʈ�� �����Ѵ�.
		@param sCommandLine ���μ����� ������ �ƱԸ�Ʈ
	*/
	void			CommandLine( const std::string& sCommandLine );
	/** 
		���μ����� ������ �ƱԸ�Ʈ�� ��ȸ�Ѵ�.
	*/
	char*			CommandLine( void );
	/** 
		���μ����� ������ ���丮�� �����Ѵ�.
		@param sDirectory ���μ����� ���� ���丮
	*/
	void			WorkingDirectory( const std::string& sDirectory );
	/** 
		���μ����� ������ ���丮�� ��ȸ�Ѵ�.
	*/
	char*			WorkingDirectory( void );
	/** 
		���μ����� ȯ�溯���� �����Ѵ�.
		@param sEnv ���μ����� ȯ�溯��
	*/
	void			Environment( const std::string& sEnv );
	/** 
		���μ����� ȯ�溯���� ��ȸ�Ѵ�.
	*/
	char*			Environment( void );
	/** 
		���μ����� ���� �ɼ��� �����Ѵ�.
		@param iFlag ���μ����� �����ɼ�
		���� ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.ko/dllproc/base/process_creation_flags.htm
	*/
	void			CreationFlag( unsigned long iFlag );
	/** 
		���μ����� ���� �ɼ��� ��ȸ�Ѵ�.
	*/
	unsigned long	CreationFlag( void );
	/** 
		���μ��� �ڵ��� ��� ���θ� �����Ѵ�.
		@param bInhrerit ���μ��� �ڵ��� ��� ����
	*/
	void			InheritHandle( bool bInhrerit );
	/** 
		���μ��� �ڵ��� ��� ���θ� ��ȸ�Ѵ�.
	*/
	bool			InheritHandle( void );
	/** 
		���μ����� ��׶���� �������� ���θ� �����Ѵ�.
		@param bBackground ���μ����� ��׶���� �������� ����
	*/
	void			Background( bool bBackground );
	/** 
		���μ����� ��׶���� �������� ���θ� ��ȸ�Ѵ�.
	*/
	bool			Background( void );
	/** 
		���μ��� ������ ����� ����� Ƚ���� �����Ѵ�.
		@param iMaxRestartCnt ���μ��� ����� Ƚ��
	*/
	void			MaxRestartCnt( unsigned short iMaxRestartCnt );
	/** 
		���μ��� ������ ����� ����� Ƚ���� ��ȸ�Ѵ�.
	*/
	unsigned short	MaxRestartCnt( void );
};

struct ProcessHandler;
/** 
	���μ��� ��ü�� ����Ʈ ������ Ÿ�� ����.
*/
PTR(Process);
/** 
	���μ����� ���� ������ �����ϴ� ��ü Ŭ����.
*/
class Process : public single_ref
{
public:
	Process() {}
	virtual ~Process() {}

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
	virtual bool			Spawn( const ProcessOption& opt, ProcessHandler* pHandler ) = 0;
	/** 
		���μ����� ���� �������� ��ȸ�Ѵ�.
	*/
	virtual bool			IsRunning( void ) = 0;
	/** 
		���μ����� �����ϱ⸦ ����Ѵ�.
		@param iTm ���μ����� ���Ḧ ����ϴ� �ð�(ms)
	*/
	virtual int				Wait( unsigned long iTm ) = 0;
	/** 
		���μ����� �����Ѵ�.
	*/
	virtual bool			Terminate( void ) = 0;
	/** 
		���μ����� �ڵ��� ��ȸ�Ѵ�.
	*/
	virtual HANDLE			GetHandle( void ) = 0;
	/** 
		���μ����� ID�� ��ȸ�Ѵ�.
	*/
	virtual unsigned long	GetProcessId( void ) = 0;
	/** 
		���μ����� �����ڵ带 ��ȸ�Ѵ�.
	*/
	virtual DWORD			ExitCode( void ) = 0;
	/** 
		���μ����� ����� ��ü�� �����Ѵ�.
		@param pUserData ���μ����� ������ ����� ��ü
	*/
	virtual void			UserData( interface_ref* pUserData ) = 0;
	/** 
		���μ����� ����� ��ü�� ��ȸ�Ѵ�.
	*/
	virtual interface_ref*	UserData(void) = 0;
	/** 
		���μ����� ����� ���θ� ��ȸ�Ѵ�.
	*/
	virtual bool			IsRestart( void ) = 0;
	/** 
		���μ����� ����� ���θ� �����Ѵ�.
	*/
	virtual void			SetRestart( bool bRestart ) = 0;
	/** 
		���μ����� ����� Ƚ���� ��ȸ�Ѵ�.
	*/
	virtual unsigned int	GetRestartCount( void ) = 0;
};