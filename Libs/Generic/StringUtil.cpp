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
//<
#include <algorithm>
//<
#include <MacroFunc.h>
#include <StringUtil.h>

namespace util
{
	std::string lower( const std::string& rData )
	{
		std::string s = rData;
		std::transform( s.begin(), s.end(), s.begin(), tolower );
		return s;
	}

	std::wstring lower( const std::wstring& rData )
	{
		std::wstring s = rData;
		std::transform( s.begin(), s.end(), s.begin(), towlower );
		return s;
	}

	std::wstring upper( const std::wstring& rData )
	{
		std::wstring s = rData;
		std::transform( s.begin(), s.end(), s.begin(), towupper );
		return s;
	}

	std::string upper( const std::string& rData )
	{
		std::string s = rData;
		std::transform( s.begin(), s.end(), s.begin(), toupper );
		return s;
	}


	std::string trim( const std::string& rData )
	{
		const std::string WHITE_SPACES(" \t\r\n");
		size_t p1 = rData.find_first_not_of(WHITE_SPACES);
		if( p1 == std::string::npos )
		{
			return null_string;
		}

		size_t p2 = rData.find_last_not_of(WHITE_SPACES);
		if( p2 == std::string::npos )
		{
			return null_string;
		}

		return rData.substr(p1, p2-p1+1);
	}

	///< key=val 문자열에서 key 을 얻어온다.
	std::string pairkey( const std::string s, const std::string v )
	{
		std::string key;
		size_t pos = s.find_first_of( v );
		if ( pos != std::string::npos )
		{
			key = trim(s.substr(0, pos));
		}
		return key;
	}

	///< key=val 문자열에서 key 을 얻어온다.
	std::string pairval( const std::string s, const std::string v )
	{
		std::string val;
		size_t pos = s.find_first_of( v );
		if ( pos != std::string::npos )
		{
			val = trim(s.substr(pos+1));
		}
		return val;
	}

	std::string chartohex( unsigned char ch, bool bUpperCase )
	{
		char szBuf[8];
		if( bUpperCase )
		{
			sprintf_s(szBuf, 8, "%02X", (unsigned)ch);
		}
		else
		{
			sprintf_s(szBuf, 8, "%02x", (unsigned)ch);
		}
		return szBuf;
	}

	unsigned char hextochar( const char *pData )
	{
		unsigned int val;
		if( ::sscanf_s(pData, "%02x", &val) != 1 )
		{
			return 0;
		}
		return static_cast<unsigned char>(val);
	}

	unsigned char hextochar( const std::string& sData )
	{
		return hextochar( sData.c_str() );
	}

	std::string tohexstring( const std::string& sData )
	{
		const char *pBuf = sData.empty() ? nullptr : sData.c_str();
		char szHex[8];
		std::string sRet;

		while ( *pBuf )
		{
			sprintf_s( szHex, 8, "%02X", (unsigned)*pBuf);
			sRet += szHex;
			pBuf++;
		}
		return sRet;
	}

	std::string urlencode( const std::string& sData )
	{
		const char *pstr = sData.empty() ? nullptr : sData.c_str();
		std::string sBuf;

		while (*pstr)
		{
			char ch = *pstr;
			if ((ch >= '0' && ch <= '9') ||
				(ch >= 'A' && ch <= 'Z') ||
				(ch >= 'a' && ch <= 'z') ||
				ch == '@' || ch == '.' )
				sBuf += *pstr;
			else if (ch == ' ') 
				sBuf += '+';
			else 
				sBuf += '%', sBuf += chartohex( ch );

			pstr++;
		}

		return sBuf;
	}

	std::string urldecode( const std::string& sData )
	{
		const char *pstr = sData.empty() ? nullptr : sData.c_str();
		std::string sBuf;

		while (*pstr)
		{
			if (*pstr == '%')
			{
				if (pstr[1] && pstr[2])
				{
					sBuf += hextochar( &pstr[1] );
					pstr += 2;
				}
			}
			else if (*pstr == '+')
			{ 
				sBuf += ' ';
			}
			else
			{
				sBuf += *pstr;
			}
			pstr++;
		}
		return sBuf;
	}


