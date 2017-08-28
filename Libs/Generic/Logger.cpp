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
#include "stdafx.h"
#include <MacroFunc.h>
#include <StringUtil.h>
#include <FileUtil.h>
#include <TimeUtil.h>
#include <Logger.h>

#define INF_TAG		"INF"
#define ERR_TAG		"ERR"
#define DBG_TAG		"DBG"
#define SND_TAG		"SND"
#define SYS_TAG		"SYS"

namespace
{
	static char *pszLevel[] = {
		INF_TAG,ERR_TAG, DBG_TAG, SND_TAG, SYS_TAG, nullptr
	};

	const uint32 MIN_LOG_SIZE = 1024 * 512;	// 512KB
	const uint32 MAX_LOG_SIZE = 4294967295;	// 4GB
}

#undef INF_TAG
#undef ERR_TAG
#undef DBG_TAG
#undef SND_TAG
#undef SYS_TAG

Logger* Logger::instance()
{
	static Logger inst;
	return &inst;
}

Logger::Logger()
	: m_File(0)
	, m_pCallback(nullptr)
	, m_iMaxSize(0)
	, m_iCurrSize(0)
	, m_bWrite(false)
	, m_iBit(eLOG_BIT_STDOUT)
	, m_tLastTm(0)
	, m_pThread(nullptr)
	, m_iLogBufs(0)
	, m_iMaxLogBufs(1000)
	, m_bLoop(true)
{
	m_szOutputPath[0] = 0;
	m_szFileName[0] = 0;
}

Logger::~Logger()
{
	if (m_File)
	{
		::fclose(m_File);
	}
}


void Logger::SetCallback( LogWriteCallback *pCallback )
{
	m_pCallback = pCallback;
}

void Logger::SetRedirect( uint32 iBit )
{
	set_bit(m_iBit,iBit);
}

void Logger::ClrRedirect( uint32 iBit )
{
	clr_bit(m_iBit,iBit);
}

void Logger::WriteFile( uint32 iLevel, const char* szFmt, va_list arg )
{
	LogData* pData = nullptr;
	Guard lock(m_Mutex);
	if ( m_LogPool.empty() )
	{
		pData = new LogData;
		m_iLogBufs++;
	}
	else
	{
		pData = m_LogPool.front(); m_LogPool.pop_front();
	}

	if ( !m_pThread )
	{
		m_pThread = CREATE_THREAD_V0( this, &Logger::Running, 0 );
		m_pThread->Resume();
	}
	vsprintf_s(pData->pText, sizeof(pData->pText), szFmt, arg);
	m_LogQ.push_back( pData );
	m_Evt.Wakeup();
}

void Logger::Write( uint32 iLevel, const char* szFmt, ... )
{
	char* p = pszLevel[0];

#define case_tag(x, y) case x : p = pszLevel[y]; break;
	switch ( iLevel )
	{
		case_tag(eLOG_INFO,    0);
		case_tag(eLOG_ERROR,   1);
		case_tag(eLOG_DEBUG,   2);
		case_tag(eLOG_SEND,    3);
		case_tag(eLOG_SYS,     4);
	}
#undef case_tag

	if (iLevel == eLOG_SYS && !is_bit_on(m_iBit,eLOG_BIT_SYSTEM))
	{
		return;
	}

	const std::string sTm = util::to_char( ::time(NULL), "HH:MI:SS");

	va_list arg;
	va_start(arg, szFmt);

	char szBufStd[2048];
	int iLen = sprintf_s( szBufStd, sizeof(szBufStd), "%s-%s: %s\n", p, sTm.c_str(), szFmt );

	if ( is_bit_on(m_iBit,eLOG_BIT_STDOUT))
	{
		// eLOG_SEND는 콘솔에 출력하지 않도록 함
		if( iLevel != eLOG_SEND )
		{
			vprintf( szBufStd, arg );
		}
	}

	if ( is_bit_on(m_iBit,eLOG_BIT_FILE) )
	{
		WriteFile( iLevel, szBufStd, arg );
	}

	if ( m_pCallback )
	{
		LogWriteCallback* pCur = m_pCallback;
		while (pCur)
		{
			szBufStd[iLen-1] = 0; // 개행문자 제거
			pCur->OnWrite( iLevel, szBufStd, arg );
			pCur = pCur->m_pNext;
		}
	}

	va_end(arg);
}

void Logger::SetFile( const std::string& sOutPath, uint32 iSize, uint32 iMaxLogBufs )
{
	Guard lock(m_Mutex);
	char szExe[MAX_PATH];
	GetModuleFileName(nullptr, szExe, sizeof(szExe));
	std::string s = basename(szExe);
	strcpy( m_szProgName, cstr(s) );
	m_iMaxSize = iSize;
	m_iMaxLogBufs = iMaxLogBufs;
	strcpy( m_szOutputPath, cstr(sOutPath) );

	int32 iRet = util::isfolder( m_szOutputPath );
	if( iRet != util::eFolder )
	{
		util::makedir( m_szOutputPath );
	}

	if( m_iMaxSize < MIN_LOG_SIZE )
	{
		m_iMaxSize = MIN_LOG_SIZE;
	}

	if( m_iMaxLogBufs > 1000 )
	{
		m_iMaxLogBufs = 1000;
	}
}

void Logger::Running( void )
{
	while (m_bLoop)
	{
		if ( m_Evt.Wait(1000) == false )
		{
			continue;
		}

		// 성능 향상을 위해서 lock 최소화
		LogQ	Que;
		{
			Guard lock(m_Mutex);
			Que.merge( m_LogQ );
		}

		if ( Que.size() == 0 )
		{
			continue;
		}

		// 하루에 파일 1개씩 생성되게 
		time_t tCurr = util::maketime(0, "YYYYMMDD");
		if ( m_File && access(m_szFileName, 0) != 0 )
		{
			::fclose(m_File);
			m_iCurrSize = 0;
			m_File = nullptr;
		}

		if ( m_File == nullptr || m_iCurrSize > m_iMaxSize || tCurr !=  m_tLastTm )
		{
			sprintf_s( m_szFileName, sizeof(m_szFileName), "%s\\%s_D%s_%d.log", 
				m_szOutputPath,
				m_szProgName, 
				util::to_char(::time(NULL), "YYMMDD_HHMISS").c_str(), 
				::GetCurrentProcessId() );

			if ( m_File )
			{ 
				::fclose(m_File); m_File = nullptr;
			}

			if ( nullptr == (m_File = fopen(m_szFileName, "w+")) )
			{ return; }

			m_iCurrSize = 0; 
			m_tLastTm = tCurr;
		}

		while( !Que.empty() )
		{
			LogData* pData = Que.front(); Que.pop_front();
			m_iCurrSize += fprintf(m_File, "%s",pData->pText );
		}
		fflush(m_File);

		// 로그버퍼 풀 정리
		if ( m_iLogBufs > m_iMaxLogBufs )
		{
			Guard lock(m_Mutex);
			m_LogPool.merge( Que );
			while ( !m_LogPool.empty() && m_iLogBufs > m_iMaxLogBufs*0.1 )
			{
				LogData* pData = m_LogPool.front(); m_LogPool.pop_front();
				delete pData;
				m_iLogBufs--;
			}
		}
	}

	printf( "log thread exit\n" );
}

void Logger::Stop( void )
{
	if ( m_pThread )
	{
		locked_exchange(m_bLoop, false);
		HANDLE hThread = m_pThread->GetHandle();
		while ( ::WaitForSingleObject(hThread, 500) == WAIT_TIMEOUT )
		{
			m_Evt.Wakeup();
			m_pThread->Terminate();
		}
	}
}
