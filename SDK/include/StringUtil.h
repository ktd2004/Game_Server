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

///< string list Ÿ�� ����
typedef std::list<std::string> stringlist;

/** 
	��ƿ��Ƽ ������ ���� ���ӽ����̽�.
*/
namespace util
{
	/** 
		�Է��� ���ڿ��� �ҹ��ڷ� �����Ѵ�.
		@param rData ���ڿ�
	*/
	std::string		lower( const std::string& rData );
	/** 
		�Է��� ���ڿ��� �ҹ��ڷ� �����Ѵ�.
		@param rData ���ڿ�
	*/
	std::wstring	lower( const std::wstring& rData );
	/** 
		�Է��� ���ڿ��� �빮�ڷ� �����Ѵ�.
		@param rData ���ڿ�
	*/
	std::wstring	upper( const std::wstring& rData );
	/** 
		�Է��� ���ڿ��� �빮�ڷ� �����Ѵ�.

		@param rData ���ڿ�
	*/
	std::string		upper( const std::string& rData );
	/** 
		�Է��� ���ڿ��� ��,�ڿ� ���� �� �� ���ڸ� �����Ѵ�.
		@param rData ���ڿ�
	*/
	std::string		trim( const std::string& rData );
	/** 
		�Է��� ���ڿ��� �и��ڷ� �����Ͽ� Ű(key) ���ڿ��� ���´�.
		@param s ���ڿ�
		@param v ���ڿ� �и���(��: key=valule)

		@verbatim
		std::string s = "socketaddr=127.0.0.3";
		printf( "key �� %s \n", util::pairkey(s, "=").c_str() );
		@endverbatim
	*/
	std::string		pairkey( const std::string s, const std::string v = "=" );
	/** 
		�Է��� ���ڿ��� �и��ڷ� �����Ͽ� ��(value) ���ڿ��� ���´�.
		@param s ���ڿ�
		@param v ���ڿ� �и���(��: key=valule)

		@verbatim
		std::string s = "socketaddr=127.0.0.3";
		printf( "value ���� %s \n", util::pairval(s, "=").c_str() );
		@endverbatim
	*/
	std::string		pairval( const std::string s, const std::string v = "=" );
	/** 
		�Է��� ���ڸ� hexa ���ڿ��� �����Ѵ�.
		@param ch ����
		@param bUpperCase hexa ���ڿ��� �빮�ڷ� ��ȯ���� ����
	*/
	std::string		chartohex( unsigned char ch, bool bUpperCase = false );
	/** 
		hexa���ڿ��� ���ڷ� ��ȯ�Ѵ�.
		@param pData hexa ���ڿ�
	*/
	unsigned char	hextochar( const char *pData );
	/** 
		�Է��� hexa���ڿ��� ���ڷ� ��ȯ�Ѵ�.
		@param pData hexa ���ڿ�
	*/
	unsigned char	hextochar( const std::string& sData );
	/** 
		���ڿ��� hexa ���ڿ��� �����Ѵ�.
		@param sData ���ڿ�
	*/
	std::string		tohexstring( const std::string& sData );
	/** 
		�Է��� ���ڿ��� URL ���ڵ� ���ڿ��� ��ȯ�Ͽ� ��ȯ�Ѵ�.
		@param sData ���ڿ�
	*/
	std::string		urlencode( const std::string& sData );
	/** 
		�Է��� URL ���ڵ� ���ڿ��� ���ڿ��� ��ȯ�Ͽ� ��ȯ�Ѵ�.
		@param sData URL ���ڵ� ���ڿ�
	*/
	std::string		urldecode( const std::string& sData );
	/** 
		���ڿ��� �и��ڷ� �м��Ͽ� �и��Ѵ�.
		@param sLine ���ڿ�
		@param sList �и��� ���ڿ��� ������ ����Ʈ
		@param cSep �и���
	*/
	size_t			tokenizer( const std::string& sLine, stringlist &sList, const char cSep );
	/** 
		���ڿ��� �и��ڷ� �м��Ͽ� �и��Ѵ�.
		@param sLine ���ڿ�
		@param sList �и��� ���ڿ��� ������ ����
		@param cSep �и���
	*/
	size_t			tokenizer( const std::string& sLine, stringvector &sList, const char cSep );
	/** 
		�Է��� ���ڿ��� ġȯ�Ѵ�.
		@param sFind �˻��� ���ڿ�
		@param sRep ġȯ�� ���ڿ�
		@param sSrc ���� ���ڿ�
	*/
	bool			replace(const std::string sFind, const std::string& sRep, std::string& sSrc );
	/** 
		�Է��� ���ڿ��� ġȯ�Ѵ�.
		@param sFind �˻��� ���ڿ�
		@param sRep ġȯ�� ���ڿ�
		@param sSrc ���� ���ڿ�
		@param iOffset ���ڿ��� �˻��ϴ� ��ġ
	*/
	size_t			replace(const std::string sFind, const std::string& sRep, std::string& sSrc, size_t iOffset );
	/** 
		�Է��� ���ڿ��� ������ ������ ����� ��ȯ�Ѵ�.
		@param s ������ ���ڿ�
		@param w ������ ����
	*/
	std::string		rpad(const std::string& s, int w);
	/** 
		�Է��� ���ڿ��� ���� ������ ����� ��ȯ�Ѵ�.
		@param s ������ ���ڿ�
		@param w ������ ����
	*/
	std::string		lpad(const std::string s, int w);
	/** 
		�Է��� short ���� ���ڿ��� ��ȯ�Ѵ�.
		@param i short��
	*/
	std::string		intstr(short i);
	/** 
		�Է��� unsigned short ���� ���ڿ��� ��ȯ�Ѵ�.
		@param i unsigned short��
	*/
	std::string		intstr(unsigned short i);
	/** 
		�Է��� int ���� ���ڿ��� ��ȯ�Ѵ�.
		@param i int��
	*/
	std::string		intstr(int i);
	/** 
		�Է��� unsigned int ���� ���ڿ��� ��ȯ�Ѵ�.
		@param i unsigned int��
	*/
	std::string		intstr(unsigned int i);
	/** 
		�Է��� __int64 ���� ���ڿ��� ��ȯ�Ѵ�.
		@param i __int64 ��
	*/
	std::string		intstr(__int64 i);
	/** 
		�Է��� double ���� ���ڿ��� ��ȯ�Ѵ�.
		@param i double ��
	*/
	std::string		intstr(double i);
	/** 
		�Է��� float ���� ���ڿ��� ��ȯ�Ѵ�.
		@param i float ��
	*/
	std::string		intstr(float i);
	/** 
		�Է��� ���ڿ��� �������� �����Ѵ�.
		@param s ���ڿ�
	*/
	bool			isnum( const std::string& s );
	/** 
		�Է��� ���ڿ��� ��� �����Ѵ�.
		@param s ���ڿ�
		@param size ����
		@param f ������� ä�� ����
	*/
	std::string		center(const std::string& s, int size, char f = ' ');
	/** 
		�Է��� ���ڿ��� escape ���ڰ� �ִ��� �����Ѵ�.
		@param s ���ڿ�
	*/
	bool			escape(const std::string& s);
	/**
		�Է��� ���ڿ��� �и��Ͽ� vCPUs �� ���������� ä���.
		@param sAffinity Affinity ���ڿ� ( ��: 1,2,3 )
		@param iCount vCPUs �� ä���� ���� ����
		@param vCPUs �����
	*/
	void			parse_affinity( const std::string& sAffinity, int32 iCount, std::vector<int32>& vCPUs );
	/**
		�Է��� ansi ���ڿ��� ��Ƽ����Ʈĳ���ͷ� ��ȯ�Ѵ�.
		@param sData ���ڿ�
	*/
	std::wstring	ansi_to_wide(const_string & sData);
	/**
		�Է��� ��Ƽ����Ʈĳ���͸� ���ڿ��� ansi ĳ���ͷ� ��ȯ�Ѵ�.
		@param sData ���ڿ�
	*/
	std::string		wide_to_ansi(const std::wstring & sData);
	/**
		�Է��� ansi ���ڿ��� utf8 ĳ���ͷ� ��ȯ�Ѵ�.
		@param sData ���ڿ�
	*/
	std::string		ansi_to_utf8( const_string & sData );
	/**
		�Է��� utf8 ���ڿ��� ansi ĳ���ͷ� ��ȯ�Ѵ�.
		@param sData ���ڿ�
	*/
	std::string		utf8_to_ansi( const_string & sData );
	/**
		�Է��� ��θ��� �����̸��� ���´�.
		@param sData ���ڿ�
	*/
	std::string		getfilename( const std::string& _path );
	/**
		�Է��� ��θ��� Ȯ���ڸ� ������ ���μ��� �̸��� ���´�.
		@param sData ���ڿ�
	*/
	std::string		getprocname( const std::string& _path );
	/**
		�Է��� ��θ��� �������ϸ��� ������ ��θ� ���´�. 
		@param sData ���ڿ�
	*/
	std::string		getworkpath( const std::string& _path );
};

