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
#include <MacroFunc.h>
#include <StringUtil.h>
#include <TimeUtil.h>

#define getdate()	time(NULL)

namespace util
{

	time_t maketime( time_t tDate, const std::string& sFrmt )
	{
		if ( !tDate )
		{ tDate = getdate(); }

		struct tm ti;
		_localtime64_s(&ti, &tDate);
		size_t pos = sFrmt.find("YYYY");
		if ( std::string::npos == pos )
		{ ti.tm_year = 0; }
		pos = sFrmt.find("MM");
		if ( std::string::npos == pos )
		{ ti.tm_mon = 0;}
		pos = sFrmt.find("DD");
		if ( std::string::npos == pos )
		{ ti.tm_mday = 0;}
		pos = sFrmt.find("HH");
		if ( std::string::npos == pos )
		{ ti.tm_hour = 0;}
		pos = sFrmt.find("MI");
		if ( std::string::npos == pos )
		{ ti.tm_min = 0;}
		pos = sFrmt.find("SS");
		if ( std::string::npos == pos )
		{ ti.tm_sec = 0;}

		return mktime(&ti);
	}


	std::string to_char(time_t tDate, const std::string& sFrmt)
	{
		std::string s = sFrmt;
		if ( !tDate )
		{ tDate = getdate(); }
		struct tm ti;
		_localtime64_s( &ti, &tDate );
		char szTmp[10];

		sprintf_s( szTmp, sizeof(szTmp), "%04d", ti.tm_year + 1900 );
		util::replace( "YYYY", szTmp, s );

		sprintf_s( szTmp, sizeof(szTmp), "%02d", (ti.tm_year + 1900) % 100 );
		util::replace( "YY", szTmp, s );

		sprintf_s( szTmp, sizeof(szTmp), "%02d", ti.tm_mon + 1 );
		util::replace( "MM", szTmp, s );

		sprintf_s( szTmp, sizeof(szTmp), "%02d", ti.tm_mday );
		util::replace( "DD", szTmp, s );

		sprintf_s( szTmp, sizeof(szTmp), "%02d", ti.tm_hour );
		util::replace( "HH", szTmp, s );

		sprintf_s( szTmp, sizeof(szTmp), "%02d", ti.tm_min );
		util::replace( "MI", szTmp, s );

		sprintf_s( szTmp, sizeof(szTmp), "%02d", ti.tm_sec );
		util::replace( "SS", szTmp, s );

		return s;
	}

	time_t to_date(const std::string sDate, const std::string sFrmt )
	{
		std::string _sDate = trim(sDate);
		std::string _sFrmt = trim(sFrmt);
		if ( _sDate.size() != _sFrmt.size())
			return 0;

		std::string s;
		struct tm ti; memset( &ti, 0x00, sizeof(ti) );

		size_t pos = _sFrmt.find( "YYYY" );
		if ( std::string::npos != pos )
		{
			s = _sDate.substr( pos, 4 );
			ti.tm_year = toint32(s.c_str()) - 1900;
		}
		pos = _sFrmt.find( "MM" );
		if ( std::string::npos != pos )
		{
			s = _sDate.substr( pos, 2 );
			ti.tm_mon = toint32(s.c_str()) - 1;
		}
		pos = _sFrmt.find( "DD" );
		if ( std::string::npos != pos )
		{
			s = _sDate.substr( pos, 2 );
			ti.tm_mday = toint32(s.c_str());
		}
		pos = _sFrmt.find( "HH" );
		if ( std::string::npos != pos )
		{
			s = _sDate.substr( pos, 2 );
			ti.tm_hour = toint32(s.c_str());
		}
		pos = _sFrmt.find( "MI" );
		if ( std::string::npos != pos )
		{
			s = _sDate.substr( pos, 2 );
			ti.tm_min = toint32(s.c_str());
		}
		pos = _sFrmt.find( "SS" );
		if ( std::string::npos != pos )
		{
			s = _sDate.substr( pos, 2 );
			ti.tm_sec = toint32(s.c_str());
		}
		return mktime(&ti);
	}

