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
#include <Ptr.h>
/** 
	SQL 실행시 바인드되는 파라미터를 저장하는 클래스
*/
class DBValue
{
public:
	enum ValueType {
		vtUnknown = 0x7E,	///< 지정되지 않음
		vtEmpty = 0x7F,		///< 비어 있음
		vtNULL = 0,			///< NULL
		vtI8,				///< 8 bit 정수
		vtI16,				///< 16 bit 정수
		vtI32,				///< 32 bit 정수
		vtI64,				///< 64 bit 정수
		vtUI8,				///< 8 bit 양수
		vtUI16,				///< 16 bit 양수
		vtUI32,				///< 32 bit 양수
		vtUI64,				///< 64 bit 양수
		vtR32,				///< 32 bit 실수
		vtR64,				///< 64 bit 실수
		vtString,			///< 문자열
		vtBinary,			///< binary
		vtDecimal,			///< 숫자
		vtDate,				///< 날짜
		vtTimeStamp,		///< 시간
	};

	ValueType		m_ValueType;	///< 자료형
	size_t			m_Size;			///< 자료크기

	/// 실제 값을 저장하는 공용체
	union {
		int8 i8;
		int16 i16;
		int32 i32;
		int64 i64;
		uchar ui8;
		uint16 ui16;
		uint32 ui32;
		uint64 ui64;
		float r32;
		double r64;
		char dummy;
	};
//	spPtr<MemBuf>	mem;

public:
	DBValue();
	DBValue(const DBValue& r);
	virtual ~DBValue();

	/** 
		데이타가 null 값인지 조사한다.
	*/
	bool	IsNull( void );
	/** 
		데이타가 저장된 공간의 주소값을 얻어온다.
	*/
	void*	Get( void );
	/** 
		데이타가 저장된 공간의 주소값을 얻어온다.
	*/
	void*	Get( void ) const; 
	/** 
		데이타의 크기를 얻어온다.
	*/
	size_t	Size( void );
	/** 
		데이타의 크기를 얻어온다.
	*/
	size_t	Size( void ) const;
	/** 
		데이타를 지운다.
	*/
	void	Clear( void );

#define op_case(VT, fmt) \
	char buf[128]; \
	_snprintf_s(buf, sizeof(buf), fmt, v); m_sBuf = buf;
#define op_set(T, VT, V, fmt) \
	DBValue(T v) : m_ValueType(VT), m_Size(sizeof(T)), V(v) { op_case(VT, fmt); } \
	DBValue& Set(T v) { Clear(); m_ValueType = VT; V = v; m_Size = sizeof(T); op_case(VT, fmt); return *this; } \
	DBValue& operator = (T v) { return Set(v); }

	op_set(int8,   vtI8,   i8,   "%d");
	op_set(int16,  vtI16,  i16,  "%d");
	op_set(int32,  vtI32,  i32,  "%d");
	op_set(int64,  vtI64,  i64,  "%I64d");
	op_set(uint8,  vtUI8,  ui8,  "%u");
	op_set(uint16, vtUI16, ui16, "%u");
	op_set(uint32, vtUI32, ui32, "%u");
	op_set(uint64, vtUI64, ui64, "%I64u");
	op_set(float,  vtR32,  r32,  "%f");
	op_set(double, vtR64,  r64,  "%lf");

	DBValue(const std::string& s);
	DBValue(void* p, size_t t);
	DBValue(const char* p);

	/** 
		데이타를 저장한다.
		@param s 문자열데이타
	*/
	DBValue&	Set(const std::string& s);
	/** 
		데이타를 저장한다.
		@param p 저장할 데이타의 주소값
		@param p 저장할 데이타의 바이트수
	*/
	DBValue&	Set(const void* p, size_t t);
	/** 
		데이타를 저장한다.
		@param v 저장할 DBValue
	*/
	DBValue&	Set(const DBValue& v);
	/** 
		데이타를 저장한다.
		@param p 저장할 데이타의 주소값
		@param p 저장할 데이타의 바이트수
	*/
	DBValue&	Set(const char* p, size_t t);
	/** 
		데이타를 저장한다.
		@param v 저장할 DBValue
	*/
	DBValue& operator = (const DBValue& v)
	{
		return Set(v);
	}
	/** 
		저장된 데이타를 문자열로 변경하여 조회한다.
	*/
	std::string&	ToString( void );

private:
	void Assign( const void* p, size_t t );
	std::string			m_sBuf;
};

/** 
	컬럼데이타를 저장하기 위한 공간을 할당하는 클래스
*/
template <class T>
class StaticArray
{
public:
	StaticArray() : m_size(0), m_array(nullptr)
	{}
	StaticArray(size_t size) : m_size(size)
	{
		if ( size > 0 )
		{
			m_array = new T[size];
		}
		else
		{
			m_array = nullptr;
		}
	}
	~StaticArray()
	{
		safe_array_delete(m_array);
	}
	/** 
		컬럼데이타를 저장하기 위한 공간을 할당한다.
		@param size 할당할 바이트수
	*/
	void alloc(size_t size)
	{ 
		safe_array_delete(m_array);
		if ( size > 0 )
		{
			m_array = new T[size];
		}
	}
	/** 
		컬럼데이타를 저장하기 위한 공간의 크기를 조회한다.
	*/
	size_t size() const
	{
		return m_size;
	}
	/** 
		컬럼데이타를 인덱스 번호로 조회한다.
		@param i 인덱스번호
	*/
	T& operator [] (size_t i)
	{
		return m_array[i];
	}
	/** 
		컬럼데이타를 인덱스 번호로 조회한다.
		@param i 인덱스번호
	*/
	const T& operator [] (size_t i) const
	{
		return m_array[i];
	}
private:
	size_t m_size;
	T* m_array;
};
