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

#include <string>
#include <Ptr.h>
#include <FastList.h>

/** 
	버퍼에 포인터값을 직렬화하기 위해서 사용되는 구조체.
*/
struct ptr_t
{
	void*	p;		///< 포인터값
	/** 
		포인터값을 저장한다.
	*/
	inline ptr_t& operator=(void* r)
	{
		this->p = r;
		return *this;
	}
	/** 
		포인터값을 참조한다.
	*/
	inline operator void* ()
	{
		return this->p;
	}
	/** 
		포인터값을 참조한다.
	*/
	inline operator void* () const
	{
		return this->p;
	}
	/** 
		포인터값을 비교한다.
	*/
	inline bool operator ==(const ptr_t& r)
	{
		return p == r.p;
	}
	/** 
		포인터값을 비교한다.
	*/
	inline bool operator !=(const ptr_t& r)
	{
		return !(p == r.p);
	}
	/** 
		포인터값을 비교한다.
	*/
	inline bool operator ==(const ptr_t& r) const
	{
		return p == r.p;
	}
	/** 
		포인터값을 비교한다.
	*/
	inline bool operator !=(const ptr_t& r) const
	{
		return !(p == r.p);
	}
};

/** 
	@def scalar값을 버퍼에 복사하거나 scalar자료형에 데이타를 저장하는 operator 정의 매크로.
*/
#define buffer_scalar_operator(y) \
	inline Buffer& operator>> ( y& Val ) \
	{ \
		if ( sizeof(y) > (int)length() ) { return *this; } \
		Val = *(reinterpret_cast<y*>(this->rd_ptr())); \
		this->rd_ptr(sizeof(Val)); \
		return *this; \
	}; \
	inline Buffer& operator<< ( y Val ) \
	{ \
		*(reinterpret_cast<y*>(this->wr_ptr())) = Val; \
		this->wr_ptr(sizeof(Val)); \
		return *this; \
	}

/** 
	scalar값이나 문자열(std::string/char*) 및 binary 데이타를 메모리에 직렬화하는 클래스

	scalar 변수나 std::string 또는 연속된 메모리공간에 저장된 binary 데이타를 Buffer에 할당된 메모리 블럭 직렬화한다.
	std::string나 char*의 문자열을 직렬화할 때는 "문자열의길이(4bytes)+문자열" 형식으로 저장되면 scalar나 binary 데이타를
	직렬화 할때는 저장된 데이타의 길이를 저장하지 않는다.

	@verbatim
	*. Buffer 직렬화 구조
	+--------+-------------------------------+-----------------------+
	|        문자열길이(4bytes)+"this is data"                         |
	+--------+-------------------------------+-----------------------+
	|        |                                |                       |
	m_begin  m_front                          m_back                  m_end
  	@endverbatim

	아래는 Buffer의 사용방법을 보여준다.
	@verbatim
	Buffer* pBuf = new Bufer(1024);
	int a = 10;
	float b = 10.23f;
	std::string c = "this is std::string";
	double e = 200.333f;

	// 데이타들을 Buffer에 순차적으로 직렬화 한다.
	*pBuf << a << b << c << d;
	// copy()함수로 e에 저장된 값을 직렬화 해도 된다.
	pBuf->copy( &e, sizeof(double) );

	// 데이타를 초기화한다.
	a = 0; b = 0.f; c = ""; d = ""; e = 0.f;

	printf( "저장된 데이타 바이트 : %d\n", pBuf->length() );
	printf( "저장 가능한 공간 : %d\n", pBuf->space() );
	printf( "저장된 데이타 시작 주소 : %p\n", pBuf->rd_ptr() );
	printf( "저장된 데이타 마지막 주소 : %p\n", pBuf->wr_ptr() );

	// Buffer 에 할당된 데이타 저장공간을 2048 로 늘려도 기존에 저장된 데이타가 사라지지 않는다.
	pBuf->resize( 2048 );
	printf( "저장 가능한 공간 : %d\n", pBuf->space() );

	// Buffer의 데이타를 꺼내서 변수들에 저장한다.
	*pBuf >> a >> b >> c >> d >> e;

	printf( "a = %d, b = %f, c = %s, d = %s\n", a, b, c.c_str(), d.c_str() );

	// Buffer를 초기화한다. 호출후에는 저장된 모든 데이타가 사라진다.
	pBuf->reset(); 

	@endverbatim
*/
class Buffer : public shared_ref, public listnode(Buffer)
{
	size_t			m_ibytes;	///< 할당버퍼크기
	char*			m_begin;	///< 버퍼시작주소
	char*			m_front;	///< 저장된데이타시작주소
	char*			m_back;		///< 저장된데이타종료주소
	char*			m_end;		///< 버퍼종료주소
	char*			m_head;		///< 예약된시작주소

public:
	Buffer();
	Buffer( size_t ibytes );
	Buffer( const Buffer &r );
	Buffer( const char *pBuf, size_t ibytes );
	virtual ~Buffer();

