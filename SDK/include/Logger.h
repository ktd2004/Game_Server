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
	로그레벨
*/
enum eLogLevel
{
	eLOG_INFO,		///< 정보
	eLOG_ERROR,		///< 에러
	eLOG_DEBUG,		///< 디버그
	eLOG_SEND,		///< 로그서버로 전송
	eLOG_SYS,		///< 시스템
};

/** 
	로그출력 방향
*/
enum eLogRedirect
{
	eLOG_BIT_STDOUT	= 2,		///< 표준출력
	eLOG_BIT_FILE	= 4,		///< 파일출력
	eLOG_BIT_SYSTEM = 8,		///< 시스템로그출력
};

/** 
	로그출력시 호출되는 콜백를 정의한 추상 클래스

	아래 예제는 prn_snd 호출시에 로그서버로 로그데이타를 전송하는 방법을 보여준다.
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
		로그출렧시에 호출되는 콜백함수
		@param iLevel 로그 레벨
		@param pFrmt 로그 포멧
		@param argp 로그데이타
	*/
	virtual void OnWrite( uint32 iLevel, const char* pFrmt, va_list argp ) = 0;
};





/** 
	로그를 표준출력이나 파일로 저장하는 클래스
*/
class Logger
{
private:
	FILE*				m_File;						///< 로그파일포인터
	char				m_szOutputPath[MAX_PATH];	///< 로그파일경로
	char				m_szFileName[MAX_PATH];		///< 로그파일명
	char				m_szProgName[MAX_PATH];		///< 프로그램명
	long				m_iMaxSize;					///< 로그파일최대크기
	volatile long		m_iCurrSize;				///< 현재의로그파일크기
	bool				m_bWrite;					///< 로그를 파일에 기록할지 여부
	LogWriteCallback*	m_pCallback;				///< 로그출력 콜백
	uint32				m_iBit;						///< 로그출력 방향
	time_t				m_tLastTm;					///< 파일생성일자
	Sync				m_Mutex;					///< 동기화
	Signal				m_Evt;						///< 이벤트
	Thread*				m_pThread;					///< 로그를 파일에 기록하는 쓰레드
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
		로그출력 콜백을 설정한다.
		@param pCallback 로그출력 콜백
	*/
	void				SetCallback( LogWriteCallback *pCallback );
	/** 
		로그출력 방향을 설정한다.
		@param iBit 로그출력 방향
	*/
	void				SetRedirect( uint32 iBit );
	/** 
		로그출력 방향을 지운다.
		@param iBit 삭제할 로그출력 방향
	*/
	void				ClrRedirect( uint32 iBit );
	/** 
		로그파일경로를 설정한다.
		@param sOutPath 로그파일 경로
		@param iSize 최대파일크기
	*/
	void				SetFile( const std::string& sOutPath, uint32 iSize = 0, uint32 iMaxLogBufs = 500 );
	/** 
		로그데이타를 파일에 기록한다.
		@param iLevel 로그 레벨
		@param szFrmt 로그 포멧
		@param ... 로그데이타
	*/
	void				Write( uint32 iLevel, const char* szFmt, ... );
	/** 
		로그 쓰레드 종료
	*/
	void				Stop( void );
private:
	void				Running( void );
};

/** 
	@def 로그 콜백 함수 설정
*/
#define prn_callback(cbk)				Logger::instance()->SetCallback(cbk)
/** 
	@def 표준 출력 금지
*/
#define prn_console_off()				Logger::instance()->ClrRedirect(eLOG_BIT_STDOUT)
/** 
	@def 표준 출력 허용
*/
#define prn_console_on()				Logger::instance()->SetRedirect(eLOG_BIT_STDOUT)
/** 
	@def 로그 파일 출력 금지
*/
#define prn_file_off()					Logger::instance()->ClrRedirect(eLOG_BIT_FILE)
/** 
	@def 로그 파일 출력 허용
*/
#define prn_file_on()					Logger::instance()->SetRedirect(eLOG_BIT_FILE)
/**
	@def 시스템 로그 출력 허용
*/
#define prn_sys_on()					Logger::instance()->SetRedirect(eLOG_BIT_SYSTEM)
/**
	@def 시스템 로그 출력 금지
*/
#define prn_sys_off()					Logger::instance()->ClrRedirect(eLOG_BIT_SYSTEM)
/** 
	@def 로그파일 출력 경로
*/
#define prn_path(path,size)				Logger::instance()->SetFile(path,size)
/** 
	@def 로그파일 출력을 종료시킨다.
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