	bool is_leap_year( time_t tDate, int* pMonth )
	{
		if ( !tDate )
		{ tDate = getdate(); }
		struct tm ti;
		_localtime64_s(&ti, &tDate);
		int iYear = ti.tm_year + 1900;   // 주의 :년도는 1900년부터 시작
		bool bChk = !(iYear%4)? true : !(iYear%100)? false: !(iYear%400)? true : false;
		if ( pMonth != nullptr )
		{ *pMonth = ti.tm_mon;	}
		return bChk;
	}

	bool is_leap_month( time_t tDate )
	{
		int iMonth = 0;
		if ( !tDate )
		{ tDate = getdate(); }
		if ( is_leap_year(tDate, &iMonth) )
		{
			if ( iMonth == 1 ) // 월은 0 ~ 11 까지
				return true;
		}
		return false;
	}

	int lastday( time_t tDate )
	{
		static int iDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		int iMonth = 0;
		if ( !tDate )
		{ tDate = getdate(); }
		if ( is_leap_year(tDate, &iMonth) && iMonth == 1 )
		{ return 29; }
		else
		{ return iDays[iMonth];	}
	}

	time_t add_date( time_t tDate, int iSec )
	{
		if ( !tDate )
		{ tDate = getdate(); }
		tDate += iSec;
		return tDate;
	}

	time_t sub_date( time_t tDate, int iSec )
	{
		if ( !tDate )
		{ tDate = getdate(); }
		tDate -= iSec;
		return tDate;
	}

	time_t next_month( time_t tDate )
	{
		if ( !tDate )
		{ tDate = getdate(); }
		struct tm ti;
		_localtime64_s( &ti, &tDate );
		ti.tm_mon = ti.tm_mon + 1;
		return mktime( &ti );
	}

	time_t next_day( time_t tDate )
	{
		if ( !tDate )
		{ tDate = 	getdate();}
		struct tm ti;
		_localtime64_s( &ti, &tDate );
		ti.tm_mday = ti.tm_mday + 1;

		return mktime( &ti );
	}

	time_t next_hour( time_t tDate )
	{
		if ( !tDate )
		{ tDate = 	getdate();}
		struct tm ti;
		_localtime64_s( &ti, &tDate );
		ti.tm_hour += 1;

		return mktime( &ti );
	}

	unsigned __int64 time2usec( void )
	{
		FILETIME   tfile;
		::GetSystemTimeAsFileTime (&tfile);
		return (unsigned __int64)((ULONGLONG)tfile.dwHighDateTime << 32 | (ULONGLONG)tfile.dwLowDateTime) / 10;
	}

	unsigned __int64 time2msec( void )
	{
		return (unsigned __int64)(time2usec() / 1000 );
	}

#define DELTA_EPOCH_IN_MICROSECS  116444736000000000ULL

	time_t usec2unix( unsigned __int64 usec )
	{
		unsigned __int64 asInt64= usec * 10;
		asInt64 -= DELTA_EPOCH_IN_MICROSECS; // 1970-Jan-01 기준 으로 변환
		return (time_t)(asInt64 / 10000000 );
	}

	time_t msec2unix( unsigned __int64 msec )
	{
		unsigned __int64 asInt64= msec * 10000;
		asInt64 -= DELTA_EPOCH_IN_MICROSECS; // 1970-Jan-01 기준 으로 변환
		return (time_t)(asInt64 / 10000000 );
	}

#define 		ONE_MIN		60				///< 1분
#define			ONE_HOUR	(60 * 60)		///< 1시간
#define 		ONE_DAY		(ONE_HOUR * 24)	///< 1일

	long get_midnight()
	{
		return (long)(maketime( time(NULL) + ONE_DAY, "YYYYMMDD" ));
	}

	int date_diff( eDateOpt type, time_t t1, time_t t2 )
	{
		switch( type )
		{
		case SEC:
			return static_cast<int>(t1 - t2);
		case MIN:
			return static_cast<int>((t1 - t2)/ONE_MIN);
		case HOUR:
			return static_cast<int>((t1 - t2)/ONE_HOUR);
		case DAY:
		default:
			return static_cast<int>((t1 - t2)/ONE_DAY);
		}
	}

	uint32 unix_timestamp()
	{
		return (uint32)getdate();
	}

};