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
#include <map>
//<
#include <Md5Hash.h>
#include <MacroFunc.h>

///////////////////////////////////////////////////////////////////////////////////

namespace
{
	//< MD5Hash
	typedef void (WINAPI *fnMD5Init)	( MD5_CTX *);
	typedef void (WINAPI *fnMD5Update)	( MD5_CTX *, const unsigned char*, unsigned int );
	typedef void (WINAPI *fnMD5Final)	( MD5_CTX * );
	HINSTANCE		g_hInstance = NULL;
	fnMD5Init		Md5InitFn	= NULL;
	fnMD5Update		Md5UpdateFn	= NULL;
	fnMD5Final		Md5FinalFn	= NULL;

	//< String Hash
	const unsigned int	HASH_TYPE	= 0x2;
	const unsigned int	HASH_SEED1	= 0x7FED7FEDu;
	const unsigned int	HASH_SEED2	= 0xEEEFEEEFu;
	unsigned int		g_HashData[1280];
	std::map<const char *, unsigned int>	m_HashMap;
	std::map<unsigned int, std::string>		m_StringMap;

	// 문자열 해쉬 값을 캐쉬하여 수행속도를 높인 문자열해쉬함수
	// 문자열의 길이에 따라 수행속도가 결정된다.
	// 문자열의 길이가 49byte 이상일 경우에 성능이 좋다.
	unsigned int StrHash(const char *pStr, int len)
	{
		if ( len == 0 ) len = (int)strlen( pStr );
		unsigned int seed1 = HASH_SEED1, seed2 = HASH_SEED2;
		const char * key = pStr;
		for(register int i=0; i<len; i++ )
		{
			unsigned char ch = (unsigned char)*(key+i);
			seed1 = g_HashData[(HASH_TYPE<<8)+ch] ^ (seed1+seed2);
			seed2 = ch+seed1+seed2+(seed2<<5)+3;
		}

		return seed1;
	}
};

initializer_begin
{
	g_hInstance = ::LoadLibrary("Cryptdll.dll");
	if ( g_hInstance )
	{
		Md5InitFn	= reinterpret_cast<fnMD5Init>(::GetProcAddress(g_hInstance, "MD5Init"));
		Md5UpdateFn = reinterpret_cast<fnMD5Update>(::GetProcAddress(g_hInstance, "MD5Update"));
		Md5FinalFn	= reinterpret_cast<fnMD5Final>(::GetProcAddress(g_hInstance, "MD5Final"));
	}

	memset(g_HashData, 0x00, sizeof(g_HashData));
	unsigned int iSeed = 0x00100001, iInx1 = 0, iInx2 = 0, i;
	for (iInx1 = 0; iInx1 < 0x100; iInx1++)
	{ 
		for(iInx2 = iInx1, i = 0; i < 5; i++, iInx2 += 0x100)
		{ 
			unsigned int temp1, temp2;

			iSeed = (iSeed * 125 + 3) % 0x2AAAAB;
			temp1 = (iSeed & 0xFFFF) << 0x10;

			iSeed = (iSeed * 125 + 3) % 0x2AAAAB;
			temp2 = (iSeed & 0xFFFF);

			g_HashData[iInx2] = (temp1 | temp2); 
		}
	}
}
end_initializer;


//<
Md5::Md5()
{
	memset( m_Data, 0x00, sizeof(m_Data) );
}

std::string Md5::ToString() const
{
	int iOffset = 0;
	char szBuf[33]; memset( szBuf, 0x00, sizeof(szBuf) );

	for ( size_t i = 0; i < 16; ++i )
	{
		iOffset += sprintf_s(szBuf+iOffset, sizeof(szBuf), "%02X", (unsigned char)m_Data[i]);
	}

	return szBuf;
}

bool Md5::FromString( const std::string& sData )
{
	if( sData.size() != 16*2 )
	{
		return false;
	}

	unsigned int val;
	for ( size_t i = 0; i < 16; ++i )
	{
		std::string sSub = sData.substr(i*2, 2);
		if( sscanf_s(sSub.c_str(), "%02X", &val) != 1 )
		{
			m_Data[i] = 0;
		}
		else
		{
			m_Data[i] = static_cast<unsigned char>(val);
		}
	}
	return true;
}

bool Md5::operator==( const Md5 & r ) const
{
	return memcmp(m_Data, r.m_Data, 16) == 0;
}

bool Md5::operator!=( const Md5 & r ) const
{
	return !(operator ==(r));
}


///////////////////////////////////////////////////////////////////////////////////

Md5Hash::Md5Hash(void)
{
	Md5InitFn( &m_Ctx );
}

Md5Hash::~Md5Hash(void)
{
}

void Md5Hash::Append( const void * buf, size_t len )
{
	Md5UpdateFn(&m_Ctx, reinterpret_cast<const unsigned char*>(buf), static_cast<unsigned int>(len));
}

Md5 Md5Hash::Finish()
{
	Md5 rst;
	Md5FinalFn(&m_Ctx);
	memcpy( rst.m_Data, m_Ctx.digest, sizeof(rst.m_Data) );
	return rst;
}

Md5 Md5Hash::FromFile( const std::string& sFileName )
{
	FILE * file = _fsopen(sFileName.c_str(), "rb", _SH_DENYNO);
	if( file == NULL )
	{
		return Md5();
	}

	Md5Hash hasher;
	enum { BUF_LEN = 1024 * 16 };
	char buf[BUF_LEN];
	for(;;)
	{
		size_t n = fread(buf, 1, BUF_LEN, file);
		if( n != BUF_LEN )
		{
			if( n < BUF_LEN && n > 0 )
			{
				hasher.Append(buf, n);
			}
			break;
		}
		else
		{
			hasher.Append(buf, n);
		}
	}

	fclose(file);

	return hasher.Finish();
}

Md5 Md5Hash::FromData( const std::string& sData )
{
	Md5Hash hasher;
	hasher.Append(sData.data(), sData.size());

	return hasher.Finish();
}

unsigned int Md5Hash::ToHash( const std::string& sData )
{
	return StrHash( sData.c_str(), (int)sData.size() );
}