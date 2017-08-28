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
	zlib 버퍼의 크기를 정의한 상수
*/
enum eZBuffer
{
	eZBUFF_SIZE = 1024 * 1024 * 5,		///< zlib 버퍼를 5kbytes 로 정의
};

/**
	버퍼에 저장된 데이타를 압축하는 클래스.

	쓰레드마다 별도의 ZipBuffer 오브젝트를 생성하여 쓰레드간의 동기화 없이 사용이 가능하다.
*/
class ZipBuffer
{
	static __declspec(thread) ZipBuffer * m_This;

	/**
		zlib에서 사용할 버퍼 구조체
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
	
	zipbuf              m_zipbuf;		///< zlib 에서 사용할 버퍼

	ZipBuffer();
	ZipBuffer(const ZipBuffer& r);
	virtual ~ZipBuffer();

	/** 
		압축된 데이타를 저장할 버퍼를 할당
	*/
	void*         Malloc();
public:
	static ZipBuffer* instance();
	/** 
		버퍼를 압축
		@param dstSize 압축된 데이타의 크기
		@param srcBuf 압축할 데이타를 저장한 버퍼의 주소값
		@param srcSize 압축할 데이타의 크기
	*/
	const void*   Flate( ulong32& dstSize, void* srcBuf, ulong32 srcSize );
	/** 
		버퍼를 압축
		@param dstBuf 압축된 데이타를 저장할 버퍼의 주소값
		@param dstSize 압축된 데이타의 크기
		@param srcBuf 압축할 데이타를 저장한 버퍼의 주소값
		@param srcSize 압축할 데이타의 크기
	*/
	int32         Flate( void* dstBuf, ulong32& dstSize, void* srcBuf, ulong32 srcSize );
	/** 
		버퍼를 압축해제
		@param dstSize 압축해제된 데이타의 크기
		@param srcBuf 압축해제할 데이타를 저장한 버퍼의 주소값
		@param srcSize 압축해제할 데이타의 크기
	*/
	const void*   Deflate( ulong32& dstSize, void* srcBuf, ulong32 srcSize );
	/** 
		버퍼를 압축해제
		@param dstBuf 압축해제된 데이타를 저장할 버퍼의 주소값
		@param dstSize 압축해제된 데이타의 크기
		@param srcBuf 압축해제할 데이타를 저장한 버퍼의 주소값
		@param srcSize 압축해제할 데이타의 크기
	*/
	int32         Deflate( void* dstBuf, ulong32& dstSize, void* srcBuf, ulong32 srcSize );
};

#define COMPRESSOR	ZipBuffer::instance()