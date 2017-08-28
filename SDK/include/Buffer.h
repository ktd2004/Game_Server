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
	���ۿ� �����Ͱ��� ����ȭ�ϱ� ���ؼ� ���Ǵ� ����ü.
*/
struct ptr_t
{
	void*	p;		///< �����Ͱ�
	/** 
		�����Ͱ��� �����Ѵ�.
	*/
	inline ptr_t& operator=(void* r)
	{
		this->p = r;
		return *this;
	}
	/** 
		�����Ͱ��� �����Ѵ�.
	*/
	inline operator void* ()
	{
		return this->p;
	}
	/** 
		�����Ͱ��� �����Ѵ�.
	*/
	inline operator void* () const
	{
		return this->p;
	}
	/** 
		�����Ͱ��� ���Ѵ�.
	*/
	inline bool operator ==(const ptr_t& r)
	{
		return p == r.p;
	}
	/** 
		�����Ͱ��� ���Ѵ�.
	*/
	inline bool operator !=(const ptr_t& r)
	{
		return !(p == r.p);
	}
	/** 
		�����Ͱ��� ���Ѵ�.
	*/
	inline bool operator ==(const ptr_t& r) const
	{
		return p == r.p;
	}
	/** 
		�����Ͱ��� ���Ѵ�.
	*/
	inline bool operator !=(const ptr_t& r) const
	{
		return !(p == r.p);
	}
};

/** 
	@def scalar���� ���ۿ� �����ϰų� scalar�ڷ����� ����Ÿ�� �����ϴ� operator ���� ��ũ��.
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
	scalar���̳� ���ڿ�(std::string/char*) �� binary ����Ÿ�� �޸𸮿� ����ȭ�ϴ� Ŭ����

	scalar ������ std::string �Ǵ� ���ӵ� �޸𸮰����� ����� binary ����Ÿ�� Buffer�� �Ҵ�� �޸� �� ����ȭ�Ѵ�.
	std::string�� char*�� ���ڿ��� ����ȭ�� ���� "���ڿ��Ǳ���(4bytes)+���ڿ�" �������� ����Ǹ� scalar�� binary ����Ÿ��
	����ȭ �Ҷ��� ����� ����Ÿ�� ���̸� �������� �ʴ´�.

	@verbatim
	*. Buffer ����ȭ ����
	+--------+-------------------------------+-----------------------+
	|        ���ڿ�����(4bytes)+"this is data"                         |
	+--------+-------------------------------+-----------------------+
	|        |                                |                       |
	m_begin  m_front                          m_back                  m_end
  	@endverbatim

	�Ʒ��� Buffer�� ������� �����ش�.
	@verbatim
	Buffer* pBuf = new Bufer(1024);
	int a = 10;
	float b = 10.23f;
	std::string c = "this is std::string";
	double e = 200.333f;

	// ����Ÿ���� Buffer�� ���������� ����ȭ �Ѵ�.
	*pBuf << a << b << c << d;
	// copy()�Լ��� e�� ����� ���� ����ȭ �ص� �ȴ�.
	pBuf->copy( &e, sizeof(double) );

	// ����Ÿ�� �ʱ�ȭ�Ѵ�.
	a = 0; b = 0.f; c = ""; d = ""; e = 0.f;

	printf( "����� ����Ÿ ����Ʈ : %d\n", pBuf->length() );
	printf( "���� ������ ���� : %d\n", pBuf->space() );
	printf( "����� ����Ÿ ���� �ּ� : %p\n", pBuf->rd_ptr() );
	printf( "����� ����Ÿ ������ �ּ� : %p\n", pBuf->wr_ptr() );

	// Buffer �� �Ҵ�� ����Ÿ ��������� 2048 �� �÷��� ������ ����� ����Ÿ�� ������� �ʴ´�.
	pBuf->resize( 2048 );
	printf( "���� ������ ���� : %d\n", pBuf->space() );

	// Buffer�� ����Ÿ�� ������ �����鿡 �����Ѵ�.
	*pBuf >> a >> b >> c >> d >> e;

	printf( "a = %d, b = %f, c = %s, d = %s\n", a, b, c.c_str(), d.c_str() );

	// Buffer�� �ʱ�ȭ�Ѵ�. ȣ���Ŀ��� ����� ��� ����Ÿ�� �������.
	pBuf->reset(); 

	@endverbatim
*/
class Buffer : public shared_ref, public listnode(Buffer)
{
	size_t			m_ibytes;	///< �Ҵ����ũ��
	char*			m_begin;	///< ���۽����ּ�
	char*			m_front;	///< ����ȵ���Ÿ�����ּ�
	char*			m_back;		///< ����ȵ���Ÿ�����ּ�
	char*			m_end;		///< ���������ּ�
	char*			m_head;		///< ����Ƚ����ּ�

public:
	Buffer();
	Buffer( size_t ibytes );
	Buffer( const Buffer &r );
	Buffer( const char *pBuf, size_t ibytes );
	virtual ~Buffer();

