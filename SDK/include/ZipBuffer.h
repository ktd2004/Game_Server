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
	zlib ������ ũ�⸦ ������ ���
*/
enum eZBuffer
{
	eZBUFF_SIZE = 1024 * 1024 * 5,		///< zlib ���۸� 5kbytes �� ����
};

/**
	���ۿ� ����� ����Ÿ�� �����ϴ� Ŭ����.

	�����帶�� ������ ZipBuffer ������Ʈ�� �����Ͽ� �����尣�� ����ȭ ���� ����� �����ϴ�.
*/
class ZipBuffer
{
	static __declspec(thread) ZipBuffer * m_This;

	/**
		zlib���� ����� ���� ����ü
	*/
	struct zipbuf
	{
		void*  p;
		size_t size;
		zipbuf() : p(nullptr), size(0)
		{ 
		}
		~zipbuf()
		{
			::free(p); p = nullptr; size = 0;
		}
	};
	
	zipbuf              m_zipbuf;		///< zlib ���� ����� ����

	ZipBuffer();
	ZipBuffer(const ZipBuffer& r);
	virtual ~ZipBuffer();

	/** 
		����� ����Ÿ�� ������ ���۸� �Ҵ�
	*/
	void*         Malloc();
public:
	static ZipBuffer* instance();
	/** 
		���۸� ����
		@param dstSize ����� ����Ÿ�� ũ��
		@param srcBuf ������ ����Ÿ�� ������ ������ �ּҰ�
		@param srcSize ������ ����Ÿ�� ũ��
	*/
	const void*   Flate( ulong32& dstSize, void* srcBuf, ulong32 srcSize );
	/** 
		���۸� ����
		@param dstBuf ����� ����Ÿ�� ������ ������ �ּҰ�
		@param dstSize ����� ����Ÿ�� ũ��
		@param srcBuf ������ ����Ÿ�� ������ ������ �ּҰ�
		@param srcSize ������ ����Ÿ�� ũ��
	*/
	int32         Flate( void* dstBuf, ulong32& dstSize, void* srcBuf, ulong32 srcSize );
	/** 
		���۸� ��������
		@param dstSize ���������� ����Ÿ�� ũ��
		@param srcBuf ���������� ����Ÿ�� ������ ������ �ּҰ�
		@param srcSize ���������� ����Ÿ�� ũ��
	*/
	const void*   Deflate( ulong32& dstSize, void* srcBuf, ulong32 srcSize );
	/** 
		���۸� ��������
		@param dstBuf ���������� ����Ÿ�� ������ ������ �ּҰ�
		@param dstSize ���������� ����Ÿ�� ũ��
		@param srcBuf ���������� ����Ÿ�� ������ ������ �ּҰ�
		@param srcSize ���������� ����Ÿ�� ũ��
	*/
	int32         Deflate( void* dstBuf, ulong32& dstSize, void* srcBuf, ulong32 srcSize );
};

#define COMPRESSOR	ZipBuffer::instance()