	/** 
		버퍼 크기를 지정된 크기로 재할당한다. 기존에 저장된 데이타는 재할당 이후에도 삭제되지 않는다.
		@param ibytes 재할당할 메모리의 크기
	*/
	void		resize( size_t ibytes );
	/** 
		읽기 주소값 위치에 지정된 크기로 공간을 예약한다. 예약된 공간의 시작 주소값은 head() 함수로 참조할 수 있다.
		@param ibytes 읽기 주소값 위치에 예약할 공간의 크기
	*/
	char*		reserve( size_t ibytes );
	/** 
		예약된 공간의 시작주소값을 조회한다.
	*/
	char*		head( void ) { return m_head; }
	/** 
		버퍼의 읽기,쓰기 위치를 버퍼의 시작주소값으로 설정하여 버퍼를 초기화한다. 기존에 존재하던 데이타는 삭제된다.
	*/
	void		reset( void );
	/** 
		버퍼에 할당된 메모리의 시작 주소값을 조회한다.
	*/
	char*		base( void );
	/** 
		데이타 읽기 주소값을 조회한다.
	*/
	char*		rd_ptr( void );
	/** 
	데이타 읽기 주소값을 설정한다. 버퍼에 할당된 메모리의 시작주소에서 종료주소까지의 범위내에서 지정해야 한다.
		@param ipos 읽기 주소값에 더해질 offset 값
	*/
	char*		rd_ptr( size_t ipos );
	/** 
		데이타 읽기 주소값을 설정한다. 버퍼에 할당된 메모리의 시작주소에서 종료주소까지의 범위내에서 지정해야 한다.
		@param pos 읽기 주소값
	*/
	char*		rd_ptr( char* pos );
	/** 
		데이타 쓰기 주소값을 조회한다.
	*/
	char*		wr_ptr( void );
	/** 
		데이타 쓰기 주소값을 설정한다. 버퍼에 할당된 메모리의 시작주소에서 종료주소까지의 범위내에서 지정해야 한다.
		@param ipos 쓰기 주소값에 더해질 offset 값
	*/
	char*		wr_ptr( size_t ipos );
	/** 
		데이타 쓰기 주소값을 설정한다. 버퍼에 할당된 메모리의 시작주소에서 종료주소까지의 범위내에서 지정해야 한다.
		@param pos 쓰기 주소값
	*/
	char*		wr_ptr( char* pos );
	/** 
		버퍼에 저장된 데이타 길이를 조회한다.(bytes).
	*/
	size_t		length( void );
	/** 
		저장된 데이타 길이를 조회한다.(bytes).
	*/
	size_t		size( void );
	/** 
		버퍼에 할당된 메모리의 크기를 조회한다.
	*/
	size_t		capacity( void );
	/** 
		버퍼에 저장 가능한 길이를 조회한다.(bytes).
	*/
	size_t		space( void );
	/** 
		binary 데이타를 버퍼로 복사한다.
		@param pBuf 데이타가 저장된 주소값
		@param ibytes 복사할 데이타의 길이
	*/
	size_t		copy( const char* pBuf, size_t ibytes );
	/** 
		데이타포멧 형식에 따라 문자열을 생성하여 버퍼에 복사한다.
		@param pFmt 데이타포멧형식으로 printf, sprintf 와 동일한 데이타포멧형식을 지원한다.
		@param ... 파라미터 리스트
	*/
	size_t		format( const char *pFmt, ... );
	/** 
		읽기 주소값 앞에 생긴 빈공간을 제거한다.
	*/
	void		crunch( void );
	/** 
		std::string에 저장된 문자열을 버퍼로 복사한다.
	*/
	Buffer& operator<< (const std::string& r);
	/** 
		버퍼에 저장된 데이타를 std::string 으로 복사한다.
	*/
	Buffer& operator>> (std::string& r);
	/** 
		문자열을 버퍼로 복사한다. 문자열은 null 로 종료처리 되어 있어야 한다.
	*/
	Buffer& operator<< (const char* pszBuf);
	/** 
		버퍼의 읽기 주소값의 문자열을 지정된 주소공간으로 복사한다.
	*/
	Buffer& operator>> (char* pszBuf);
	/** 
		포인터값을 버퍼로 복사한다.
	*/
	Buffer& operator<<(ptr_t p);
	/** 
		버퍼에서 포인터값을 ptr_t 구조체에 복사한다.
	*/
	inline Buffer& operator>>(ptr_t& p)
	{
		memcpy( &p, this->rd_ptr(), sizeof(ptr_t) );
		this->rd_ptr(sizeof(ptr_t));
		return *this;
	}
	/**
		버퍼로 데이타를 복사한다.
	*/
	Buffer& operator<<(const Buffer* p);
	/**
		버퍼에서 데이타를 복사한다.
	*/
	Buffer& operator>>(Buffer* p);
	/**
		버퍼로 데이타를 복사한다.
	*/
	Buffer& operator<<(const Buffer& p);
	/**
		버퍼에서 데이타를 복사한다.
	*/
	Buffer& operator>>(Buffer& p);
	/**
		버퍼를 삭제한다.
	*/
	void release( void );