	/** 
		���� ũ�⸦ ������ ũ��� ���Ҵ��Ѵ�. ������ ����� ����Ÿ�� ���Ҵ� ���Ŀ��� �������� �ʴ´�.
		@param ibytes ���Ҵ��� �޸��� ũ��
	*/
	void		resize( size_t ibytes );
	/** 
		�б� �ּҰ� ��ġ�� ������ ũ��� ������ �����Ѵ�. ����� ������ ���� �ּҰ��� head() �Լ��� ������ �� �ִ�.
		@param ibytes �б� �ּҰ� ��ġ�� ������ ������ ũ��
	*/
	char*		reserve( size_t ibytes );
	/** 
		����� ������ �����ּҰ��� ��ȸ�Ѵ�.
	*/
	char*		head( void ) { return m_head; }
	/** 
		������ �б�,���� ��ġ�� ������ �����ּҰ����� �����Ͽ� ���۸� �ʱ�ȭ�Ѵ�. ������ �����ϴ� ����Ÿ�� �����ȴ�.
	*/
	void		reset( void );
	/** 
		���ۿ� �Ҵ�� �޸��� ���� �ּҰ��� ��ȸ�Ѵ�.
	*/
	char*		base( void );
	/** 
		����Ÿ �б� �ּҰ��� ��ȸ�Ѵ�.
	*/
	char*		rd_ptr( void );
	/** 
	����Ÿ �б� �ּҰ��� �����Ѵ�. ���ۿ� �Ҵ�� �޸��� �����ּҿ��� �����ּұ����� ���������� �����ؾ� �Ѵ�.
		@param ipos �б� �ּҰ��� ������ offset ��
	*/
	char*		rd_ptr( size_t ipos );
	/** 
		����Ÿ �б� �ּҰ��� �����Ѵ�. ���ۿ� �Ҵ�� �޸��� �����ּҿ��� �����ּұ����� ���������� �����ؾ� �Ѵ�.
		@param pos �б� �ּҰ�
	*/
	char*		rd_ptr( char* pos );
	/** 
		����Ÿ ���� �ּҰ��� ��ȸ�Ѵ�.
	*/
	char*		wr_ptr( void );
	/** 
		����Ÿ ���� �ּҰ��� �����Ѵ�. ���ۿ� �Ҵ�� �޸��� �����ּҿ��� �����ּұ����� ���������� �����ؾ� �Ѵ�.
		@param ipos ���� �ּҰ��� ������ offset ��
	*/
	char*		wr_ptr( size_t ipos );
	/** 
		����Ÿ ���� �ּҰ��� �����Ѵ�. ���ۿ� �Ҵ�� �޸��� �����ּҿ��� �����ּұ����� ���������� �����ؾ� �Ѵ�.
		@param pos ���� �ּҰ�
	*/
	char*		wr_ptr( char* pos );
	/** 
		���ۿ� ����� ����Ÿ ���̸� ��ȸ�Ѵ�.(bytes).
	*/
	size_t		length( void );
	/** 
		����� ����Ÿ ���̸� ��ȸ�Ѵ�.(bytes).
	*/
	size_t		size( void );
	/** 
		���ۿ� �Ҵ�� �޸��� ũ�⸦ ��ȸ�Ѵ�.
	*/
	size_t		capacity( void );
	/** 
		���ۿ� ���� ������ ���̸� ��ȸ�Ѵ�.(bytes).
	*/
	size_t		space( void );
	/** 
		binary ����Ÿ�� ���۷� �����Ѵ�.
		@param pBuf ����Ÿ�� ����� �ּҰ�
		@param ibytes ������ ����Ÿ�� ����
	*/
	size_t		copy( const char* pBuf, size_t ibytes );
	/** 
		����Ÿ���� ���Ŀ� ���� ���ڿ��� �����Ͽ� ���ۿ� �����Ѵ�.
		@param pFmt ����Ÿ������������ printf, sprintf �� ������ ����Ÿ���������� �����Ѵ�.
		@param ... �Ķ���� ����Ʈ
	*/
	size_t		format( const char *pFmt, ... );
	/** 
		�б� �ּҰ� �տ� ���� ������� �����Ѵ�.
	*/
	void		crunch( void );
	/** 
		std::string�� ����� ���ڿ��� ���۷� �����Ѵ�.
	*/
	Buffer& operator<< (const std::string& r);
	/** 
		���ۿ� ����� ����Ÿ�� std::string ���� �����Ѵ�.
	*/
	Buffer& operator>> (std::string& r);
	/** 
		���ڿ��� ���۷� �����Ѵ�. ���ڿ��� null �� ����ó�� �Ǿ� �־�� �Ѵ�.
	*/
	Buffer& operator<< (const char* pszBuf);
	/** 
		������ �б� �ּҰ��� ���ڿ��� ������ �ּҰ������� �����Ѵ�.
	*/
	Buffer& operator>> (char* pszBuf);
	/** 
		�����Ͱ��� ���۷� �����Ѵ�.
	*/
	Buffer& operator<<(ptr_t p);
	/** 
		���ۿ��� �����Ͱ��� ptr_t ����ü�� �����Ѵ�.
	*/
	inline Buffer& operator>>(ptr_t& p)
	{
		memcpy( &p, this->rd_ptr(), sizeof(ptr_t) );
		this->rd_ptr(sizeof(ptr_t));
		return *this;
	}
	/**
		���۷� ����Ÿ�� �����Ѵ�.
	*/
	Buffer& operator<<(const Buffer* p);
	/**
		���ۿ��� ����Ÿ�� �����Ѵ�.
	*/
	Buffer& operator>>(Buffer* p);
	/**
		���۷� ����Ÿ�� �����Ѵ�.
	*/
	Buffer& operator<<(const Buffer& p);
	/**
		���ۿ��� ����Ÿ�� �����Ѵ�.
	*/
	Buffer& operator>>(Buffer& p);
	/**
		���۸� �����Ѵ�.
	*/
	void release( void );

