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
#include <Logger.h>
#include <INILoader.h>

INILoader::INILoader(void)
: m_bLoaded(false)
{
}

INILoader::~INILoader(void)
{
}

INILoader* INILoader::instance()
{
	static INILoader sInstance;
	return &sInstance;
}

bool INILoader::Load( const std::string& sPath )
{
	if ( util::getfilename(sPath) == util::getworkpath(sPath) )
	{
		m_sPath = ".\\" + sPath;
	}
	else
	{
		m_sPath = sPath;
	}
	
	char	szBuf[1024 * 4];
	char	szSection[1024];
	DWORD   iBytes = ::GetPrivateProfileSectionNames(szBuf, 1024, cstr(m_sPath));
	int     iPos = 0;  

	if ( iBytes == 0 )
	{
		return false;
	}

	m_Sections.clear();

	// 섹션 이름 로딩
	for ( int32 i = 0; i < (int32)iBytes; i++ )
	{   
		if(szBuf[i] != '\0')
		{
			szSection[iPos++] = szBuf[i];
			continue;   
		}   
		else
		{
			szSection[iPos] = '\0';
			m_Sections.insert( SectionMap::value_type(szSection, KeyMap()));
			memset(szSection, 0x00, sizeof(szSection));
			iPos = 0;
		}
	}

	// 섹션 로딩
	foreach_do( SectionMap, itSec, m_Sections )
	{
//		printf( "[%s]\n", cstr(itSec->first) );

		iBytes = ::GetPrivateProfileSection( cstr(itSec->first), szBuf, sizeof(szBuf), cstr(m_sPath));
		if ( iBytes == 0 )
		{
			continue;
		}

		iPos = 0;
		for ( int32 i = 0; i < (int32)iBytes; i++ )
		{   
			if(szBuf[i] != '\0')
			{
				szSection[iPos++] = szBuf[i];
				continue;   
			}   
			else
			{
				szSection[iPos] = '\0';
				std::string sValue = util::trim(szSection);
				if ( sValue[0] != '#')
				{
					itSec->second.insert( KeyMap::value_type(util::pairkey(sValue),util::pairval(sValue)));
				}
				iPos = 0;
			}
		}
	}

	m_bLoaded = true;

	return true;
}

bool INILoader::IsLoaded( void )
{
	return m_bLoaded;
}

INILoader::KeyMap& INILoader::GetKeyMap( const std::string& sSection )
{
	return m_Sections[sSection];
}

INILoader::SectionMap& INILoader::GetSectionMap( void )
{
	return m_Sections;
}

const std::string& INILoader::GetString( const std::string& Section, const std::string& Key, const std::string& Default )
{
	SectionMap::iterator itSec = m_Sections.find( Section );
	if ( itSec != m_Sections.end() )
	{
		KeyMap::iterator itVal = itSec->second.find( Key );
		if ( itVal != itSec->second.end() )
		{
			return itVal->second;
		}
	}

	return Default;
}

int32 INILoader::GetInt( const std::string& Section, const std::string& Key, int32 Default )
{
	const std::string& sVal = GetString( Section, Key, null_string );
	if ( sVal.empty() )
	{
		return Default;
	}

	return stoint32(sVal);
}


uint64 INILoader::GetUint64( const std::string& Section, const std::string& Key, uint64 Default )
{
	std::string sVal = GetString( Section, Key, null_string );
	if ( sVal.empty() )
	{
		return Default;
	}
	return stouint64(sVal);
}

double INILoader::GetDouble( const std::string& Section, const std::string& Key, const double Default )
{
	std::string sVal = GetString( Section, Key, null_string );
	if ( sVal.empty() )
	{
		return Default;
	}
	return stodouble(sVal);
}

bool INILoader::GetBool( const std::string& Section, const std::string& Key, const bool Default )
{
	std::string sVal = GetString( Section, Key, null_string );
	if ( sVal.empty() )
	{
		return Default;
	}

	if ( util::upper(sVal) == "TRUE" )
	{
		return true;
	}
	
	return false;
}

bool INILoader::SetString( const std::string& Section, const std::string& Key, const std::string& Value )
{
	SectionMap::iterator itSec = m_Sections.find( Section );
	if ( itSec != m_Sections.end() )
	{
		KeyMap::iterator itVal = itSec->second.find( Key );
		if ( itVal != itSec->second.end() )
		{
			itVal->second = Value;
		}
		else
		{
			itSec->second[Key] = Value;
		}
	}
	else
	{
		m_Sections.insert( SectionMap::value_type(Section, KeyMap()) );
		m_Sections[Section].insert(KeyMap::value_type(Key,Value));
	}

	return true;
}

bool INILoader::SetInt( const std::string& Section, const std::string& Key, const int32 iValue )
{
	char szBuf[50]; _snprintf_s( szBuf, sizeof(szBuf), "%d", iValue );
	return SetString( Section, Key, szBuf );
}

bool INILoader::SetDouble( const std::string& Section, const std::string& Key, const double dValue )
{
	char szBuf[50]; _snprintf_s( szBuf, sizeof(szBuf), "%f", dValue );
	return SetString( Section, Key, szBuf );
}


bool INILoader::SetBool( const std::string& Section, const std::string& Key, const bool bValue )
{
 	char* szBuf[2] = { "FALSE", "TRUE" };
 	return SetString( Section, Key, szBuf[bValue] );
}

bool INILoader::Save( const std::string& sFileName )
{
	if ( sFileName.empty() )
	{
		return false;
	}

	FILE* fp = nullptr;
	fp = fopen(cstr(sFileName), "w" );
	if (fp == nullptr)
	{
		return false;
	}

	foreach_do( SectionMap, itSec, m_Sections )
	{
		fprintf( fp, "[%s]\n", cstr(itSec->first) );
		foreach_do( KeyMap, itVal, itSec->second )
		{
			fprintf( fp, "%s=%s\n", cstr(itVal->first), cstr(itVal->second) );
		}
	}

	fclose( fp );

	return true;
}

bool INILoader::IsExists( const std::string& Section, const std::string& Key )
{
	SectionMap::iterator itSec = m_Sections.find( Section );
	if ( itSec != m_Sections.end() )
	{
		KeyMap::iterator itVal = itSec->second.find( Key );
		if ( itVal != itSec->second.end() )
		{
			return true;
		}
	}
	return false;
}

void INILoader::Dump( void )
{
	foreach_do( SectionMap, itSec, m_Sections )
	{
		prn_inf( "[%s]", cstr(itSec->first) );
		foreach_do( KeyMap, itVal, itSec->second )
		{
			prn_inf( "\t%s=%s", cstr(itVal->first), cstr(itVal->second) );
		}
	}
}

