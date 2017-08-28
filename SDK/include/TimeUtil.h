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
//<
#include <string>
#include <time.h>


/** 
	유틸리티 모음을 위한 네임스페이스.
*/
namespace util
{
	/** 
		UTC시간으로 부터 날짜포멧에 따라 값을 추출하여 UTC 시간을 만든다.
		@param tDate UTC 시간
		@param sFrmt 날짜포멧( 예: YYYY-MM-DD HH:MI:SS )
		@li YYYY 년도
		@li YY 년도
		@li MM 월
		@li DD 일
		@li HH 시간
		@li MI 분
		@li SS 초

		@verbatim
		// 현재시간에서 년월일 값만 뽑아서 t에 저장한다.
		time_t t = maketime( time(NULL), "YYYYMMDD" );
		// 현재시간에서 년월일시 값만 뽑아서 t 에 저장한다.
		time_t t = maketime( (time_t)0, "YYYYMMDDHH" );
		@endverbatim
	*/
	time_t				maketime( time_t tDate, const std::string& sFrmt );
	/** 
		UTC시간을 날짜포멧에 따라 시간문자열로 만든다.
		@param tDate UTC 시간
		@param sFrmt 날짜포멧( 예: YYYY-MM-DD HH:MI:SS )
		@li YYYY 년도
		@li YY 년도
		@li MM 월
		@li DD 일
		@li HH 시간
		@li MI 분
		@li SS 초

		@verbatim
		printf( "오늘의 날짜는 %s\n", util::to_char((time_t)0, "YYYY/MM/DD HH:MI:SS").c_str() );
		결과) 오늘의 날짜는 2013/05/23 00:00:00
		@endverbatim
	*/
	std::string			to_char( time_t tDate, const std::string& sFrmt );
	/** 
		시간문자열을 날짜포멧에 따라 UTC 시간으로 변환한다.
		@param sDate 시간문자열(예: 2013-05-06 12:00:33)
		@param sFrmt 날짜포멧( 예: YYYY-MM-DD HH:MI:SS )
		@li YYYY 년도
		@li YY 년도
		@li MM 월
		@li DD 일
		@li HH 시간
		@li MI 분
		@li SS 초

		@verbatim
		time_t t = util::to_char( "2013-04-05 13:24:25", "YYYY-MM-DD HH:MI:SS" );
		printf( "날짜는 %s\n", util::to_char(t, "YYYY-MM-DD HH:MI:SS").c_str() );
		결과) 날짜는 2013-04-05 13:24:25
		@endverbatim
	*/
	time_t				to_date( const std::string sDate, const std::string sFrmt );
	/** 
		입력한 UTC 시간이 윤년인지 조사한다.
		@param tDate UTC 시간
		@param pMonth 현재 년도가 윤년일 경우 윤달을 얻어온다.
	*/
	bool				is_leap_year( time_t tDate, int* pMonth = nullptr );
	/** 
		입력한 UTC 시간이 윤달인지 조사한다.
		@param tDate UTC 시간
	*/
	bool				is_leap_month( time_t tDate );
	/** 
		월의 마지막 날짜를 구한다.
		@param tDate UTC 시간
	*/
	int					lastday( time_t tDate );
	/** 
		입력한 UTC 시간에 초를 더한다.
		@param tDate UTC 시간
		@param iSec 초
	*/
	time_t				add_date( time_t tDate, int iSec );
	/** 
		입력한 UTC 시간에 초를 뺀다.
		@param tDate UTC 시간
		@param iSec 초
	*/
	time_t				sub_date( time_t tDate, int iSec );
	/** 
		입력한 UTC 시간 + 1개월을 반환한다.
		@param tDate UTC 시간
		@verbatim
		// 현재 2013-05-23 12:43:45 라면
		time_t t = util::next_month( time(NULL) );
		printf( "다음달 오늘은 %s\n", util::to_char(t, "YYYY/MM/DD HH:MI:SS").c_str() );
		결과) 다음달 오늘은 2013-06-23 12:43:45
		@endverbatim
	*/
	time_t				next_month( time_t tDate );
	/** 
		입력한 UTC 시간 + 1일을 반환한다.
		@param tDate UTC 시간
		@verbatim
		// 현재 2013-05-23 12:43:45 라면
		time_t t = util::next_month( time(NULL) );
		printf( "내일은 %s\n", util::to_char(t, "YYYY/MM/DD HH:MI:SS").c_str() );
		결과) 내일은 2013-05-24 12:43:45
		@endverbatim
	*/
	time_t				next_day( time_t tDate );
	/** 
		입력한 UTC 시간 + 1시간을 반환한다.
		@param tDate UTC 시간
		@verbatim
		// 현재 2013-05-23 12:43:45 라면
		time_t t = util::next_month( time(NULL) );
		printf( "내일은 %s\n", util::to_char(t, "YYYY/MM/DD HH:MI:SS").c_str() );
		결과) 다음시간은 2013-05-23 13:43:45
		@endverbatim
	*/
	time_t				next_hour( time_t tDate );
	/** 
		현재 시간을 마이크로초 단위로 얻어온다. 결과값을 1000000으로 나누면 초단위가 된다.
	*/
	unsigned __int64	time2usec( void );
	/** 
		현재 시간을 밀리초 단위로 얻어온다. 결과값을 1000으로 나누면 초단위가 된다.
	*/
	unsigned __int64	time2msec( void );
	/** 
		마이크로초 단위의 시간을 UTC 시간으로 변경한다.
		@param usec 마이크로초
		@verbatim
		// 현재 2013-05-23 12:43:45 라면
		unsigned __int64 usec = util::time2usec();
		printf( "현재시간은 %s\n", util::to_char(t, "YYYY/MM/DD HH:MI:SS").c_str() );
		결과) 현재시간은 2013-05-23 12:43:45
		@endverbatim
	*/
	time_t				usec2unix( unsigned __int64 usec );
	/** 
		밀리초 단위의 시간을 UTC 시간으로 변경한다.
		@param usec 밀리초
		@verbatim
		// 현재 2013-05-23 12:43:45 라면
		unsigned __int64 msec = util::time2msec();
		printf( "현재시간은 %s\n", util::to_char(t, "YYYY/MM/DD HH:MI:SS").c_str() );
		결과) 현재시간은 2013-05-23 12:43:45
		@endverbatim
	*/
	time_t				msec2unix( unsigned __int64 mesc );
	/** 
		현재일의 자정(24:00)을 구한다.
	*/
	long				get_midnight();

	/** date_diff 함수의 연산방법 정의 상수 */
	enum eDateOpt
	{
		SEC			= 0,				///< 초단위
		MIN			= 1,				///< 분단위
		HOUR		= 2,				///< 시간단위
		DAY			= 3,				///< 일단위
	};
	/** 
		UTC 시간의 차이를 옵션에 따라 환산하여 반환한다.
		@param type 옵션
		@li SEC t2 - t1 결과를 초단위로 환산
		@li MIN t2 - t1 결과를 분단위로 환산
		@li HOUR t2 - t1 결과를 시간단위 환산
		@li DAY t2 - t1 결과를 일단위 환산

		@verbatim
		// t1 = 2013-05-23 12:43:45, t2 = 2013-05-23 12:42:45
		printf( "t2 - t1 의 차이는 %d 초\n", util::date_diff(util::SEC, t1, t2) );
		결과) t2 - t1 의 차이는 60 초
		@endverbatim
	*/
	int					date_diff( eDateOpt opt, time_t t1, time_t t2 );

	/**
	db unix_timestamp() 대응
	*/
	unsigned int		unix_timestamp();
};