	size_t tokenizer( const std::string& sLine, stringlist &sList, const char cSep )
	{
		static const char DOUBLE_QUATATION	= '\"';
		static const char SEPERATOR = ',';
		static const char NULLCHAR	= '\0';
		static const char ESCAPE_CHAR = '\\';

		const char *pBuf = sLine.empty() ? nullptr : sLine.c_str();
		isnull_ret(pBuf, 0);
		size_t iLen = sLine.size();
		std::string sToken;

		bool bQuat = false;
		for ( size_t i = 0; i < iLen+1; ++i )
		{
			if ( *pBuf == NULLCHAR )
			{
				if ( sToken.size() )
					sList.push_back( sToken );
				return sList.size();
			}
			else if ( *pBuf == ESCAPE_CHAR && *(pBuf + 1) == DOUBLE_QUATATION )
			{
				pBuf++;
				sToken += *pBuf;
			}
			else if ( *pBuf == DOUBLE_QUATATION )
			{
				bQuat = bQuat ? false : true;
			}
			else if ( *pBuf == cSep )
			{
				if ( !bQuat )
				{
					sList.push_back( sToken );
					sToken.clear();
				}
				else
				{
					sToken += *pBuf;
				}
			}
			else 
			{
				sToken += *pBuf;
			}
			pBuf++;
		}

		return sList.size();
	}

	size_t tokenizer( const std::string& sLine, stringvector &sList, const char cSep )
	{
		static const char DOUBLE_QUATATION	= '\"';
		static const char SEPERATOR = ',';
		static const char NULLCHAR	= '\0';
		static const char ESCAPE_CHAR = '\\';

		const char *pBuf = is_empty_cstr_ret_null(sLine);
		isnull_ret(pBuf, 0);
		size_t iLen = sLine.size();
		std::string sToken;

		bool bQuat = false;
		for ( size_t i = 0; i < iLen+1; ++i )
		{
			if ( *pBuf == NULLCHAR )
			{
				if ( sToken.size() )
					sList.push_back( sToken );
				return sList.size();
			}
			else if ( *pBuf == ESCAPE_CHAR && *(pBuf + 1) == DOUBLE_QUATATION )
			{
				pBuf++;
				sToken += *pBuf;
			}
			else if ( *pBuf == DOUBLE_QUATATION )
			{
				bQuat = bQuat ? false : true;
			}
			else if ( *pBuf == cSep )
			{
				if ( !bQuat )
				{
					sList.push_back( sToken );
					sToken.clear();
				}
				else
				{
					sToken += *pBuf;
				}
			}
			else 
			{
				sToken += *pBuf;
			}
			pBuf++;
		}

		return sList.size();
	}

	bool replace(const std::string sFind, const std::string& sRep, std::string& sSrc )
	{
		size_t iOffset = 0;
		std::string t;
		bool b = false;

		while (1)
		{
			size_t pos = sSrc.find( sFind, iOffset );
			if ( std::string::npos == pos )
			{
				t += sSrc.substr( iOffset, -1 );
				break;
			}
			t += sSrc.substr(iOffset, pos - iOffset);
			t += sRep;
			iOffset = pos + sFind.size();
			b = true;
		}
		sSrc = t;
		return b;
	}

	size_t replace(const std::string sFind, const std::string& sRep, std::string& sSrc, size_t iOffset )
	{
		std::string t;
		size_t pos = sSrc.find( sFind, iOffset );
		if ( std::string::npos == pos )
		{
			return std::string::npos;
		}

		t += sSrc.substr(0, pos);
		t += sRep;
		sSrc = t + sSrc.substr(pos+1, -1);
		iOffset = pos + sRep.size();
		return iOffset;
	}

