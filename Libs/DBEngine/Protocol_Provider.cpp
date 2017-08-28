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
#include <Protocol_Provider.h>

Buffer& operator<< (Buffer& pk, const ExecuteQueryHeader& r)
{
	pk.copy( (const char*)&r, sizeof(ExecuteQueryHeader) );
	return pk;
}

Buffer& operator>> (Buffer& pk, ExecuteQueryHeader& r)
{
	::memcpy( &r, pk.rd_ptr(), sizeof(ExecuteQueryHeader) );
	pk.rd_ptr(sizeof(ExecuteQueryHeader));
	return pk;
}

//////////////////////////////////////////////////////////////////////////

Buffer& operator<< (Buffer& pk, const QueryParamsHeader& r)
{
	pk.copy( (const char*)&r, sizeof(QueryParamsHeader) );
	return pk;
}

Buffer& operator>> (Buffer& pk, QueryParamsHeader& r)
{
	::memcpy( &r, pk.rd_ptr(), sizeof(QueryParamsHeader) );
	pk.rd_ptr( sizeof(QueryParamsHeader) );
	return pk;
}

//////////////////////////////////////////////////////////////////////////

Buffer& operator<< (Buffer& pk, const QueryResultHeader& r)
{
	pk.copy( (const char*)&r, sizeof(QueryResultHeader) );
	return pk;
}

Buffer& operator>> (Buffer& pk, QueryResultHeader& r)
{
	::memcpy( &r, pk.rd_ptr(), sizeof(QueryResultHeader) );
	pk.rd_ptr( sizeof(QueryResultHeader) );
	return pk;
}