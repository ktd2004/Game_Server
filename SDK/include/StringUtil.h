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

#include <DataTypes.h>

///< string list 타입 선언
typedef std::list<std::string> stringlist;

/** 
	유틸리티 모음을 위한 네임스페이스.
*/
namespace util
{
	/** 
		입력한 문자열을 소문자로 변경한다.
		@param rData 문자열
	*/
	std::string		lower( const std::string& rData );
	/** 
		입력한 문자열을 소문자로 변경한다.
		@param rData 문자열
	*/
	std::wstring	lower( const std::wstring& rData );
	/** 
		입력한 문자열을 대문자로 변경한다.
		@param rData 문자열
	*/
	std::wstring	upper( const std::wstring& rData );
	/** 
		입력한 문자열을 대문자로 변경한다.

		@param rData 문자열
	*/
	std::string		upper( const std::string& rData );
	/** 
		입력한 문자열의 앞,뒤에 공백 및 탭 문자를 제거한다.
		@param rData 문자열
	*/
	std::string		trim( const std::string& rData );
	/** 
		입력한 문자열을 분리자로 분할하여 키(key) 문자열을 얻어온다.
		@param s 문자열
		@param v 문자열 분리자(예: key=valule)

		@verbatim
		std::string s = "socketaddr=127.0.0.3";
		printf( "key 는 %s \n", util::pairkey(s, "=").c_str() );
		@endverbatim
	*/
	std::string		pairkey( const std::string s, const std::string v = "=" );
	/** 
		입력한 문자열을 분리자로 분할하여 값(value) 문자열을 얻어온다.
		@param s 문자열
		@param v 문자열 분리자(예: key=valule)

		@verbatim
		std::string s = "socketaddr=127.0.0.3";
		printf( "value 값은 %s \n", util::pairval(s, "=").c_str() );
		@endverbatim
	*/
	std::string		pairval( const std::string s, const std::string v = "=" );
	/** 
		입력한 문자를 hexa 문자열로 변경한다.
		@param ch 문자
		@param bUpperCase hexa 문자열을 대문자로 반환할지 여부
	*/
	std::string		chartohex( unsigned char ch, bool bUpperCase = false );
	/** 
		hexa문자열을 문자로 반환한다.
		@param pData hexa 문자열
	*/
	unsigned char	hextochar( const char *pData );
	/** 
		입력한 hexa문자열을 문자로 반환한다.
		@param pData hexa 문자열
	*/
	unsigned char	hextochar( const std::string& sData );
	/** 
		문자열를 hexa 문자열로 변경한다.
		@param sData 문자열
	*/
	std::string		tohexstring( const std::string& sData );
	/** 
		입력한 문자열을 URL 인코딩 문자열로 변환하여 반환한다.
		@param sData 문자열
	*/
	std::string		urlencode( const std::string& sData );
	/** 
		입력한 URL 인코딩 문자열을 문자열로 변환하여 반환한다.
		@param sData URL 인코딩 문자열
	*/
	std::string		urldecode( const std::string& sData );
	/** 
		문자열을 분리자로 분석하여 분리한다.
		@param sLine 문자열
		@param sList 분리된 문자열을 저장할 리스트
		@param cSep 분리자
	*/
	size_t			tokenizer( const std::string& sLine, stringlist &sList, const char cSep );
	/** 
		문자열을 분리자로 분석하여 분리한다.
		@param sLine 문자열
		@param sList 분리된 문자열을 저장할 백터
		@param cSep 분리자
	*/
	size_t			tokenizer( const std::string& sLine, stringvector &sList, const char cSep );
	/** 
		입력한 문자열을 치환한다.
		@param sFind 검색할 문자열
		@param sRep 치환할 문자열
		@param sSrc 원본 문자열
	*/
	bool			replace(const std::string sFind, const std::string& sRep, std::string& sSrc );
	/** 
		입력한 문자열을 치환한다.
		@param sFind 검색할 문자열
		@param sRep 치환할 문자열
		@param sSrc 원본 문자열
		@param iOffset 문자열을 검색하는 위치
	*/
	size_t			replace(const std::string sFind, const std::string& sRep, std::string& sSrc, size_t iOffset );
	/** 
		입력한 문자열을 오른쪽 정렬한 결과를 반환한다.
		@param s 정렬할 문자열
		@param w 정렬한 넓이
	*/
	std::string		rpad(const std::string& s, int w);
	/** 
		입력한 문자열을 왼쪽 정렬한 결과를 반환한다.
		@param s 정렬할 문자열
		@param w 정렬한 넓이
	*/
	std::string		lpad(const std::string s, int w);
	/** 
		입력한 short 값를 문자열로 변환한다.
		@param i short값
	*/
	std::string		intstr(short i);
	/** 
		입력한 unsigned short 값를 문자열로 변환한다.
		@param i unsigned short값
	*/
	std::string		intstr(unsigned short i);
	/** 
		입력한 int 값를 문자열로 변환한다.
		@param i int값
	*/
	std::string		intstr(int i);
	/** 
		입력한 unsigned int 값를 문자열로 변환한다.
		@param i unsigned int값
	*/
	std::string		intstr(unsigned int i);
	/** 
		입력한 __int64 값를 문자열로 변환한다.
		@param i __int64 값
	*/
	std::string		intstr(__int64 i);
	/** 
		입력한 double 값를 문자열로 변환한다.
		@param i double 값
	*/
	std::string		intstr(double i);
	/** 
		입력한 float 값를 문자열로 변환한다.
		@param i float 값
	*/
	std::string		intstr(float i);
	/** 
		입력한 문자열이 숫자인지 조사한다.
		@param s 문자열
	*/
	bool			isnum( const std::string& s );
	/** 
		입력한 문자열이 가운데 정렬한다.
		@param s 문자열
		@param size 넓이
		@param f 빈공간에 채울 문자
	*/
	std::string		center(const std::string& s, int size, char f = ' ');
	/** 
		입력한 문자열에 escape 문자가 있는지 조사한다.
		@param s 문자열
	*/
	bool			escape(const std::string& s);
	/**
		입력한 문자열에 분리하여 vCPUs 에 순차적으로 채운다.
		@param sAffinity Affinity 문자열 ( 예: 1,2,3 )
		@param iCount vCPUs 에 채워질 값의 개수
		@param vCPUs 결과값
	*/
	void			parse_affinity( const std::string& sAffinity, int32 iCount, std::vector<int32>& vCPUs );
	/**
		입력한 ansi 문자열을 멀티바이트캐릭터로 변환한다.
		@param sData 문자열
	*/
	std::wstring	ansi_to_wide(const_string & sData);
	/**
		입력한 멀티바이트캐릭터를 문자열을 ansi 캐릭터로 변환한다.
		@param sData 문자열
	*/
	std::string		wide_to_ansi(const std::wstring & sData);
	/**
		입력한 ansi 문자열을 utf8 캐릭터로 변환한다.
		@param sData 문자열
	*/
	std::string		ansi_to_utf8( const_string & sData );
	/**
		입력한 utf8 문자열을 ansi 캐릭터로 변환한다.
		@param sData 문자열
	*/
	std::string		utf8_to_ansi( const_string & sData );
	/**
		입력한 경로명에서 파일이름을 얻어온다.
		@param sData 문자열
	*/
	std::string		getfilename( const std::string& _path );
	/**
		입력한 경로명에서 확장자를 제외한 프로세스 이름을 얻어온다.
		@param sData 문자열
	*/
	std::string		getprocname( const std::string& _path );
	/**
		입력한 경로명에서 실행파일명을 제외한 경로를 얻어온다. 
		@param sData 문자열
	*/
	std::string		getworkpath( const std::string& _path );
};

