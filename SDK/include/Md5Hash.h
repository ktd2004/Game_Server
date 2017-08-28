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
	MD5 �ؽ� ����� �����ϴ� ����ü.

	Md5Hash Ŭ������ ���ڿ�, ����, ���ۿ� ����� ����Ÿ�� MD5 �ؽ��� ������� �����Ѵ�.
*/
struct Md5
{
	unsigned char m_Data[16];		///< MD5 �ؽ���

	Md5();
	/** MD5 �ؽ����� 16���� ���ڿ��� ��ȯ�Ѵ� */
	std::string ToString() const;
	/**  MD5 �ؽ����ڿ��� ���� MD5 �ؽ��� �����Ѵ� */
	bool		FromString( const std::string& sData );
	/** �� ������ */
	bool operator == (const Md5 & r) const;
	/** �� ������ */
	bool operator != (const Md5 & r) const;
};

/** 
	MD5 �ؽ����� ����ϱ� ���� Md5Hash Ŭ���� ���ο��� ���Ǵ� ����ü
*/
typedef struct
{
	ULONG			i[2];
	ULONG			buf[4];
	unsigned char	in[64];
	unsigned char	digest[16];
} MD5_CTX;

/** 
	���ڿ�, ����, ���ۿ� ����� ����Ÿ�� ���� MD5 �ؽ����� �����Ѵ�

	Md5Hash�� windows ���� �����ϴ� MD5 �ؽ� �Լ��� ȣ���Ͽ� �ؽ����� ����Ѵ�.	����� �ؽ����� 16���� ������ ���ڿ� Ȥ�� 16 bytes��
	binary �����ε� ���� �����ϴ�. �Ʒ��� ���ڿ��� �ؽ������� ��ȯ�ϴ� ����� �����ش�.

	@verbatim
	Md5Hash hasher;

	char szBuf[2048];
	strcpy( szBuf, "this is test data" );

	hasher.Append( szBuf, strlen(szBuf) );
	Md5 rst = hasher.Finish();
	printf( "hash val = %s\n", Md5.ToString().c_str() );

	// �� ����� ���� ������ �ϸ�.
	printf( "hash val = %s\n", Md5Hash::FromData( "this is test data" ).c_str() );

	// ������ MD5 �ؽ� ���ڿ��� �����Ϸ���
	printf( "hash val = %s\n", Md5Hash::FromFile( "ex3.exe" ).c_str() );

	// ���ڿ��ؽ� ���� �����Ϸ���
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
		MD5 �ؽ����� ������ ����Ÿ�� �߰��Ѵ�.
		@param pBuf �Է��� ����Ÿ�� ����� �ּҰ�
		@param iLen ����Ÿ�� ����
	*/
	void				Append( const void * pBuf, size_t iLen );
	/** 
		Append �Լ��� �߰��� ����Ÿ�� ���� MD5 �ؽ����� ����Ѵ�.
	*/
	Md5					Finish( void );
	/** 
		���Ͽ� ����� ��ü ����Ÿ�� ���� MD5�ؽ��� �����Ѵ�.
		@param sFileName ���� ���
	*/
	static Md5			FromFile( const std::string& sFileName );
	/** 
		����Ÿ�� MD5�ؽ����� �����Ѵ�.
		@param sData ����Ÿ
	*/
	static Md5			FromData( const std::string& sData );
	/** 
		���ڿ��� ���� �ؽ����� �����Ѵ�.
		@param sData ����Ÿ
	*/
	static unsigned int ToHash( const std::string& sData );
};