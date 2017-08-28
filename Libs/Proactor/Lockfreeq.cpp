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
		// tail �� ���� ������ �ٽ� tail->m_next �� �̵��Ͽ� �ٽ� push �õ�
		if (oldTailNext != nullptr)
		{
			locked_cmp_exchange_pointer( m_tail, oldTailNext, oldTail );
			continue;
		}
		// tail->m_next �� null �̸� push �Ϸ�.
		if (locked_cmp_exchange_pointer(oldTail->m_next, data, nullptr) == nullptr)
		{
			// tail �� ������ ��ҷ� ����
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
		// ����Ʈ�� �ʱ�ȭ�Ѵ�.
		//if( locked_cmp_exchange_pointer(m_head.m_next, NULL, oldHeadNext) == oldHeadNext )
		if (locked_cmp_exchange_pointer(((Buffer*)m_head)->m_next, nullptr, oldHeadNext) == oldHeadNext)
		{
			// �ٸ� �����忡�� tail �� ������ �ְ� �������� ������ �������. tail�� ����� �����.
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