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

#include <stdarg.h>
#include <DataTypes.h>
#include <Sync.h>
#include <Signal.h>
#include <ThreadFactory.h>
#include <FastList.h>

/** 
	�α׷���
*/
enum eLogLevel
{
	eLOG_INFO,		///< ����
	eLOG_ERROR,		///< ����
	eLOG_DEBUG,		///< �����
	eLOG_SEND,		///< �α׼����� ����
	eLOG_SYS,		///< �ý���
};

/** 
	�α���� ����
*/
enum eLogRedirect
{
	eLOG_BIT_STDOUT	= 2,		///< ǥ�����
	eLOG_BIT_FILE	= 4,		///< �������
	eLOG_BIT_SYSTEM = 8,		///< �ý��۷α����
};

/** 
	�α���½� ȣ��Ǵ� �ݹ鸦 ������ �߻� Ŭ����

	�Ʒ� ������ prn_snd ȣ��ÿ� �α׼����� �α׵���Ÿ�� �����ϴ� ����� �����ش�.
	@verbatim
	class PAgentConnector : public LogWriteCallback
	{
		void OnWrite( uint32 iLevel, const char* pFrmt, va_list argp )
		{
			if ( iLevel == eLOG_SEND && m_Logger )
			{
				char szBuf[2048];
				int32 iLen = vsprintf_s( szBuf, sizeof(szBuf), pFrmt, argp );
				Buffer* pBuf = get_OnLogTransmit( iLevel, szBuf );
				m_Logger->Send( pBuf, &m_LoggerAddr );
			}
		}
	};

	PAgentConnector inst;
	prn_callback( &inst );

	prn_snd( "this is test data" );

	@endverbatim
*/
struct LogWriteCallback
{
	LogWriteCallback* m_pNext;
	LogWriteCallback() : m_pNext(nullptr) {}
	/** 
		�α��⎶�ÿ� ȣ��Ǵ� �ݹ��Լ�
		@param iLevel �α� ����
		@param pFrmt �α� ����
		@param argp �α׵���Ÿ
	*/
	virtual void OnWrite( uint32 iLevel, const char* pFrmt, va_list argp ) = 0;
};





/** 
	�α׸� ǥ������̳� ���Ϸ� �����ϴ� Ŭ����
*/
class Logger
{
private:
	FILE*				m_File;						///< �α�����������
	char				m_szOutputPath[MAX_PATH];	///< �α����ϰ��
	char				m_szFileName[MAX_PATH];		///< �α����ϸ�
	char				m_szProgName[MAX_PATH];		///< ���α׷���
	long				m_iMaxSize;					///< �α������ִ�ũ��
	volatile long		m_iCurrSize;				///< �����Ƿα�����ũ��
	bool				m_bWrite;					///< �α׸� ���Ͽ� ������� ����
	LogWriteCallback*	m_pCallback;				///< �α���� �ݹ�
	uint32				m_iBit;						///< �α���� ����
	time_t				m_tLastTm;					///< ���ϻ�������
	Sync				m_Mutex;					///< ����ȭ
	Signal				m_Evt;						///< �̺�Ʈ
	Thread*				m_pThread;					///< �α׸� ���Ͽ� ����ϴ� ������
	struct LogData : public listnode(LogData)
	{
		char	pText[2048];
	};
	typedef util::list<LogData>	LogQ;
	LogQ				m_LogQ;
	LogQ				m_LogPool;
	uint32				m_iLogBufs;
	uint32				m_iMaxLogBufs;
	volatile bool		m_bLoop;

private:
	Logger();
	Logger( const Logger& r ) {}
	virtual ~Logger();
	void				WriteFile(uint32 iLevel, const char* szFmt, va_list arg);

public:
	static Logger*		instance();
	/** 
		�α���� �ݹ��� �����Ѵ�.
		@param pCallback �α���� �ݹ�
	*/
	void				SetCallback( LogWriteCallback *pCallback );
	/** 
		�α���� ������ �����Ѵ�.
		@param iBit �α���� ����
	*/
	void				SetRedirect( uint32 iBit );
	/** 
		�α���� ������ �����.
		@param iBit ������ �α���� ����
	*/
	void				ClrRedirect( uint32 iBit );
	/** 
		�α����ϰ�θ� �����Ѵ�.
		@param sOutPath �α����� ���
		@param iSize �ִ�����ũ��
	*/
	void				SetFile( const std::string& sOutPath, uint32 iSize = 0, uint32 iMaxLogBufs = 500 );
	/** 
		�α׵���Ÿ�� ���Ͽ� ����Ѵ�.
		@param iLevel �α� ����
		@param szFrmt �α� ����
		@param ... �α׵���Ÿ
	*/
	void				Write( uint32 iLevel, const char* szFmt, ... );
	/** 
		�α� ������ ����
	*/
	void				Stop( void );
private:
	void				Running( void );
};

