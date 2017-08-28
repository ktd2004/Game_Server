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
#include "TCPLink.h"

//<
class IOContext
{
public:
	enum eSize
	{
		eMaxBufferSize = 4096,
		eAcceptBufferSize = (sizeof(sockaddr_in)+16)*2,
	};

private:
	IOContext( const IOContext& r ) {}

public:
	OVERLAPPED		m_OV;		///< OVERLAPPED 정보

public:
	IOContext();
	virtual ~IOContext();
	virtual	void		OnHandle( DWORD dwTransferredBytes, DWORD err ) = 0;

	//< OVERLAPPED*를 IOContext*로 주소 변환
	static IOContext*	Cast( LPOVERLAPPED pOV );
};