	std::string rpad(const std::string& s, int w)
	{
		size_t iLen = s.size();
		if ( w > (int)iLen )
		{
			std::string space(w - (int)iLen, ' ');
			return space + s;
		}
		return s.substr(0,w);
	}

	std::string lpad(const std::string s, int w)
	{
		size_t iLen = s.size();
		if ( w > (int) iLen )
		{
			std::string space(w - (int)iLen, ' ');
			return s + space;
		}
		return s.substr(0,w);
	}

	std::string intstr(short i)
	{
		char szBuf[20];
		sprintf_s( szBuf, sizeof(szBuf), "%d", i);
		return szBuf;
	}

	std::string intstr(unsigned short i)
	{
		char szBuf[20];
		sprintf_s( szBuf, sizeof(szBuf), "%u", i);
		return szBuf;
	}

	std::string intstr(int i)
	{
		char szBuf[20];
		sprintf_s( szBuf, sizeof(szBuf), "%d", i);
		return szBuf;
	}

	std::string intstr(unsigned int i)
	{
		char szBuf[20];
		sprintf_s( szBuf, sizeof(szBuf), "%u", i);
		return szBuf;
	}

	std::string intstr(__int64 i)
	{
		char szBuf[20];
		sprintf_s( szBuf, sizeof(szBuf), "%I64d", i);
		return szBuf;
	}

	std::string intstr(double i)
	{
		char szBuf[20];
		sprintf_s( szBuf, sizeof(szBuf), "%lf", i);
		return szBuf;
	}

	std::string intstr(float i)
	{
		char szBuf[20];
		sprintf_s( szBuf, sizeof(szBuf), "%f", i);
		return szBuf;
	}

	bool isnum( const std::string& s )
	{
		size_t iLen = s.size();
		for ( size_t i = 0; i < iLen; i++ )
		{
			if ( isdigit( s[i] ) == 0 )
				return false;
		}
		return true;
	}

	std::string center(const std::string& s, int size, char f )
	{
		int iLen = size-(unsigned int)s.length();
		if ( iLen >= 0 )
		{ iLen = iLen / 2; }
		else
		{ iLen = 0; }

		std::string val(iLen, f);
		size = size - (val.length() * 2 + s.length());
		return val + s + std::string(size, ' ') + val;
	}

	bool escape(const std::string& s)
	{
		const std::string SPECIAL_CHAR(" \t\r\n%\\/*\"',`;=");
		size_t p1 = s.find_first_of(SPECIAL_CHAR);
		if( p1 != std::string::npos )
		{
			return true;
		}
		return false;
	}

	void parse_affinity( const std::string& sAffinity, int32 iCount, std::vector<int32>& vCPUs )
	{
		stringlist CPUList;
		util::tokenizer( sAffinity, CPUList, ',' );
		stringlist::iterator it = CPUList.begin();
		vCPUs.clear();
		for ( int32 i = 0; i < iCount; i++ )
		{	
			if ( CPUList.size() == 0 )
			{
				vCPUs.push_back( (i%4)+1 );
				continue;
			}

			if ( it == CPUList.end() )
			{
				it = CPUList.begin();
			}

			vCPUs.push_back( toint32(it->c_str()) );
			++it;				
		}
	}

	std::wstring ansi_to_wide(const_string & sData)
	{
		UINT iCodePage = CP_ACP;

		if( sData.empty() )
		{
			return std::wstring();
		}

		int32 iLen = MultiByteToWideChar(iCodePage, 0, sData.data(), (int32)sData.size(), NULL, 0);
		if( iLen == 0 )
		{
			return std::wstring();
		}

		std::vector<wchar_t> sRst((size_t)iLen);
		MultiByteToWideChar(iCodePage, 0, sData.data(), (int32)sData.size(), &(sRst[0]), iLen);
		return std::wstring(&(sRst[0]), sRst.size());
	}

