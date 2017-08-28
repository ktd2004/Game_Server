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
	RPC.NET에서 송수신되는 패킷의 헤더를 Buffer에 저장하는 클래스

	RPC.NET에서 네트웍으로 송수신되는 패킷은 패킷헤더{ 패킷의길이(unsigned int) + 패킷ID(unsigned int) } + 데이타 형식으로 버퍼에 저장된다.
	
	        header              data
	+---------------------+---------------------+
	| len     | protocol  | this is date


	패킷의 길이는 패킷의 전체길이 즉 패킷헤더크기 + 데이타크기 이다.
*/
class Packet
{
public:
#pragma pack(push)
#pragma pack(1)
	struct header
	{
		unsigned int	len;		///< 패킷길이(패킷헤더길이+데이타길이)
		unsigned int	protocol;	///< 패킷ID
	};
#pragma pack(pop)

protected:
	Buffer*			m_pBuf;			///< 패킷을 직렬화할 Buffer포인터

	Packet( const Packet& r ) {}

public:
	Packet();
	/**
		Buffer의 rd_ptr() 가리키는 위치에 Packet::header 를 저장할 공간을 예약하며,
		Buffer의 head() 는 Packet::header의 시작 주소를 가리킨다.
	*/
	Packet(Buffer *pBuf);
	virtual ~Packet();
	/**	Buffer에 예약된 Packet::header에 ID를 설정한다.	*/
	inline void id( unsigned int iProtocol )
	{
		((header*)m_pBuf->head())->protocol = iProtocol;
	}
	/** Buffer에 예약된 Packet::header에 패킷의길이를 갱신한다. 	*/
	inline Buffer* get()
	{
		((header*)m_pBuf->head())->len = (unsigned int)(m_pBuf->wr_ptr() - m_pBuf->head());
		return m_pBuf;
	}
	/**
		Buffer의 wr_ptr()이 가리키는 위치에 Packet::header 크기 만큼 공간을 예약한다.

		하나의 Buffer에 다수의 패킷을 직렬화 할때 유용하게 사용할 수 있다.
		@verbatim
		// 버퍼를 할당한다.
		Buffer* pBuf = new Buffer(2048);
		
		Packet pk;
		for ( int i = 0; i < 3; i++ )
		{
			// Buffer의 wr_ptr()이 가리키는 위치에 패킷헤더를 공간을 할당한다.
			pk.set(pBuf);
			// 패킷 ID 를 설정한다.
			pk.id( i );
			// 전송할 데이타를 저장한다.
			pk << "1234567890";
			// 패킷길이를 갱신한다.
			pk.get();
		}

		// 위 코드가 수행되면 pBuf 에는 아래와 같이 3개의 패킷이 직렬화된다.
		// 패킷헤더+데이타+패킷헤더+데이타+패킷헤더+데이타

		// 패킷을 전송한다.
		spLink->Send( pBuf );
		@endverbatim
	*/
	inline Buffer* set(Buffer* pBuf)
	{
		m_pBuf = pBuf;
		pBuf->reserve(sizeof(header));
		return pBuf;
	}
	/** Buffer에 예약된 데이타를 직렬화. 	*/
	template<typename T> inline Packet& operator<< ( const T& Val )
	{
		*m_pBuf << Val;
		return *this;
	}
	/** Buffer에 저장된 데이타를 복사한다. */
	template<typename T> inline Packet& operator>> ( T& Val )
	{
		*m_pBuf >> Val;
		return *this;
	}

public:
	/**
		Buffer의 rd_ptr() 가리키는 위치에서 프로토콜ID 조회한다.
		@param rBuf 데이타가 저장된 버퍼
	*/
	static unsigned int	id( Buffer& rBuf );
	/**
		Buffer의 rd_ptr() 가리키는 위치에서 패킷길이를 조회한다.
		@param rBuf 데이타가 저장된 버퍼
	*/
	static unsigned int	length( Buffer& rBuf );
	/**
		Buffer에 저장된 복수개의 패킷에서 한개의 패킷을 분할한다.
		@param rBuf 데이타가 저장된 버퍼
		@param iLen 패킷의 길이
	*/
	static Buffer*		split( Buffer& rBuf, size_t iLen );
	/**
		Buffer의 rd_ptr() 가리키는 위치에 패킷의 길이를 설정한다.
	*/
	static Buffer*		update( Buffer& rBuf );
	/**
		Buffer에 저장된 복수개의 패킷을 분할하여 Buffer 리스트에 저장한다.
		@param rBuf 데이타가 저장된 버퍼
		@param lst 분할된 패킷을 저장할 버퍼리스트
		@param iMaxSize 패킷의 최대길이 만약 0 이면 패킷길이를 조사하지 않는다
	*/
	static bool			parse( Buffer* pBuf, Buffer::QType& lst, size_t iMaxSize = 0 );
};