/** 
	@def �α� �ݹ� �Լ� ����
*/
#define prn_callback(cbk)				Logger::instance()->SetCallback(cbk)
/** 
	@def ǥ�� ��� ����
*/
#define prn_console_off()				Logger::instance()->ClrRedirect(eLOG_BIT_STDOUT)
/** 
	@def ǥ�� ��� ���
*/
#define prn_console_on()				Logger::instance()->SetRedirect(eLOG_BIT_STDOUT)
/** 
	@def �α� ���� ��� ����
*/
#define prn_file_off()					Logger::instance()->ClrRedirect(eLOG_BIT_FILE)
/** 
	@def �α� ���� ��� ���
*/
#define prn_file_on()					Logger::instance()->SetRedirect(eLOG_BIT_FILE)
/**
	@def �ý��� �α� ��� ���
*/
#define prn_sys_on()					Logger::instance()->SetRedirect(eLOG_BIT_SYSTEM)
/**
	@def �ý��� �α� ��� ����
*/
#define prn_sys_off()					Logger::instance()->ClrRedirect(eLOG_BIT_SYSTEM)
/** 
	@def �α����� ��� ���
*/
#define prn_path(path,size)				Logger::instance()->SetFile(path,size)
/** 
	@def �α����� ����� �����Ų��.
*/
#define prn_stop()						Logger::instance()->Stop()

#if defined(_DEBUG)

#define prn_inf( _fmt, ... )			Logger::instance()->Write( eLOG_INFO,  _fmt, __VA_ARGS__ )
#define prn_dbg( _fmt, ... )			Logger::instance()->Write( eLOG_DEBUG, "%s@ "_fmt, __FUNCTION__, __VA_ARGS__ )
#define prn_err( _fmt, ... )			Logger::instance()->Write( eLOG_ERROR, "%s@ "_fmt, __FUNCTION__, __VA_ARGS__ )
#define prn_snd( _fmt, ... )			Logger::instance()->Write( eLOG_SEND,  _fmt, __VA_ARGS__ )
#define prn_sys( _fmt, ... )			Logger::instance()->Write( eLOG_SYS, "%s@ "_fmt, __FUNCTION__, __VA_ARGS__ )

#else

#define prn_inf( _fmt, ... )			Logger::instance()->Write( eLOG_INFO,  _fmt, __VA_ARGS__ )
#define prn_dbg( _fmt, ... )
#define prn_err( _fmt, ... )			Logger::instance()->Write( eLOG_ERROR, "%s@ "_fmt, __FUNCTION__, __VA_ARGS__ )
#define prn_snd( _fmt, ... )			Logger::instance()->Write( eLOG_SEND,  _fmt, __VA_ARGS__ )
#define prn_sys( _fmt, ... )			Logger::instance()->Write( eLOG_SYS, "%s@ "_fmt, __FUNCTION__, __VA_ARGS__ )

#endif

#define prn_sbj(s)						prn_inf( "%s", util::center(s, 50, '=').c_str())
#define prn_bar()						prn_inf( "%s", cstr(std::string(50, '=')) )