	std::string wide_to_ansi(const std::wstring & sData)
	{
		UINT iCodePage = CP_ACP;

		if( sData.empty() )
		{
			return std::string();		// empty string.
		}

		int32 iLen = WideCharToMultiByte(iCodePage, 0, sData.data(), (int32)sData.size(), NULL, 0, NULL, NULL);
		if( iLen == 0 )
		{
			return std::string();
		}

		std::vector<char> sRst((size_t)iLen);

		WideCharToMultiByte(iCodePage, 0, sData.data(), (int32)sData.size(), &(sRst[0]), iLen, NULL, NULL);
		return std::string(&(sRst[0]), sRst.size());
	}

	std::string ansi_to_utf8( const_string & sData )
	{
		/*
		MS에서 제공하는 API만으로 Ansi를 Utf8으로 전환하려면 Wide로 바꾼다음 다시 바꿔야 한다.
		그래서 MultiByteToWideChar 로 wchar_t로 전환한 다음 다시 codepage를 UTF8으로 해서 Multibyte로 바꾼다.
		*/

		if( sData.empty() )
		{
			return string();
		}

		uint32 iCodePage = CP_ACP;

		int32 iLen = MultiByteToWideChar(iCodePage, 0, sData.data(), (int32)sData.size(), NULL, 0);
		if ( iLen == 0 )
		{
			return string();
		}

		std::vector<wchar_t> sWide((size_t)iLen);
		MultiByteToWideChar(iCodePage, 0, sData.data(), (int32)sData.size(), &(sWide[0]), iLen);

		int32 iUtf8Len = WideCharToMultiByte(CP_UTF8, 0, &(sWide[0]), iLen, NULL, 0, NULL, NULL);
		if ( iUtf8Len == 0 )
		{
			return string();
		}

		std::vector<char> sRst((size_t)iUtf8Len);
		WideCharToMultiByte(CP_UTF8, 0, &(sWide[0]), iLen, &(sRst[0]), iUtf8Len, NULL, NULL);
		return string(&(sRst[0]), sRst.size());
	}

	std::string utf8_to_ansi( const_string & sData )
	{
		if( sData.empty() )
		{
			return string();
		}

		int32 iLen = MultiByteToWideChar(CP_UTF8, 0, sData.data(), (int32)sData.size(), NULL, 0);
		if( iLen == 0 )
		{
			return string();
		}

		std::vector<wchar_t> sWide((size_t)iLen);
		MultiByteToWideChar(CP_UTF8, 0, sData.data(), (int32)sData.size(), &(sWide[0]), iLen);

		UINT iCodePage = CP_ACP;

		int32 iUtf8Len = WideCharToMultiByte(iCodePage, 0, &(sWide[0]), iLen, NULL, 0, NULL, NULL);
		if( iUtf8Len == 0 )
		{
			return string();
		}

		std::vector<char> sRst((size_t)iUtf8Len);
		WideCharToMultiByte(iCodePage, 0, &(sWide[0]), iLen, &(sRst[0]), iUtf8Len, NULL, NULL);
		return string(&(sRst[0]), sRst.size());
	}

	std::string getfilename( const std::string& _path )
	{
//		char _token = '\\';
		const std::string _token("\\/");
		size_t _pos = _path.find_last_of( _token );
		if( _pos == std::string::npos )
		{
			return _path;
		}

		return _path.substr( _pos + 1 );
	}

	std::string getprocname( const std::string& _path )
	{
		std::string s = getfilename(_path);
		size_t _pos = s.find_last_of( '.' );
		if( _pos == std::string::npos )
		{
			return s;
		}
		return s.substr( 0, _pos );
	}

	std::string getworkpath( const std::string& _path )
	{
//		char _token = '\\';
		const std::string _token("\\/");
		size_t _pos = _path.find_last_of( _token );
		if( _pos == std::string::npos )
		{
			return _path;
		}

		return _path.substr( 0, _pos );
	}
};