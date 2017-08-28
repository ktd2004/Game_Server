#include "stdafx.h"
#include <ZipBuffer.h>
#include <zlib.h>

ZipBuffer * ZipBuffer::m_This = nullptr;

ZipBuffer* ZipBuffer::instance()
{
	if (m_This == nullptr)
	{
		m_This = new ZipBuffer;
	}

	return m_This;
}

ZipBuffer::ZipBuffer()
{
}

ZipBuffer::ZipBuffer( const ZipBuffer& r )
{

}

ZipBuffer::~ZipBuffer()
{
}

void* ZipBuffer::Malloc()
{
	if ( !m_zipbuf.p )
	{
		m_zipbuf.p    = malloc(eZBUFF_SIZE + 512);
		m_zipbuf.size = eZBUFF_SIZE;
	}
	return m_zipbuf.p;
}


const void* ZipBuffer::Flate( ulong32& dstSize, void* srcBuf, ulong32 srcSize )
{
	Bytef* pBuf = (Bytef*)Malloc();
	if (Z_OK != ::compress2(pBuf, &dstSize, (Bytef*)srcBuf, (uLongf)srcSize, 5))
	{
		return nullptr;
	}
	return pBuf;
}

int32 ZipBuffer::Flate( void* dstBuf, ulong32& dstSize, void* srcBuf, ulong32 srcSize )
{
	if (Z_OK != ::compress2((Bytef*)dstBuf, (uLongf*)&dstSize, (Bytef*)srcBuf, (uLongf)srcSize, 5))
	{
		return 0;
	}
	return dstSize;
}

const void* ZipBuffer::Deflate( ulong32& dstSize, void* srcBuf, ulong32 srcSize )
{
	dstSize = eZBUFF_SIZE;
	Bytef* pBuf = (Bytef*)Malloc();
	if ( Z_OK != ::uncompress(pBuf, &dstSize, (Bytef*)srcBuf, (uLongf)srcSize) )
	{
		return nullptr;
	}
	return pBuf;
}

int32 ZipBuffer::Deflate( void* dstBuf, ulong32& dstSize, void* srcBuf, ulong32 srcSize )
{
	if ( Z_OK != ::uncompress((Bytef*)dstBuf, (uLongf*)&dstSize, (Bytef*)srcBuf, (uLongf)srcSize) )
	{
		return 0;
	}
	return dstSize;
}