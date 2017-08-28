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

// dbengine
#include <DataTypes.h>
#include <DBValue.h>

DBValue::DBValue()
: m_ValueType(vtNULL)
, m_Size(0)
, i64(0)
{}

DBValue::DBValue( const DBValue& v )
	: m_ValueType(vtNULL)
	, m_Size(0)
	, i64(0)
{ 
	m_ValueType = v.m_ValueType;
	ui64 = v.ui64;
	m_sBuf = v.m_sBuf;
	m_Size = v.m_Size;
}

DBValue::~DBValue()
{ Clear(); }

DBValue::DBValue( const std::string& s )
: m_ValueType(vtNULL)
, m_Size(0)
, i64(0)
{
	Clear();
	Assign( s.c_str(), s.length() );
	m_ValueType = vtString;
}

DBValue::DBValue( void* p, size_t t )
: m_ValueType(vtNULL)
, m_Size(0)
, i64(0)
{ 
	Clear();
	Assign( p, t );
	m_ValueType = vtBinary;
}

DBValue::DBValue( const char* p )
: m_ValueType(vtNULL)
, m_Size(0)
, i64(0)
{
	Clear();
	Assign( p, ::strlen(p) );
	m_ValueType = vtString;
}

DBValue& DBValue::Set( const std::string& s )
{
	Clear();
	Assign( s.c_str(), s.length() );
	m_ValueType = vtString;
	return *this;
}

DBValue& DBValue::Set( const void* p, size_t t )
{
	Clear();
	Assign( p, t );
	m_ValueType = vtBinary;
	return *this;
}

DBValue& DBValue::Set( const DBValue& v )
{
	Clear();
	switch (v.m_ValueType) {
	case vtString:
	case vtBinary:
		m_ValueType = v.m_ValueType; m_sBuf = v.m_sBuf;
		break;
	default:
		m_ValueType = v.m_ValueType; ui64 = v.ui64;
		break;
	}
	m_Size = v.m_Size;
	return *this;
}

DBValue& DBValue::Set( const char* p, size_t t )
{
	Clear();
	m_sBuf.assign( p, t );
	m_Size = t;
	m_ValueType = vtString;
	return *this;
}

bool DBValue::IsNull()
{
	return m_ValueType == vtNULL;
}

void* DBValue::Get()
{
	switch (m_ValueType)
	{
	case vtString:
	case vtBinary:
		return (void*)m_sBuf.c_str();
	default:
		return &i64;		// union 으로 되어 있으니까.. 버퍼의 제일 앞을 주면 된다.
	}
}

void* DBValue::Get() const
{
	switch (m_ValueType)
	{
	case vtString:
	case vtBinary:
		return (void*)m_sBuf.c_str();
	default:
		return (void*)&i64;		// union 으로 되어 있으니까.. 버퍼의 제일 앞을 주면 된다.
	}
}

size_t DBValue::Size()
{ return m_Size; }

size_t DBValue::Size() const
{ return m_Size; }

void DBValue::Clear()
{ m_sBuf = null_string; m_ValueType = vtNULL; m_Size = 0; i64 = 0; }


std::string& DBValue::ToString()
{
	if ( m_sBuf.empty() )
	{
		char buf[128];
		switch (m_ValueType)
		{
		case vtNULL: { m_sBuf = ""; break; }
		case vtI8  : { _snprintf_s(buf, sizeof(buf), "%d", i8); m_sBuf = buf; break; }
		case vtI16 : { _snprintf_s(buf, sizeof(buf), "%d", i16); m_sBuf = buf; break; }
		case vtI32 : { _snprintf_s(buf, sizeof(buf), "%d", i32); m_sBuf = buf; break; }
		case vtI64 : { _snprintf_s(buf, sizeof(buf), "%I64d", i64); m_sBuf = buf; break; }
		case vtUI8 : { _snprintf_s(buf, sizeof(buf), "%u", ui8); m_sBuf = buf; break; }
		case vtUI16: { _snprintf_s(buf, sizeof(buf), "%u", ui16); m_sBuf = buf; break; }
		case vtUI32: { _snprintf_s(buf, sizeof(buf), "%u", ui32); m_sBuf = buf; break; }
		case vtUI64: { _snprintf_s(buf, sizeof(buf), "%I64u", ui64); m_sBuf = buf; break; }
		case vtR32 : { _snprintf_s(buf, sizeof(buf), "%f", r32); m_sBuf = buf; break; }
		case vtR64 : { _snprintf_s(buf, sizeof(buf), "%lf", r64); m_sBuf = buf; break; }
		default : { m_sBuf = null_string; break; }
		}
	}
	return m_sBuf;
}

void DBValue::Assign( const void* p, size_t t )
{
	m_Size = t;
	m_sBuf.assign( (char*)p, t );
}
