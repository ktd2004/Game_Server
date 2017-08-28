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
#include "Buffer.h"
#include <Compile.h>
#include <Performance.h>

namespace
{
	Proactor_Counter& g_proactor_counter = PROACTOR_COUNTER;
}

Buffer::Buffer()
: m_ibytes(2048)
, m_begin(nullptr)
, m_front(nullptr)
, m_back(nullptr)
, m_end(nullptr)
{
	this->incref();
	resize( m_ibytes );
	m_head = rd_ptr();
	locked_inc( g_proactor_counter.iBuffers );
}

Buffer::Buffer( size_t ibytes )
: m_ibytes(ibytes)
, m_begin(nullptr)
, m_front(nullptr)
, m_back(nullptr)
, m_end(nullptr)
{
	this->incref();
	resize( m_ibytes );
	m_head = rd_ptr();
	locked_inc( g_proactor_counter.iBuffers );
}

Buffer::Buffer( const Buffer &ref )
: m_ibytes(ref.m_ibytes)
, m_begin(nullptr)
, m_front(nullptr)
, m_back(nullptr)
, m_end(nullptr)
{
	this->incref();
	Buffer &r = const_cast<Buffer &>(ref);
	resize( r.size() );
	copy( r.rd_ptr(), r.length() );
	m_head = rd_ptr();
	locked_inc( g_proactor_counter.iBuffers );
}

Buffer::Buffer( const char *pBuf, size_t ibytes )
: m_ibytes(ibytes)
, m_begin(nullptr)
, m_front(nullptr)
, m_back(nullptr)
, m_end(nullptr)
{
	this->incref();
	resize( ibytes );
	copy( pBuf, ibytes );
	m_head = rd_ptr();
	locked_inc( g_proactor_counter.iBuffers );
}

Buffer::~Buffer()
{
	safe_array_delete(m_begin);
	m_begin = m_front = m_back = m_end = 0;
	m_ibytes = 0;
	locked_dec( g_proactor_counter.iBuffers );
}

void Buffer::resize( size_t ibytes )
{
	char *pBuf = nullptr;
	// 가장 큰값을 추려내서..
	size_t ilen = length();
	size_t icap = capacity();
	ibytes = std::max<size_t>(ibytes, std::max<size_t>(ilen, icap));

	// 버퍼가 할당되지 않았으면 메모리 할당
	if ( !m_begin )
	{
		pBuf = (char*)new char[ibytes];
		pBuf[0] = 0;
		m_begin = pBuf;
		m_front = m_begin;
		m_back = m_begin;
		m_end = m_begin + ibytes;
		m_ibytes = ibytes;
	}
	// Resize 요청이 현재 할당된 저장용량 보다 크면 재할당후 데이타 복사
	else if ( icap < ibytes )
	{
		pBuf = new char[ibytes];
		memcpy( pBuf, m_front, ilen );
		pBuf[ilen] = 0;
		safe_array_delete( m_begin );

		m_begin = pBuf;
		m_front = m_begin;
		m_back = m_begin + ilen;
		m_end = m_begin + ibytes;
		m_ibytes = ibytes;
	}
}

void Buffer::crunch()
{
	if ( m_front != m_begin )
	{
		size_t iDataLen = length();
		memmove( m_begin, m_front, iDataLen );
		m_front = m_begin;
		m_back = m_front + iDataLen;
		m_end = m_begin + capacity();
	}
}

char* Buffer::reserve( size_t ibytes )
{
	m_head = wr_ptr();
	wr_ptr( ibytes );
	return m_head;
}

size_t Buffer::copy( const char* pBuf, size_t ibytes )
{
	if ( ibytes > space() )
		resize( capacity() + ibytes );

	memcpy( m_back, pBuf, ibytes );
	m_back += ibytes;
	return ibytes;
}

char& Buffer::operator[]( size_t i )
{
	return rd_ptr()[i];
}

size_t Buffer::format( const char *pFmt, ... )
{
	va_list args;
	va_start( args, pFmt );
	size_t ibytes = vsnprintf_s( wr_ptr(), space(), _TRUNCATE, pFmt, args);
	wr_ptr( ibytes );
	return ibytes;
}

Buffer& Buffer::operator<<(const std::string& r)
{
	*this << (unsigned int)r.size();
	this->copy(r.c_str(), r.size());
	return *this;
}

Buffer& Buffer::operator<<(const char* pszBuf)
{
	unsigned int ibytes = (unsigned int)::strlen(pszBuf);
	*this << ibytes;
	copy(pszBuf, ibytes);
	return *this;
}

Buffer& Buffer::operator<<(ptr_t p)
{
	this->copy((const char*)&p, sizeof(ptr_t));
	return *this;
}

Buffer& Buffer::operator<<(const Buffer* p)
{
	Buffer* r = const_cast<Buffer *>(p);
	copy(r->rd_ptr(), r->length());
	return *this;
}

Buffer& Buffer::operator<<(const Buffer& p)
{
	Buffer& r = const_cast<Buffer &>(p);
	copy(r.rd_ptr(), r.length());
	return *this;
}

Buffer& Buffer::operator>>(std::string& r)
{
	unsigned int ibytes = 0; *this >> ibytes;
	if (ibytes > (unsigned int)length())
	{
		return *this;
	}
	r.assign(rd_ptr(), ibytes);
	this->rd_ptr(ibytes);
	return *this;
}

Buffer& Buffer::operator>>(char* pszBuf)
{
	unsigned int ibytes = 0; *this >> ibytes;
	if (ibytes > (unsigned int)length())
	{
		return *this;
	}
	memcpy(pszBuf, this->rd_ptr(), ibytes);
	this->rd_ptr(ibytes);
	return *this;
}

Buffer& Buffer::operator>>(Buffer* p)
{
	p->copy(this->rd_ptr(), this->length());
	this->rd_ptr(this->length());
	return *this;
}

Buffer& Buffer::operator>>(Buffer& p)
{
	p.copy(this->rd_ptr(), this->length());
	this->rd_ptr(this->length());
	return *this;
}

void Buffer::release(void)
{
	if (this->decref() == 0)
	{
		delete this;
	}
}

char* Buffer::base(void)
{
	return m_begin;
}

void Buffer::reset(void)
{
	m_front = m_back = m_begin;
}

char* Buffer::rd_ptr(void)
{
	return m_front;
}

char* Buffer::rd_ptr(size_t ipos)
{
	return rd_ptr(m_front + ipos);
}

char* Buffer::rd_ptr(char* pos)
{
	if (pos > m_end)
	{
		m_front = m_end;
		m_back = m_end;
	}
	else if (pos > m_back)
	{
		m_front = m_back;
	}
	else if (pos < m_begin)
	{
		m_front = m_begin;
	}
	else
	{
		m_front = pos;
	}

	return m_front;
}

char* Buffer::wr_ptr(void)
{
	return m_back;
}

char* Buffer::wr_ptr(size_t ipos)
{
	return wr_ptr(m_back + ipos);
}

char* Buffer::wr_ptr(char* pos)
{
	if (pos > m_end)
	{
		m_back = m_end;
	}
	else if (pos < m_begin)
	{
		m_front = m_begin;
		m_back = m_begin;
	}
	else if (pos < m_front)
	{
		m_front = pos;
		m_back = m_front;
	}
	else
	{
		m_back = pos;
	}

	return m_back;
}

size_t Buffer::length(void)
{
	return m_back - m_front;
}

size_t Buffer::size(void)
{
	return m_back - m_front;
}

size_t Buffer::capacity(void)
{
	return m_ibytes;
}

size_t Buffer::space(void)
{
	return m_end - m_back;
}
