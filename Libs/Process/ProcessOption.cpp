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
#include <DataTypes.h>
#include <StringUtil.h>
#include <ProcessOption.h>
#include <ProcessFactory.h>

namespace
{
	std::string GetEscapedArgString( const std::string& a )
	{
		// command line처리에서 ""로 둘러 싸야할 escape 문자들.
		// cmd.exe 의 help page에 나오는 escape 처리에 대한 help를 참조했다.
		const std::string sEscapeChars(" \r\n\t&()<>@^|");
		bool bNeedQuoted = false;

		if( a.empty() )
		{
			bNeedQuoted = true;
		}
		else if( a.find_first_of(sEscapeChars) != std::string::npos )
		{
			bNeedQuoted = true;
		}

		std::string rst;

		if( bNeedQuoted )
		{
			rst += "\"";
		}
		rst += a;
		if( bNeedQuoted )
		{
			rst += "\"";
		}

		return rst;
	}
};

//<
ProcessOption::ProcessOption()
: m_iInheritHandle(0)
, m_iCreationFlag(0)
, m_bBackground(true)
, m_iMaxRestartCnt(0)
{
	m_sEnvironment = ProcessFactory::GetEnvString();
	//prn_sys( "env string size(%d)", m_sEnvironment.size() );
}

ProcessOption::ProcessOption( const ProcessOption& r )
: m_iInheritHandle(r.m_iInheritHandle)
, m_iCreationFlag(r.m_iCreationFlag)
, m_bBackground(r.m_bBackground)
, m_sCommandLine(r.m_sCommandLine)
, m_sDirectory(r.m_sDirectory)
, m_sEnvironment(r.m_sEnvironment)
, m_sExecPath(r.m_sExecPath)
, m_sExec(r.m_sExec)
, m_iMaxRestartCnt(r.m_iMaxRestartCnt)
{
}

ProcessOption::~ProcessOption()
{
}

char* ProcessOption::Path( void )
{
	return m_sExecPath.empty() ? nullptr : (char*)m_sExecPath.c_str();
}

void ProcessOption::Path( const std::string& sExecPath )
{
	m_sExecPath = util::trim( sExecPath );
}

char* ProcessOption::Exec( void )
{
	return m_sExec.empty() ? nullptr : (char*)m_sExec.c_str();
}

void ProcessOption::Exec( const std::string& sExec )
{
	m_sExec = util::trim( sExec );
}

void ProcessOption::CommandLine( const std::string& sCommandLine )
{
//	m_sCommandLine = GetEscapedArgString( util::trim(sCommandLine) );
	m_sCommandLine = util::trim(sCommandLine);
}

char* ProcessOption::CommandLine( void )
{
	return m_sCommandLine.empty() ? nullptr : (char*)m_sCommandLine.c_str();
}

void ProcessOption::WorkingDirectory( const std::string& sDirectory )
{
	m_sDirectory = util::trim( sDirectory );
}

char* ProcessOption::WorkingDirectory( void )
{
	return m_sDirectory.empty() ? nullptr : (char*)m_sDirectory.c_str();
}

void ProcessOption::Environment( const std::string& sEnv )
{
	m_sEnvironment += sEnv;
}

char* ProcessOption::Environment( void )
{
	m_sEnvironment += '\0';
	return (char*)m_sEnvironment.c_str();
}

void ProcessOption::CreationFlag( unsigned long iFlag )
{
	m_iCreationFlag = iFlag;
}

unsigned long ProcessOption::CreationFlag( void )
{
	return m_iCreationFlag;
}

void ProcessOption::InheritHandle( bool bInhrerit )
{
	m_iInheritHandle = bInhrerit ? 1 : 0;
}

bool ProcessOption::InheritHandle( void )
{
	return m_iInheritHandle ? true : false;
}

void ProcessOption::Background( bool bBackground )
{
	m_bBackground = bBackground;
}

bool ProcessOption::Background( void )
{
	return m_bBackground;
}

void ProcessOption::MaxRestartCnt( unsigned short iMaxRestartCnt )
{
	m_iMaxRestartCnt = iMaxRestartCnt;
}

unsigned short ProcessOption::MaxRestartCnt( void )
{
	return m_iMaxRestartCnt;
}
