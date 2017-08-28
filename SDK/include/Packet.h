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
	RPC.NET���� �ۼ��ŵǴ� ��Ŷ�� ����� Buffer�� �����ϴ� Ŭ����

	RPC.NET���� ��Ʈ������ �ۼ��ŵǴ� ��Ŷ�� ��Ŷ���{ ��Ŷ�Ǳ���(unsigned int) + ��ŶID(unsigned int) } + ����Ÿ �������� ���ۿ� ����ȴ�.
	
	        header              data
	+---------------------+---------------------+
	| len     | protocol  | this is date


	��Ŷ�� ���̴� ��Ŷ�� ��ü���� �� ��Ŷ���ũ�� + ����Ÿũ�� �̴�.
*/
class Packet
{
public:
#pragma pack(push)
#pragma pack(1)
	struct header
	{
		unsigned int	len;		///< ��Ŷ����(��Ŷ�������+����Ÿ����)
		unsigned int	protocol;	///< ��ŶID
	};
#pragma pack(pop)

protected:
	Buffer*			m_pBuf;			///< ��Ŷ�� ����ȭ�� Buffer������

	Packet( const Packet& r ) {}

public:
	Packet();
	/**
		Buffer�� rd_ptr() ����Ű�� ��ġ�� Packet::header �� ������ ������ �����ϸ�,
		Buffer�� head() �� Packet::header�� ���� �ּҸ� ����Ų��.
	*/
	Packet(Buffer *pBuf);
	virtual ~Packet();
	/**	Buffer�� ����� Packet::header�� ID�� �����Ѵ�.	*/
	inline void id( unsigned int iProtocol )
	{
		((header*)m_pBuf->head())->protocol = iProtocol;
	}
	/** Buffer�� ����� Packet::header�� ��Ŷ�Ǳ��̸� �����Ѵ�. 	*/
	inline Buffer* get()
	{
		((header*)m_pBuf->head())->len = (unsigned int)(m_pBuf->wr_ptr() - m_pBuf->head());
		return m_pBuf;
	}
	/**
		Buffer�� wr_ptr()�� ����Ű�� ��ġ�� Packet::header ũ�� ��ŭ ������ �����Ѵ�.

		�ϳ��� Buffer�� �ټ��� ��Ŷ�� ����ȭ �Ҷ� �����ϰ� ����� �� �ִ�.
		@verbatim
		// ���۸� �Ҵ��Ѵ�.
		Buffer* pBuf = new Buffer(2048);
		
		Packet pk;
		for ( int i = 0; i < 3; i++ )
		{
			// Buffer�� wr_ptr()�� ����Ű�� ��ġ�� ��Ŷ����� ������ �Ҵ��Ѵ�.
			pk.set(pBuf);
			// ��Ŷ ID �� �����Ѵ�.
			pk.id( i );
			// ������ ����Ÿ�� �����Ѵ�.
			pk << "1234567890";
			// ��Ŷ���̸� �����Ѵ�.
			pk.get();
		}

		// �� �ڵ尡 ����Ǹ� pBuf ���� �Ʒ��� ���� 3���� ��Ŷ�� ����ȭ�ȴ�.
		// ��Ŷ���+����Ÿ+��Ŷ���+����Ÿ+��Ŷ���+����Ÿ

		// ��Ŷ�� �����Ѵ�.
		spLink->Send( pBuf );
		@endverbatim
	*/
	inline Buffer* set(Buffer* pBuf)
	{
		m_pBuf = pBuf;
		pBuf->reserve(sizeof(header));
		return pBuf;
	}
	/** Buffer�� ����� ����Ÿ�� ����ȭ. 	*/
	template<typename T> inline Packet& operator<< ( const T& Val )
	{
		*m_pBuf << Val;
		return *this;
	}
	/** Buffer�� ����� ����Ÿ�� �����Ѵ�. */
	template<typename T> inline Packet& operator>> ( T& Val )
	{
		*m_pBuf >> Val;
		return *this;
	}

public:
	/**
		Buffer�� rd_ptr() ����Ű�� ��ġ���� ��������ID ��ȸ�Ѵ�.
		@param rBuf ����Ÿ�� ����� ����
	*/
	static unsigned int	id( Buffer& rBuf );
	/**
		Buffer�� rd_ptr() ����Ű�� ��ġ���� ��Ŷ���̸� ��ȸ�Ѵ�.
		@param rBuf ����Ÿ�� ����� ����
	*/
	static unsigned int	length( Buffer& rBuf );
	/**
		Buffer�� ����� �������� ��Ŷ���� �Ѱ��� ��Ŷ�� �����Ѵ�.
		@param rBuf ����Ÿ�� ����� ����
		@param iLen ��Ŷ�� ����
	*/
	static Buffer*		split( Buffer& rBuf, size_t iLen );
	/**
		Buffer�� rd_ptr() ����Ű�� ��ġ�� ��Ŷ�� ���̸� �����Ѵ�.
	*/
	static Buffer*		update( Buffer& rBuf );
	/**
		Buffer�� ����� �������� ��Ŷ�� �����Ͽ� Buffer ����Ʈ�� �����Ѵ�.
		@param rBuf ����Ÿ�� ����� ����
		@param lst ���ҵ� ��Ŷ�� ������ ���۸���Ʈ
		@param iMaxSize ��Ŷ�� �ִ���� ���� 0 �̸� ��Ŷ���̸� �������� �ʴ´�
	*/
	static bool			parse( Buffer* pBuf, Buffer::QType& lst, size_t iMaxSize = 0 );
};

