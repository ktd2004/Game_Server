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

/** 
	MD5 해쉬 결과를 저장하는 구조체.

	Md5Hash 클래스로 문자열, 파일, 버퍼에 저장된 데이타를 MD5 해쉬한 결과값을 저장한다.
*/
struct Md5
{
	unsigned char m_Data[16];		///< MD5 해쉬값

	Md5();
	/** MD5 해쉬값을 16진수 문자열로 변환한다 */
	std::string ToString() const;
	/**  MD5 해쉬문자열로 부터 MD5 해쉬값 생성한다 */
	bool		FromString( const std::string& sData );
	/** 비교 연산자 */
	bool operator == (const Md5 & r) const;
	/** 비교 연산자 */
	bool operator != (const Md5 & r) const;
};

/** 
	MD5 해쉬값을 계산하기 위해 Md5Hash 클래스 내부에서 사용되는 구조체
*/
typedef struct
{
	ULONG			i[2];
	ULONG			buf[4];
	unsigned char	in[64];
	unsigned char	digest[16];
} MD5_CTX;

/** 
	문자열, 파일, 버퍼에 저장된 데이타로 부터 MD5 해쉬값을 추출한다

	Md5Hash는 windows 에서 제공하는 MD5 해쉬 함수를 호출하여 해쉬값을 계산한다.	추출된 해쉬값은 16진수 형태의 문자열 혹은 16 bytes의
	binary 값으로도 참조 가능하다. 아래는 문자열을 해쉬값으로 변환하는 방법을 보여준다.

	@verbatim
	Md5Hash hasher;

	char szBuf[2048];
	strcpy( szBuf, "this is test data" );

	hasher.Append( szBuf, strlen(szBuf) );
	Md5 rst = hasher.Finish();
	printf( "hash val = %s\n", Md5.ToString().c_str() );

	// 위 방법을 좀더 간단히 하면.
	printf( "hash val = %s\n", Md5Hash::FromData( "this is test data" ).c_str() );

	// 파일의 MD5 해쉬 문자열를 추출하려면
	printf( "hash val = %s\n", Md5Hash::FromFile( "ex3.exe" ).c_str() );

	// 문자열해쉬 값을 추출하려면
	printf( "hash digit = %u\n", Md5Hash::ToHash( "this is test data" ) );
	@endverbatim
*/
class Md5Hash
{
private:
	MD5_CTX			m_Ctx;

public:
	Md5Hash();
	virtual ~Md5Hash();

	/** 
		MD5 해쉬값을 추출할 데이타를 추가한다.
		@param pBuf 입력할 데이타가 저장된 주소값
		@param iLen 데이타의 길이
	*/
	void				Append( const void * pBuf, size_t iLen );
	/** 
		Append 함수로 추가한 데이타로 부터 MD5 해쉬값을 계산한다.
	*/
	Md5					Finish( void );
	/** 
		파일에 저장된 전체 데이타로 부터 MD5해쉬값 생성한다.
		@param sFileName 파일 경로
	*/
	static Md5			FromFile( const std::string& sFileName );
	/** 
		데이타의 MD5해쉬값을 생성한다.
		@param sData 데이타
	*/
	static Md5			FromData( const std::string& sData );
	/** 
		문자열로 부터 해쉬값을 생성한다.
		@param sData 데이타
	*/
	static unsigned int ToHash( const std::string& sData );
};