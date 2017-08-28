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
	레퍼런스 카운트를 관리하는 클래스

	쓰레드간에 레퍼런스카운터의 증감을 동기화 시키는 클래스
*/
class shared_ref : public interface_ref
{
	volatile long m_ref;		///< 레퍼런스 카운트.

public:
	shared_ref() : m_ref(0) {}
	virtual ~shared_ref()	{}

	/** 레퍼런스카운트 1증가. */
	inline long incref(void)
	{
		return locked_inc(this->m_ref);
	}
	/** 레퍼런스카운트 지정된 설정값만큼 증가. */
	inline long incref(long r)
	{
		return locked_exchange_add(this->m_ref, r) + r;
	}
	/** 레퍼런스카운트 1감소. */
	inline long decref(void)
	{
		return locked_dec(this->m_ref);
	}
	/** 레퍼런스카운트 설정값 만큼 감소. */
	inline long decref(long r)
	{
		return locked_exchange_sub(this->m_ref, r);
	}
	/** 레퍼런스카운트 조회. */
	inline long refcount() { return this->m_ref; }
};

class single_ref : public interface_ref
{
	volatile long m_ref;		///< 레퍼런스 카운트.

public:
	single_ref() : m_ref(0) {}
	virtual ~single_ref()	{}

	/** 레퍼런스카운트 1증가. */
	inline long incref(void)
	{
		return ++this->m_ref;
	}
	/** 레퍼런스카운트 지정된 설정값만큼 증가. */
	inline long incref(long r)
	{
		this->m_ref += r;
		return this->m_ref;
	}
	/** 레퍼런스카운트 1감소. */
	inline long decref(void)
	{
		this->m_ref--;
		return this->m_ref;
	}
	/** 레퍼런스카운트 설정값 만큼 감소. */
	inline long decref(long r)
	{
		this->m_ref -= r;
		return this->m_ref;
	}
	/** 레퍼런스카운트 조회. */
	inline long refcount() { return this->m_ref; }
};

/** 
	스마트 포인터.

	spRef에 할당된 오브젝트는 spRef 가 삭제될때 같이 삭제된다.
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
	/** 저장된 클래스오브젝트를 조회 */
	inline T* get() { return (T*)this->m_p; }
	/** 저장된 클래스오브젝트를 분리 */
	inline T* detach()
	{
		T* p = (T*)locked_exchange_pointer(this->m_p, nullptr);
		return p;
	}
	/** 클래스오브젝트를 부착한다 */
	inline spRef& attach(T* p)
	{
		T* t = (T*)locked_exchange_pointer(this->m_p, p);
		safe_delete(t);
		return *this;
	}
	/** 저장된 클래스오브젝트를 조회 */
	inline operator T*() { return (T*)this->m_p; }
	/** 저장된 클래스오브젝트를 조회 */
	inline operator T*() const { return (T*)this->m_p; }
	/** 저장된 클래스오브젝트를 레퍼런스로 변환 */
	inline T& operator *() const { return *(T*)this->m_p; }
	/** 저장된 클래스오브젝트를 맴버 참조 */
	inline T* operator ->() const	{ return (T*)this->m_p; }
	/** 저장된 클래스오브젝트가 nullptr 인지 조사 */
	inline bool operator !() const { return (this->m_p == 0); }
	/** 클래스오브젝트가 동일한지 비교 */
	inline bool operator ==(T* p) const { return this->m_p == p; }
	/** 클래스오브젝트를 저장한다. */
	inline spRef& operator =(T* p)
	{
		T* t = (T*)locked_exchange_pointer(this->m_p, p);
		safe_delete(t);
		return *this;
	}
	/** 상대편 spRef에서 클래스오브젝트를 분리후 자신에 저장 */
	inline spRef& operator =(const spRef<T>& sp)
	{
		T* src = (T*)locked_exchange_pointer(sp.m_p, nullptr);
		T* t = (T*)locked_exchange_pointer(this->m_p, src);
		safe_delete(t);
		return *this;
	}	
};

/** 
	쓰레드간 참조 안전성을 보장하는 스마트포인터

	spPtr에 할당된 오브젝트는 spPtr이 레퍼런스 카운트를 감소시킨후 레퍼런스 카운트가 0 이 될때
	오브젝트를 삭제한다.
	객체의 레퍼런스카운트는 증감은 쓰레드간에 동기화되므로 멀티쓰레드간에도 안전한 삭제를 보장한다.
	아래의 코드는 spPtr 스마트포인터가 MyObj를 관리하기 위한 방법을 보여준다.

	@verbatim
	// MyObj 스마트포인터 MyObjPtr 타입을 선언하는 매크로.
	PTR(MyObj);
	// MyObj 를 spPtr 스마트포인터에 할당하기 위해서는 shared_ref 를 상속해야 한다.
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
		// MyObj 를 생성하여 스마트포인터에 할당한다.
		MyObjPtr	spObj(new MyObj);

		spObj->dump();

		// Func 함수가 종료되면 MyObj가 자동 삭제된다.
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

	/** 저장된 클래스오브젝트를 조회 */
	inline T*	get() { return (T*)this->m_p; }
	/** 저장된 클래스오브젝트를 분리 */
	inline T*	detach()
	{
		T* p = (T*)locked_exchange_pointer(this->m_p, nullptr);
		if ( p ) p->decref();
		return p;
	}
	/** 파라미터를 설정한다 */
	inline void* param( void ) { return m_param; }
	/** 파라미터를 조회한다. */
	inline void param( void* p ) { m_param = p; }
	/** 저장된 클래스오브젝트를 조회 */
	inline operator T*() { return (T*)this->m_p; }
	/** 저장된 클래스오브젝트를 조회 */
	inline operator T*() const { return (T*)this->m_p; }
	/** 저장된 클래스오브젝트를 레퍼런스로 변환 */
	inline T& operator *() const { return *(T*)this->m_p; }
	/** 저장된 클래스오브젝트를 맴버 참조 */
	inline T* operator ->() const { return (T*)this->m_p; }
	/** 클래스오브젝트가 동일한지 비교 */
	inline bool operator !() const { return (this->m_p == 0); }
	/** 클래스오브젝트가 동일한지 비교 */
	inline bool operator ==(T* p) const { return (this->m_p == p); }
	/** 클래스오브젝트를 저장한다. */
	inline spPtr& operator =(T* p)
	{
		assign(this->m_p, p);
		return *this;
	}
	/** 클래스오브젝트를 저장한다. */
	inline spPtr& operator =(const spPtr<T>& sp)
	{
		assign(this->m_p, sp.m_p);
		m_param = sp.m_param;
		return *this;
	}
	/** 스마트포인터 복사 */
	template <class U> spPtr(const spPtr<U>& sp)
		: m_p(0), m_param(0)
	{
		assign(this->m_p, sp.m_p);
		m_param = sp.m_param;
	}
	/** 스마트포인터 복사 */
	template <class U> spPtr& operator =(const spPtr<U>& sp)
	{
		assign(this->m_p, sp.m_p);
		m_param = sp.m_param;
		return *this;
	}

private:
	/** 스마트포인터에 클래스오브젝트 할당 */
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
	@def spPtr 스마트포인터 타입을 간편하게 선언할 수 있게 도와주는 매크로 예를 들면 PTR(MyObj)를 선언하면 MyObjPtr 이라는 스마트포인터가 선언된다.
*/
#define PTR(className)	class className; typedef spPtr<className> className##Ptr;