	/** 
		버퍼내의 지정 주소값에 저장된 데이타를 직접 참조한다.
	*/
	char& operator[] ( size_t i );
	/** 
		bool 값을 버퍼로 복사, 버퍼를 bool 변수로 복사한다.
	*/
	buffer_scalar_operator( bool );
	/** 
		char 값을 버퍼로 복사, 버퍼를 char 변수로 복사한다.
	*/
	buffer_scalar_operator( char );
	/** 
		unsigned char 값을 버퍼로 복사, 버퍼를 unsigned char 변수로 복사한다.
	*/
	buffer_scalar_operator( unsigned char );
	/** 
		short 값을 버퍼로 복사, 버퍼를 short 변수로 복사한다.
	*/
	buffer_scalar_operator( short );
	/** 
		unsigned short 값을 버퍼로 복사, 버퍼를 unsigned short 변수로 복사한다.
	*/
	buffer_scalar_operator( unsigned short );
	/** 
		int 값을 버퍼로 복사, 버퍼를 int 변수로 복사한다.
	*/
	buffer_scalar_operator( int );
	/** 
		unsigned int 값을 버퍼로 복사, 버퍼를 unsigned int 변수로 복사한다.
	*/
	buffer_scalar_operator( unsigned int );
	/** 
		long 값을 버퍼로 복사, 버퍼를 long 변수로 복사한다.
	*/
	buffer_scalar_operator( long );
	/** 
		unsigned long 값을 버퍼로 복사, 버퍼를 unsigned long 변수로 복사.
	*/
	buffer_scalar_operator( unsigned long );
	/** 
		__int64 값을 버퍼로 복사, 버퍼를 __int64 변수로 복사.
	*/
	buffer_scalar_operator( __int64 );
	/** 
		unsigned __int64 값을 버퍼로 복사, 버퍼를 unsigned __int64 변수로 복사한다.
	*/
	buffer_scalar_operator( unsigned __int64 );
	/** 
		float 값을 버퍼로 복사, 버퍼를 float 변수로 복사한다.
	*/
	buffer_scalar_operator( float );
	/** 
		double 값을 버퍼로 복사, 버퍼를 double 변수로 복사한다.
	*/
	buffer_scalar_operator( double );
	/** 
		버퍼 큐타입 선언
	*/
	typedef util::list<Buffer>		QType;
};