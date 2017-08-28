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

#include <conio.h>	// for _kbhit()
#include <CrashDump.h>
#include <Management.h>

/** 
	������ �⺻ ����� ó���ϴ� Ŭ����
*/
class ServerApp
	: public ICrashHandler
{
public:
	struct Properties
	{
		uint32				iSID;						///< �������̵�
		uint32				iTaskDuration;				///< �׽�ũ�����ٷ��� �׽�ũ ó�� ����
		bool				bLogArchive;				///< �α����� ��������
		std::string			sLogArchivePath;			///< �α����� ���� ���
		std::string			sLogBindAddr;				///< �α׼����� �α� �����ϱ� ���� �����Ǵ� UDPLink�� �ּҰ�
		bool				bLogTransmit;				///< �α׼����� �α� ���� ����
		std::string			sLogSAddr;					///< �α׼��� �ּ�
		uint32				iLogSPort;					///< �α׼��� ��Ʈ
		uint32				iCompressOverSize;			///< ��Ŷ ���� ����
		int32				iTaskSchedulersCount;		///< �׽�ũ�����ٷ� ����
		int32				iNotifyTaskSchedulerID;		///< ��Ʈ������ ���ŵ� ��Ŷ�� ó���� �׽�ũ�����ٷ�ID
		PAgentReceiver*		pPAgentHandler;				///< PAgent�� ������ ó���ϴ� �̺�Ʈ �ڵ鷯
		int32				iNetworkThreadsCount;		///< ��Ʈ�� �������� ����
		std::string			sPAgentAddr;				///< PAgent�� �ּ�
		int32				iPAgentPort;				///< PAgent�� ��Ʈ
		bool				bBlock;						///< Ŭ���̾�Ʈ ���� ���� ����
		uint32				iMaxClient;					///< �ִ� Ŭ���̾�Ʈ ��
		uint32				iUsers;						///< ���� ���ӵ� Ŭ���̾�Ʈ ��
		bool				bSyslog;					///< �ý��� �α� on/off

		Properties()
			: iSID(-1)
			, sPAgentAddr(null_string)
			, iPAgentPort(0)
			, iTaskDuration(100)
			, bLogArchive(true)
			, sLogArchivePath(".\\")
			, sLogBindAddr("localhost")
			, bLogTransmit(false)
			, sLogSAddr("localhost")
			, iLogSPort(4001)
			, iCompressOverSize(0)
			, iTaskSchedulersCount(1)
			, iNotifyTaskSchedulerID(1)
			, pPAgentHandler(nullptr)
			, iNetworkThreadsCount(1)
			, bBlock(false)
			, iMaxClient(2000)
			, iUsers(0)
			, bSyslog(false)
		{
		}
		Properties( const Properties& r )
			: iSID(r.iSID)
			, sPAgentAddr(r.sPAgentAddr)
			, iPAgentPort(r.iPAgentPort)
			, iTaskDuration(r.iTaskDuration)
			, bLogArchive(r.bLogArchive)
			, sLogArchivePath(r.sLogArchivePath)
			, sLogBindAddr(r.sLogBindAddr)
			, bLogTransmit(r.bLogTransmit)
			, sLogSAddr(r.sLogSAddr)
			, iLogSPort(r.iLogSPort)
			, iCompressOverSize(r.iCompressOverSize)
			, iTaskSchedulersCount(r.iTaskSchedulersCount)
			, iNotifyTaskSchedulerID(r.iNotifyTaskSchedulerID)
			, pPAgentHandler(r.pPAgentHandler)
			, iNetworkThreadsCount(r.iNetworkThreadsCount)
			, bBlock(r.bBlock)
			, iMaxClient(r.iMaxClient)
			, iUsers(r.iUsers)
			, bSyslog(r.bSyslog)
		{
		}
	};

protected:
	uint32				m_tCheckDuration;				///< ���� ���� üũ ����, 1��
	int32				m_argc;							///< �ƱԸ�Ʈ ����
	char**				m_argv;							///< �ƱԸ�Ʈ��
	int32				m_iState;						///< ��������
	std::string			m_sSetupFileName;				///< �����������ϸ�
	std::string			m_sSectionName;					///< �����������Ǹ�
	int32				m_iExitCode;					///< �����ڵ�
	PAgentConnector*	m_PAgent;						///< PAgent ���������
	Properties			m_AppProp;						///< ����������

public:
	ServerApp(int32 argc, char* argv[], bool bArgParse = true);
	virtual ~ServerApp();

private:
	ServerApp(const ServerApp& r) {}

public:
	/** 
		������ ũ�����ɶ� �������ϸ��� �����ϱ� ���ؼ� ȣ��Ǵ� �ݹ��Լ�
		@param lpszDumpPath ũ�������� ���ϸ�
	*/
	void OnCrash( IN LPTSTR lpszDumpPath ) override;
	/** 
		Ŀ�ǵ������ �Ľ��Ѵ�.
		@param argc �ƱԸ�Ʈ ����
		@param argv �ƱԸ�Ʈ��
	*/
	void ParseCommandLine( int32 argc, char* argv[] );
	/** 
		������ ���¸� üũ�ϴ� ������ �����Ѵ�.
		@param tDuration üũ����(msec)
	*/
	void CheckStateDuration( uint32 tDuration );
	/** 
		������ �������� �̸��� ��ȸ�Ѵ�.
	*/
	std::string		GetProgramName();
	/** 
		������ �������� �̸��� ��ȸ�Ѵ�.
	*/
	std::string		GetSetupFileName();
	/** 
		������ ���Ǹ��� ��ȸ�Ѵ�.
	*/
	std::string		GetSectionName();
	/** 
		PAgent�� ������ �����ߴ��� �����Ѵ�.
	*/
	bool			IsAgentMode();
	/** 
		PAgent�� �ּҸ� ��ȸ�Ѵ�.
	*/
	std::string		GetAgentAddr();
	/** 
		PAgent�� ��Ʈ�� ��ȸ�Ѵ�.
	*/
	int32			GetAgentPort();
	/** 
		�������̵� ��ȸ�Ѵ�.
	*/
	int32			GetSID();
	/** 
		�������̵� �����Ѵ�.
	*/
	void			SetSID(int32 iSID);
	/** 
		Ŭ���̾�Ʈ ���� ������ �����Ѵ�.
		@param bBlock �������� ����
	*/
	void			SetBlockClient( bool bBlock );
	/** 
		Ŭ���̾�Ʈ ���� ���� ���θ� ��ȸ�Ѵ�.
	*/
	bool			GetBlockClient( void );
	/** 
		�ִ� Ŭ���̾�Ʈ���� �����Ѵ�.
		@param iMaxClient �ִ�Ŭ���̾�Ʈ��
	*/
	void			SetMaxClient( uint32 iMaxClient );
	/** 
		�ִ� Ŭ���̾�Ʈ���� ��ȸ�Ѵ�.
	*/
	uint32			GetMaxClient( void );
	/** 
		���� ���ӵ� Ŭ���̾�Ʈ���� �����Ѵ�.
		@param iUsers ���� ���ӵ� Ŭ���̾�Ʈ��
	*/
	void			SetUsers( uint32 iUsers );
	/** 
		���� ���ӵ� Ŭ���̾�Ʈ���� ��ȸ�Ѵ�.
	*/
	uint32			GetUsers( void );
	/** 
		������ �����Ѵ�.
		@param iExitCode �����ڵ�
	*/
	void			QuitApplication(int32 iExitCode);
	/** 
		������ ����ȸ� ������ ���������� ������ ���ؼ� OnSetProperty �Լ��� ȣ��ȴ�.
		OnSetProperty �Լ������� ���������� �ʿ��� ������ ���� SetProperty �Լ��� �������� �����Ѵ�.
		@param Prop ����������
	*/
	void			SetProperty( ServerApp::Properties& Prop );
	/** 
		������������ ��ȸ�Ѵ�.
	*/
	Properties&		GetProperty( void );
	/** 
		�������������� �ε��Ѵ�.
		@param sFileName �����������ϸ�
		@param sSectionName �������Ǹ�
	*/
	bool			LoadSetupFile( const std::string& sFileName, const std::string& sSectionName );
	/** 
		������ �ʱ�ȭ�Ѵ�.
	*/
	bool			Initialize(void);
	/** 
		������ �̺�Ʈ ó���� �����Ѵ�.
	*/
	virtual void	RunEventLoop(void);
	/** 
		�������� �ڵ带 ��ȸ�Ѵ�.
	*/
	int32			ExitCode(void);

protected:
	virtual bool	OnInitialized(int32 argc, char* argv[]);
	virtual bool	OnSetProperty( ServerApp::Properties& appCfg );
	virtual void	OnQuitApplication();
	virtual void	DoCommand( const std::string& sCommand );
};

inline int32 RUN_MAIN(ServerApp* pHldr, const char* pConfigFile,const char* pSectionName, int argc, char* argv[])
{
	if ( !pHldr->LoadSetupFile(pConfigFile, pSectionName) || !pHldr->Initialize() )
	{
		return pHldr->ExitCode();
	}
	pHldr->RunEventLoop();
#if defined(_DEBUG)
	DWORD iProcessId = ::GetCurrentProcessId();
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, iProcessId);
	::TerminateProcess( hProcess, pHldr->ExitCode() );
#endif
	return pHldr->ExitCode();
}

#define RUN_SERVER_APP(app)	\
app Hldr(argc, argv); \
return RUN_MAIN( &Hldr, CONFIG_FILE, SECTION_NAME, argc, argv );