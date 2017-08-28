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
	SQL ����� ���ε�Ǵ� �Ķ���͸� �����ϴ� Ŭ����
*/
class DBValue
{
public:
	enum ValueType {
		vtUnknown = 0x7E,	///< �������� ����
		vtEmpty = 0x7F,		///< ��� ����
		vtNULL = 0,			///< NULL
		vtI8,				///< 8 bit ����
		vtI16,				///< 16 bit ����
		vtI32,				///< 32 bit ����
		vtI64,				///< 64 bit ����
		vtUI8,				///< 8 bit ���
		vtUI16,				///< 16 bit ���
		vtUI32,				///< 32 bit ���
		vtUI64,				///< 64 bit ���
		vtR32,				///< 32 bit �Ǽ�
		vtR64,				///< 64 bit �Ǽ�
		vtString,			///< ���ڿ�
		vtBinary,			///< binary
		vtDecimal,			///< ����
		vtDate,				///< ��¥
		vtTimeStamp,		///< �ð�
	};

	ValueType		m_ValueType;	///< �ڷ���
	size_t			m_Size;			///< �ڷ�ũ��

	/// ���� ���� �����ϴ� ����ü
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
		����Ÿ�� null ������ �����Ѵ�.
	*/
	bool	IsNull( void );
	/** 
		����Ÿ�� ����� ������ �ּҰ��� ���´�.
	*/
	void*	Get( void );
	/** 
		����Ÿ�� ����� ������ �ּҰ��� ���´�.
	*/
	void*	Get( void ) const; 
	/** 
		����Ÿ�� ũ�⸦ ���´�.
	*/
	size_t	Size( void );
	/** 
		����Ÿ�� ũ�⸦ ���´�.
	*/
	size_t	Size( void ) const;
	/** 
		����Ÿ�� �����.
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
		����Ÿ�� �����Ѵ�.
		@param s ���ڿ�����Ÿ
	*/
	DBValue&	Set(const std::string& s);
	/** 
		����Ÿ�� �����Ѵ�.
		@param p ������ ����Ÿ�� �ּҰ�
		@param p ������ ����Ÿ�� ����Ʈ��
	*/
	DBValue&	Set(const void* p, size_t t);
	/** 
		����Ÿ�� �����Ѵ�.
		@param v ������ DBValue
	*/
	DBValue&	Set(const DBValue& v);
	/** 
		����Ÿ�� �����Ѵ�.
		@param p ������ ����Ÿ�� �ּҰ�
		@param p ������ ����Ÿ�� ����Ʈ��
	*/
	DBValue&	Set(const char* p, size_t t);
	/** 
		����Ÿ�� �����Ѵ�.
		@param v ������ DBValue
	*/
	DBValue& operator = (const DBValue& v)
	{
		return Set(v);
	}
	/** 
		����� ����Ÿ�� ���ڿ��� �����Ͽ� ��ȸ�Ѵ�.
	*/
	std::string&	ToString( void );

private:
	void Assign( const void* p, size_t t );
	std::string			m_sBuf;
};

/** 
	�÷�����Ÿ�� �����ϱ� ���� ������ �Ҵ��ϴ� Ŭ����
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
		�÷�����Ÿ�� �����ϱ� ���� ������ �Ҵ��Ѵ�.
		@param size �Ҵ��� ����Ʈ��
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
		�÷�����Ÿ�� �����ϱ� ���� ������ ũ�⸦ ��ȸ�Ѵ�.
	*/
	size_t size() const
	{
		return m_size;
	}
	/** 
		�÷�����Ÿ�� �ε��� ��ȣ�� ��ȸ�Ѵ�.
		@param i �ε�����ȣ
	*/
	T& operator [] (size_t i)
	{
		return m_array[i];
	}
	/** 
		�÷�����Ÿ�� �ε��� ��ȣ�� ��ȸ�Ѵ�.
		@param i �ε�����ȣ
	*/
	const T& operator [] (size_t i) const
	{
		return m_array[i];
	}
private:
	size_t m_size;
	T* m_array;
};
