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
//<
#include <Buffer.h>

/** 
	무잠금 Buffer 큐 클래스.

	전송할 패킷을 저장하는 대기열로 사용된다.
*/
class lock_free_queue
{
	char			m_head[sizeof(Buffer)];
	Buffer*			m_tail;
	volatile long	m_sending;

public:
	lock_free_queue();
	virtual ~lock_free_queue();

	bool		is_sending( void );
	void		off_sending( void );
	void		push( Buffer* data );
	Buffer*		pop( void );
	void		clear( void );
};