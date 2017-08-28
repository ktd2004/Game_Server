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
	��ƿ��Ƽ ������ ���� ���ӽ����̽�.
*/
namespace util
{
	/** 
		UTC�ð����� ���� ��¥���信 ���� ���� �����Ͽ� UTC �ð��� �����.
		@param tDate UTC �ð�
		@param sFrmt ��¥����( ��: YYYY-MM-DD HH:MI:SS )
		@li YYYY �⵵
		@li YY �⵵
		@li MM ��
		@li DD ��
		@li HH �ð�
		@li MI ��
		@li SS ��

		@verbatim
		// ����ð����� ����� ���� �̾Ƽ� t�� �����Ѵ�.
		time_t t = maketime( time(NULL), "YYYYMMDD" );
		// ����ð����� ����Ͻ� ���� �̾Ƽ� t �� �����Ѵ�.
		time_t t = maketime( (time_t)0, "YYYYMMDDHH" );
		@endverbatim
	*/
	time_t				maketime( time_t tDate, const std::string& sFrmt );
	/** 
		UTC�ð��� ��¥���信 ���� �ð����ڿ��� �����.
		@param tDate UTC �ð�
		@param sFrmt ��¥����( ��: YYYY-MM-DD HH:MI:SS )
		@li YYYY �⵵
		@li YY �⵵
		@li MM ��
		@li DD ��
		@li HH �ð�
		@li MI ��
		@li SS ��

		@verbatim
		printf( "������ ��¥�� %s\n", util::to_char((time_t)0, "YYYY/MM/DD HH:MI:SS").c_str() );
		���) ������ ��¥�� 2013/05/23 00:00:00
		@endverbatim
	*/
	std::string			to_char( time_t tDate, const std::string& sFrmt );
	/** 
		�ð����ڿ��� ��¥���信 ���� UTC �ð����� ��ȯ�Ѵ�.
		@param sDate �ð����ڿ�(��: 2013-05-06 12:00:33)
		@param sFrmt ��¥����( ��: YYYY-MM-DD HH:MI:SS )
		@li YYYY �⵵
		@li YY �⵵
		@li MM ��
		@li DD ��
		@li HH �ð�
		@li MI ��
		@li SS ��

		@verbatim
		time_t t = util::to_char( "2013-04-05 13:24:25", "YYYY-MM-DD HH:MI:SS" );
		printf( "��¥�� %s\n", util::to_char(t, "YYYY-MM-DD HH:MI:SS").c_str() );
		���) ��¥�� 2013-04-05 13:24:25
		@endverbatim
	*/
	time_t				to_date( const std::string sDate, const std::string sFrmt );
	/** 
		�Է��� UTC �ð��� �������� �����Ѵ�.
		@param tDate UTC �ð�
		@param pMonth ���� �⵵�� ������ ��� ������ ���´�.
	*/
	bool				is_leap_year( time_t tDate, int* pMonth = nullptr );
	/** 
		�Է��� UTC �ð��� �������� �����Ѵ�.
		@param tDate UTC �ð�
	*/
	bool				is_leap_month( time_t tDate );
	/** 
		���� ������ ��¥�� ���Ѵ�.
		@param tDate UTC �ð�
	*/
	int					lastday( time_t tDate );
	/** 
		�Է��� UTC �ð��� �ʸ� ���Ѵ�.
		@param tDate UTC �ð�
		@param iSec ��
	*/
	time_t				add_date( time_t tDate, int iSec );
	/** 
		�Է��� UTC �ð��� �ʸ� ����.
		@param tDate UTC �ð�
		@param iSec ��
	*/
	time_t				sub_date( time_t tDate, int iSec );
	/** 
		�Է��� UTC �ð� + 1������ ��ȯ�Ѵ�.
		@param tDate UTC �ð�
		@verbatim
		// ���� 2013-05-23 12:43:45 ���
		time_t t = util::next_month( time(NULL) );
		printf( "������ ������ %s\n", util::to_char(t, "YYYY/MM/DD HH:MI:SS").c_str() );
		���) ������ ������ 2013-06-23 12:43:45
		@endverbatim
	*/
	time_t				next_month( time_t tDate );
	/** 
		�Է��� UTC �ð� + 1���� ��ȯ�Ѵ�.
		@param tDate UTC �ð�
		@verbatim
		// ���� 2013-05-23 12:43:45 ���
		time_t t = util::next_month( time(NULL) );
		printf( "������ %s\n", util::to_char(t, "YYYY/MM/DD HH:MI:SS").c_str() );
		���) ������ 2013-05-24 12:43:45
		@endverbatim
	*/
	time_t				next_day( time_t tDate );
	/** 
		�Է��� UTC �ð� + 1�ð��� ��ȯ�Ѵ�.
		@param tDate UTC �ð�
		@verbatim
		// ���� 2013-05-23 12:43:45 ���
		time_t t = util::next_month( time(NULL) );
		printf( "������ %s\n", util::to_char(t, "YYYY/MM/DD HH:MI:SS").c_str() );
		���) �����ð��� 2013-05-23 13:43:45
		@endverbatim
	*/
	time_t				next_hour( time_t tDate );
	/** 
		���� �ð��� ����ũ���� ������ ���´�. ������� 1000000���� ������ �ʴ����� �ȴ�.
	*/
	unsigned __int64	time2usec( void );
	/** 
		���� �ð��� �и��� ������ ���´�. ������� 1000���� ������ �ʴ����� �ȴ�.
	*/
	unsigned __int64	time2msec( void );
	/** 
		����ũ���� ������ �ð��� UTC �ð����� �����Ѵ�.
		@param usec ����ũ����
		@verbatim
		// ���� 2013-05-23 12:43:45 ���
		unsigned __int64 usec = util::time2usec();
		printf( "����ð��� %s\n", util::to_char(t, "YYYY/MM/DD HH:MI:SS").c_str() );
		���) ����ð��� 2013-05-23 12:43:45
		@endverbatim
	*/
	time_t				usec2unix( unsigned __int64 usec );
	/** 
		�и��� ������ �ð��� UTC �ð����� �����Ѵ�.
		@param usec �и���
		@verbatim
		// ���� 2013-05-23 12:43:45 ���
		unsigned __int64 msec = util::time2msec();
		printf( "����ð��� %s\n", util::to_char(t, "YYYY/MM/DD HH:MI:SS").c_str() );
		���) ����ð��� 2013-05-23 12:43:45
		@endverbatim
	*/
	time_t				msec2unix( unsigned __int64 mesc );
	/** 
		�������� ����(24:00)�� ���Ѵ�.
	*/
	long				get_midnight();

	/** date_diff �Լ��� ������ ���� ��� */
	enum eDateOpt
	{
		SEC			= 0,				///< �ʴ���
		MIN			= 1,				///< �д���
		HOUR		= 2,				///< �ð�����
		DAY			= 3,				///< �ϴ���
	};
	/** 
		UTC �ð��� ���̸� �ɼǿ� ���� ȯ���Ͽ� ��ȯ�Ѵ�.
		@param type �ɼ�
		@li SEC t2 - t1 ����� �ʴ����� ȯ��
		@li MIN t2 - t1 ����� �д����� ȯ��
		@li HOUR t2 - t1 ����� �ð����� ȯ��
		@li DAY t2 - t1 ����� �ϴ��� ȯ��

		@verbatim
		// t1 = 2013-05-23 12:43:45, t2 = 2013-05-23 12:42:45
		printf( "t2 - t1 �� ���̴� %d ��\n", util::date_diff(util::SEC, t1, t2) );
		���) t2 - t1 �� ���̴� 60 ��
		@endverbatim
	*/
	int					date_diff( eDateOpt opt, time_t t1, time_t t2 );

	/**
	db unix_timestamp() ����
	*/
	unsigned int		unix_timestamp();
};