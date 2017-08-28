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
#include <MacroFunc.h>

class interface_ref
{
public:
	virtual long incref(void) = 0;
	virtual long incref(long r) = 0;
	virtual long decref(void) = 0;
	virtual long decref(long r) = 0;
	virtual long refcount() = 0;
};

/** 
	���۷��� ī��Ʈ�� �����ϴ� Ŭ����

	�����尣�� ���۷���ī������ ������ ����ȭ ��Ű�� Ŭ����
*/
class shared_ref : public interface_ref
{
	volatile long m_ref;		///< ���۷��� ī��Ʈ.

public:
	shared_ref() : m_ref(0) {}
	virtual ~shared_ref()	{}

	/** ���۷���ī��Ʈ 1����. */
	inline long incref(void)
	{
		return locked_inc(this->m_ref);
	}
	/** ���۷���ī��Ʈ ������ ��������ŭ ����. */
	inline long incref(long r)
	{
		return locked_exchange_add(this->m_ref, r) + r;
	}
	/** ���۷���ī��Ʈ 1����. */
	inline long decref(void)
	{
		return locked_dec(this->m_ref);
	}
	/** ���۷���ī��Ʈ ������ ��ŭ ����. */
	inline long decref(long r)
	{
		return locked_exchange_sub(this->m_ref, r);
	}
	/** ���۷���ī��Ʈ ��ȸ. */
	inline long refcount() { return this->m_ref; }
};

class single_ref : public interface_ref
{
	volatile long m_ref;		///< ���۷��� ī��Ʈ.

public:
	single_ref() : m_ref(0) {}
	virtual ~single_ref()	{}

	/** ���۷���ī��Ʈ 1����. */
	inline long incref(void)
	{
		return ++this->m_ref;
	}
	/** ���۷���ī��Ʈ ������ ��������ŭ ����. */
	inline long incref(long r)
	{
		this->m_ref += r;
		return this->m_ref;
	}
	/** ���۷���ī��Ʈ 1����. */
	inline long decref(void)
	{
		this->m_ref--;
		return this->m_ref;
	}
	/** ���۷���ī��Ʈ ������ ��ŭ ����. */
	inline long decref(long r)
	{
		this->m_ref -= r;
		return this->m_ref;
	}
	/** ���۷���ī��Ʈ ��ȸ. */
	inline long refcount() { return this->m_ref; }
};

/** 
	����Ʈ ������.

	spRef�� �Ҵ�� ������Ʈ�� spRef �� �����ɶ� ���� �����ȴ�.
*/
template <class T>
class spRef
{
	typedef T type;
	mutable volatile T* m_p;
public:
	spRef()
		: m_p(0)
	{}
	spRef(T* p)
		: m_p(p)
	{
	}
	spRef(const spRef<T>& r)
		: m_p(0)
	{
		if ( r.m_p )
		{
			this->m_p = (T*)locked_exchange_pointer(r.m_p, nullptr);
		}
	}
	virtual ~spRef()
	{ 
		T* p = (T*)locked_exchange_pointer(this->m_p, nullptr);
		safe_delete(p);
	}
	/** ����� Ŭ����������Ʈ�� ��ȸ */
	inline T* get() { return (T*)this->m_p; }
	/** ����� Ŭ����������Ʈ�� �и� */
	inline T* detach()
	{
		T* p = (T*)locked_exchange_pointer(this->m_p, nullptr);
		return p;
	}
	/** Ŭ����������Ʈ�� �����Ѵ� */
	inline spRef& attach(T* p)
	{
		T* t = (T*)locked_exchange_pointer(this->m_p, p);
		safe_delete(t);
		return *this;
	}
	/** ����� Ŭ����������Ʈ�� ��ȸ */
	inline operator T*() { return (T*)this->m_p; }
	/** ����� Ŭ����������Ʈ�� ��ȸ */
	inline operator T*() const { return (T*)this->m_p; }
	/** ����� Ŭ����������Ʈ�� ���۷����� ��ȯ */
	inline T& operator *() const { return *(T*)this->m_p; }
	/** ����� Ŭ����������Ʈ�� �ɹ� ���� */
	inline T* operator ->() const	{ return (T*)this->m_p; }
	/** ����� Ŭ����������Ʈ�� nullptr ���� ���� */
	inline bool operator !() const { return (this->m_p == 0); }
	/** Ŭ����������Ʈ�� �������� �� */
	inline bool operator ==(T* p) const { return this->m_p == p; }
	/** Ŭ����������Ʈ�� �����Ѵ�. */
	inline spRef& operator =(T* p)
	{
		T* t = (T*)locked_exchange_pointer(this->m_p, p);
		safe_delete(t);
		return *this;
	}
	/** ����� spRef���� Ŭ����������Ʈ�� �и��� �ڽſ� ���� */
	inline spRef& operator =(const spRef<T>& sp)
	{
		T* src = (T*)locked_exchange_pointer(sp.m_p, nullptr);
		T* t = (T*)locked_exchange_pointer(this->m_p, src);
		safe_delete(t);
		return *this;
	}	
};

