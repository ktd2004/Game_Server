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
#include <Logger.h>
#include <StringUtil.h>
#include <Errorcodes.h>
#include <INILoader.h>
#include "INISequence.h"

INISequence::INISequence()
{

}

INISequence::~INISequence()
{

}

bool INISequence::Load( const std::string& sFileName, bool bDebug )
{
	INILoader ini;
	if ( !ini.Load(sFileName) )
	{
		return false;
	}

	FILE* fp = fopen(cstr(sFileName), "r");
	if ( !fp )
	{
		prn_err( "fopen error(%d)", GetLastError() );
		return false;
	}

	std::string sSection;
	std::string sComment;
	std::string sSID;
	int32 iPos = 1, iComment = 1;
	while ( !::feof(fp) )
	{
		char szBuf[1024 * 4]; memset( szBuf, 0x00, sizeof(szBuf) );
		fgets( szBuf, sizeof(szBuf), fp );
		std::string s = util::trim(szBuf);
		if ( s.empty() )
		{
			continue;
		}
		else if ( s[0] == '#' || s[1] == '!')
		{
			iComment = iPos;
			sComment = s;
			continue;
		}
		else if ( s[0] == '[' && s[s.size()-1] == ']' )
		{	
			sSection = s.substr(1, s.size()-2);
			sSection = util::trim(sSection);
			sSID = ini.GetString( sSection, "id", "" );
			continue;
		}

		std::string sKey = util::pairkey( s );
		std::string sValue = util::pairval( s );
		if ( iPos != iComment )
		{
			sComment = "";
		}
		
		m_INIMap.insert( INIMap::value_type(iPos, Record(sSID,sSection,sKey,sValue,sComment)) );
		if ( bDebug )
		{
			prn_inf( "[%02d] [%s] [%s] %s=%s %s", 
				iPos,
				cstr(sSID),
				cstr(sSection),
				cstr(sKey),
				cstr(sValue),
				sComment.size() ? cstr(sComment) : "null" );
		}
		iPos++;
		sComment = "";
	}

	fclose(fp);

	return true;
}
