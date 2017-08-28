#pragma once

#include <DBEngine.h>

#define ANY_QUEUE	0
//#define UID_QUEUE(uid)	(uid % 5)

typedef IDBCommand::DBCommandType	CommandType;

template <class T>
class QueryAbstract_v0 : public Query
{
public:
	typedef functional_v1<void, T*>		CallbackFn;

public:
	QueryAbstract_v0( QUID _iQID=ANY_QUEUE, CommandType _eType=IDBCommand::ctSP )
		: Query(_iQID, _eType)
	{}

	virtual ~QueryAbstract_v0() {}

	CallbackFn*	GetFn()
	{ return &m_Fn; }

	template <class O, class F>
	void SetFn( O* _o, F _f )
	{ m_Fn.assign( _o, _f ); }

	virtual void OnResult()
	{
		if( m_Fn )
		{
			m_Fn( (T*)this );
		}
	}

	virtual void OnError( int32 iErrorCode, const std::string& sErrorMsg )
	{
		__super::OnError( iErrorCode, sErrorMsg );
	}

protected:
	CallbackFn		m_Fn;
};

template <class T>
class QueryAbstract_v1 : public Query
{
public:
	typedef functional_v2<void, NetLinkPtr, T*>		CallbackFn;

public:
	QueryAbstract_v1( NetLinkPtr spLink, QUID _iQID=ANY_QUEUE, CommandType _eType=IDBCommand::ctSP )
		: Query(_iQID, _eType)
		, m_spLink(spLink)
	{}
	
	virtual ~QueryAbstract_v1() {}

	CallbackFn*	GetFn()
	{ return &m_Fn; }

	template <class O, class F>
	void SetFn( O* _o, F _f )
	{
		m_Fn.assign( _o, _f );
	}

	void SetLink( NetLinkPtr spLink )
	{ m_spLink = spLink; }

	virtual void OnResult()
	{
		if( m_Fn )
		{
			m_Fn( m_spLink, (T*)this );
		}
	}

	virtual void OnError( int32 iErrorCode, const std::string& sErrorMsg )
	{
		__super::OnError( iErrorCode, sErrorMsg );
	}

protected:
	NetLinkPtr		m_spLink;
	CallbackFn		m_Fn;
};

///////////////////////////////////////////////////////////////////////////////
// functor ������Ʈ�� mfunctinal �� ����Ͽ���.
// �ݹ��Լ��� ����Ҷ� �������� �������� �ʱ� ������ �޸𸮰� ����Ǵ� ������
// �ִ�. ������ ������ �����Ҷ� �ݵ�� �ݹ��� Ŭ������ �����ؾ� �ϸ�
// ������Ͽ� ��� ������ �����ؾ� ũ������ �ȳ���.
///////////////////////////////////////////////////////////////////////////////
template <class B, class T>
class QueryAbstract_v2 : public Query
{
public:
	typedef mfunctional_v1<B, void, T*>		CallbackFn;

public:
	QueryAbstract_v2( QUID _iQID=ANY_QUEUE, CommandType _eType=IDBCommand::ctSP )
		: Query(_iQID, _eType)
	{}

	virtual ~QueryAbstract_v2() {}

	CallbackFn*	GetFn()
	{ return &m_Fn; }

	template <class O, class F>
	void SetFn( O* _o, F _f )
	{ m_Fn.assign( _o, _f ); }

	virtual void OnResult()
	{
		if( m_Fn )
		{
			m_Fn( (T*)this );
		}
	}

	virtual void OnError( int32 iErrorCode, const std::string& sErrorMsg )
	{
		__super::OnError( iErrorCode, sErrorMsg );
	}

protected:
	CallbackFn		m_Fn;
};

template <class B, class T>
class QueryAbstract_v3 : public Query
{
public:
	typedef mfunctional_v2<B, void, NetLinkPtr, T*>		CallbackFn;

public:
	QueryAbstract_v3( NetLinkPtr spLink, QUID _iQID=ANY_QUEUE, CommandType _eType=IDBCommand::ctSP )
		: Query(_iQID, _eType)
		, m_spLink(spLink)
	{}

	virtual ~QueryAbstract_v3() {}

	CallbackFn*	GetFn()
	{ return &m_Fn; }

	template <class O, class F>
	void SetFn( O* _o, F _f )
	{
		m_Fn.assign( _o, _f );
	}

	void SetLink( NetLinkPtr spLink )
	{ m_spLink = spLink; }

	virtual void OnResult()
	{
		if( m_Fn )
		{
			m_Fn( m_spLink, (T*)this );
		}
	}

	virtual void OnError( int32 iErrorCode, const std::string& sErrorMsg )
	{
		__super::OnError( iErrorCode, sErrorMsg );
	}

protected:
	NetLinkPtr		m_spLink;
	CallbackFn		m_Fn;
};