/** 
	�����尣 ���� �������� �����ϴ� ����Ʈ������

	spPtr�� �Ҵ�� ������Ʈ�� spPtr�� ���۷��� ī��Ʈ�� ���ҽ�Ų�� ���۷��� ī��Ʈ�� 0 �� �ɶ�
	������Ʈ�� �����Ѵ�.
	��ü�� ���۷���ī��Ʈ�� ������ �����尣�� ����ȭ�ǹǷ� ��Ƽ�����尣���� ������ ������ �����Ѵ�.
	�Ʒ��� �ڵ�� spPtr ����Ʈ�����Ͱ� MyObj�� �����ϱ� ���� ����� �����ش�.

	@verbatim
	// MyObj ����Ʈ������ MyObjPtr Ÿ���� �����ϴ� ��ũ��.
	PTR(MyObj);
	// MyObj �� spPtr ����Ʈ�����Ϳ� �Ҵ��ϱ� ���ؼ��� shared_ref �� ����ؾ� �Ѵ�.
	class MyObj : public shared_ref
	{
	public:
		MyObj() {}
		virtual ~MyObj() {}

		void dump()
		{
			printf( __FUNCTION__"\n" );
		}
	};

	void Func( void )
	{
		// MyObj �� �����Ͽ� ����Ʈ�����Ϳ� �Ҵ��Ѵ�.
		MyObjPtr	spObj(new MyObj);

		spObj->dump();

		// Func �Լ��� ����Ǹ� MyObj�� �ڵ� �����ȴ�.
	}

	@endverbatim
*/
template <class T>
class spPtr
{
public:
	typedef T type;
	T*			m_p;		// object
	void*		m_param;
	spPtr()
		: m_p(0), m_param(0)
	{} 
	spPtr(T* p)
		: m_p(p), m_param(0)
	{
		istrue_do( m_p, m_p->incref() );
	}
	spPtr(const spPtr<T>& sp)
		: m_p(sp.m_p), m_param(sp.m_param)
	{
		istrue_do( this->m_p, this->m_p->incref() );
	}

	virtual ~spPtr()
	{ 
		if ( this->m_p && 0 == this->m_p->decref() )
		{
			T* p = (T*)locked_exchange_pointer(this->m_p, nullptr);
			safe_delete(p);
		}
	}

	/** ����� Ŭ����������Ʈ�� ��ȸ */
	inline T*	get() { return (T*)this->m_p; }
	/** ����� Ŭ����������Ʈ�� �и� */
	inline T*	detach()
	{
		T* p = (T*)locked_exchange_pointer(this->m_p, nullptr);
		if ( p ) p->decref();
		return p;
	}
	/** �Ķ���͸� �����Ѵ� */
	inline void* param( void ) { return m_param; }
	/** �Ķ���͸� ��ȸ�Ѵ�. */
	inline void param( void* p ) { m_param = p; }
	/** ����� Ŭ����������Ʈ�� ��ȸ */
	inline operator T*() { return (T*)this->m_p; }
	/** ����� Ŭ����������Ʈ�� ��ȸ */
	inline operator T*() const { return (T*)this->m_p; }
	/** ����� Ŭ����������Ʈ�� ���۷����� ��ȯ */
	inline T& operator *() const { return *(T*)this->m_p; }
	/** ����� Ŭ����������Ʈ�� �ɹ� ���� */
	inline T* operator ->() const { return (T*)this->m_p; }
	/** Ŭ����������Ʈ�� �������� �� */
	inline bool operator !() const { return (this->m_p == 0); }
	/** Ŭ����������Ʈ�� �������� �� */
	inline bool operator ==(T* p) const { return (this->m_p == p); }
	/** Ŭ����������Ʈ�� �����Ѵ�. */
	inline spPtr& operator =(T* p)
	{
		assign(this->m_p, p);
		return *this;
	}
	/** Ŭ����������Ʈ�� �����Ѵ�. */
	inline spPtr& operator =(const spPtr<T>& sp)
	{
		assign(this->m_p, sp.m_p);
		m_param = sp.m_param;
		return *this;
	}
	/** ����Ʈ������ ���� */
	template <class U> spPtr(const spPtr<U>& sp)
		: m_p(0), m_param(0)
	{
		assign(this->m_p, sp.m_p);
		m_param = sp.m_param;
	}
	/** ����Ʈ������ ���� */
	template <class U> spPtr& operator =(const spPtr<U>& sp)
	{
		assign(this->m_p, sp.m_p);
		m_param = sp.m_param;
		return *this;
	}

private:
	/** ����Ʈ�����Ϳ� Ŭ����������Ʈ �Ҵ� */
	template <class U> static U * assign(U*& destPtr, interface_ref* srcPtr)
	{
		if ( srcPtr )
		{ srcPtr->incref(); }
		
		if ( destPtr )
		{
			if ( 0 == destPtr->decref() )
			{
				interface_ref* p = (interface_ref*)locked_exchange_pointer(destPtr, nullptr);
				safe_delete(p);
			}
		}
		
		locked_exchange_pointer(destPtr, srcPtr);
		return destPtr;
	}
};

/**
	@def spPtr ����Ʈ������ Ÿ���� �����ϰ� ������ �� �ְ� �����ִ� ��ũ�� ���� ��� PTR(MyObj)�� �����ϸ� MyObjPtr �̶�� ����Ʈ�����Ͱ� ����ȴ�.
*/
#define PTR(className)	class className; typedef spPtr<className> className##Ptr;