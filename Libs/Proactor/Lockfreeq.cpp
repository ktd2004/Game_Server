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
#include "Lockfreeq.h"

lock_free_queue::lock_free_queue()
	: m_tail(nullptr)
	, m_sending(0)
//	, m_head(1)
{
	//m_tail = &m_head;
	memset( m_head, 0x00, sizeof(m_head) );
	m_tail = (Buffer*)m_head;
}

lock_free_queue::~lock_free_queue()
{
	clear();
}

bool lock_free_queue::is_sending( void )
{
	return 0 != locked_compare_exchange(m_sending, 1, 0);
}

void lock_free_queue::off_sending( void )
{
	locked_compare_exchange(m_sending, 0, 1);
}

void lock_free_queue::push( Buffer* data )
{
	while( true )
	{
		Buffer* oldTail = m_tail;
		if( oldTail != m_tail )
			continue;

		Buffer* oldTailNext = oldTail->m_next;
		// tail 이 변경 됬으면 다시 tail->m_next 로 이동하여 다시 push 시도
		if (oldTailNext != nullptr)
		{
			locked_cmp_exchange_pointer( m_tail, oldTailNext, oldTail );
			continue;
		}
		// tail->m_next 가 null 이면 push 완료.
		if (locked_cmp_exchange_pointer(oldTail->m_next, data, nullptr) == nullptr)
		{
			// tail 을 마지막 요소로 변경
			locked_cmp_exchange_pointer( m_tail, data, oldTail );
			return;
		}
	}
}

Buffer* lock_free_queue::pop( void )
{
//	Buffer* oldHead = &m_head;
	Buffer* oldHead = (Buffer*)m_head;
	while( true )
	{
		Buffer* oldTail = m_tail;
		Buffer* oldHeadNext = oldHead->m_next;
		if (oldHeadNext == nullptr)
		{
			return nullptr;
		}
		// 리스트를 초기화한다.
		//if( locked_cmp_exchange_pointer(m_head.m_next, NULL, oldHeadNext) == oldHeadNext )
		if (locked_cmp_exchange_pointer(((Buffer*)m_head)->m_next, nullptr, oldHeadNext) == oldHeadNext)
		{
			// 다른 쓰레드에서 tail 에 뭔가를 넣고 있을지도 모르지만 상관없다. tail을 헤더로 맞춘다.
			locked_exchange_pointer(m_tail, &m_head);
			return oldHeadNext;
		}
	}
}

void lock_free_queue::clear( void )
{
	Buffer* data = pop();
	while ( data )
	{
		Buffer* t = data;
		data = data->m_next;
//		delete t;
		t->release();
	}
}