	/** 
		���۳��� ���� �ּҰ��� ����� ����Ÿ�� ���� �����Ѵ�.
	*/
	char& operator[] ( size_t i );
	/** 
		bool ���� ���۷� ����, ���۸� bool ������ �����Ѵ�.
	*/
	buffer_scalar_operator( bool );
	/** 
		char ���� ���۷� ����, ���۸� char ������ �����Ѵ�.
	*/
	buffer_scalar_operator( char );
	/** 
		unsigned char ���� ���۷� ����, ���۸� unsigned char ������ �����Ѵ�.
	*/
	buffer_scalar_operator( unsigned char );
	/** 
		short ���� ���۷� ����, ���۸� short ������ �����Ѵ�.
	*/
	buffer_scalar_operator( short );
	/** 
		unsigned short ���� ���۷� ����, ���۸� unsigned short ������ �����Ѵ�.
	*/
	buffer_scalar_operator( unsigned short );
	/** 
		int ���� ���۷� ����, ���۸� int ������ �����Ѵ�.
	*/
	buffer_scalar_operator( int );
	/** 
		unsigned int ���� ���۷� ����, ���۸� unsigned int ������ �����Ѵ�.
	*/
	buffer_scalar_operator( unsigned int );
	/** 
		long ���� ���۷� ����, ���۸� long ������ �����Ѵ�.
	*/
	buffer_scalar_operator( long );
	/** 
		unsigned long ���� ���۷� ����, ���۸� unsigned long ������ ����.
	*/
	buffer_scalar_operator( unsigned long );
	/** 
		__int64 ���� ���۷� ����, ���۸� __int64 ������ ����.
	*/
	buffer_scalar_operator( __int64 );
	/** 
		unsigned __int64 ���� ���۷� ����, ���۸� unsigned __int64 ������ �����Ѵ�.
	*/
	buffer_scalar_operator( unsigned __int64 );
	/** 
		float ���� ���۷� ����, ���۸� float ������ �����Ѵ�.
	*/
	buffer_scalar_operator( float );
	/** 
		double ���� ���۷� ����, ���۸� double ������ �����Ѵ�.
	*/
	buffer_scalar_operator( double );
	/** 
		���� ťŸ�� ����
	*/
	typedef util::list<Buffer>		QType;
};