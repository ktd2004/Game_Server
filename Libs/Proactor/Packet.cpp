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
//
#include <Performance.h>
#include <Packet.h>
#include <ProactorFactory.h>
#include <Logger.h>

namespace
{
	Proactor_Counter& g_proactor_counter = PROACTOR_COUNTER;
}

//<
Packet::Packet()
	: m_pBuf(nullptr)
{
}

Packet::Packet(Buffer *pBuf)
{
	m_pBuf = pBuf;
	pBuf->reserve(sizeof(header));
}

Packet::~Packet()
{
}

//< static function
//< 현재 rd_ptr() 위치에서 입출력하는 함수
unsigned int Packet::id( Buffer& rBuf )
{
	return ((header*)rBuf.rd_ptr())->protocol;
}

unsigned int Packet::length( Buffer& rBuf )
{
	return ((header*)rBuf.rd_ptr())->len;
}

Buffer* Packet::split( Buffer& rBuf, size_t iLen )
{
	Buffer*	p = new Buffer(iLen);
	p->copy( rBuf.rd_ptr(), iLen );
	// 읽은 길이만큼 원본에서 제거
	rBuf.rd_ptr( iLen );
	return p;
}


bool Packet::parse( Buffer* pBuf, Buffer::QType& lst, size_t iMaxSize )
{
	size_t iTotalSize = 0;
	while ( (iTotalSize = pBuf->length()) > 0 )
	{
		unsigned int iLen		= Packet::length(*pBuf);
		unsigned int iProtocol	= Packet::id(*pBuf);
		if ( iLen <= 0 || (iMaxSize != 0 && iLen > (unsigned int)iMaxSize) )
		{
			// packet 뭔가 문제가 있다.
			prn_err( "Something is wrong, length(%u), max length(%d)", iLen, iMaxSize );
			locked_inc( g_proactor_counter.iInvalidLengths );
			return false;
		}

		if ( iLen <= iTotalSize )
		{
			Buffer* p = new Buffer(iLen);
			p->copy(pBuf->rd_ptr(), iLen);
			pBuf->rd_ptr(iLen);
			lst.push_back(p);
		}
		else
		{ 
			// 패킷이 부족하게 왔다.
			locked_inc( g_proactor_counter.iInsufficentLengths );
			pBuf->crunch();
			if ( iLen > (unsigned int)pBuf->size() )
			{	// 패킷이 부족하게 왔다 버퍼 capacity 하고 비교해서 버퍼가 부족하면 버퍼를 늘려서 재할당한다.
				pBuf->resize(iLen);
				locked_inc( g_proactor_counter.iResizeBufferLengths );
			}
			return true;
		}
	}

	// 수신한 패킷을 전부 분리했으니까
	pBuf->reset();
	return true;
}

Buffer* Packet::update( Buffer& rBuf )
{
	((header*)rBuf.rd_ptr())->len = (unsigned int)(rBuf.wr_ptr() - rBuf.rd_ptr());
	return &rBuf;
}