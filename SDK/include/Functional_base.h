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

/** 
	함수자의 최상위 클래스.
*/
class functional_base
{
public:
	functional_base() {}
	virtual ~functional_base() {}
};


/** 
	@def 함수자에 함수객체를 대입, 삭제하는 맴버함수를 정의하는 매크로.
*/
#define implement_functional(subfix) \
functional_##subfix() : m_impl(0) {}; \
template<class F> \
functional_##subfix(F f) : m_impl(0) \
{ \
	assign(f); \
} \
template<class O, class F> \
functional_##subfix(const O& o, F f) : m_impl(0) \
{ \
	assign(o, f); \
} \
virtual ~functional_##subfix() \
{ \
	clear(); \
} \
template<class F> \
void assign(F f) \
{ \
	clear(); \
	m_impl = new static_function_object<F>(f); \
} \
template<class O, class F> \
void assign(const O& o, F f) \
{ \
	clear(); \
	m_impl = new member_function_object<O, F>(o, f); \
} \
void clear() \
{  \
	if ( m_impl ) delete m_impl; m_impl = nullptr; \
} \
inline bool operator ! () const \
{ \
	return m_impl == nullptr; \
} \
inline operator bool () const \
{ \
	return (m_impl != nullptr && m_impl->null() != false); \
} \
private: \
	function_object_interface*		m_impl


/** 
	@def 함수자의 맴버함수를 구현한 매크로
*/
#define implement_mfunctional(subfix) \
mfunctional_##subfix() : m_Func(nullptr), m_Inst(nullptr) {} \
virtual ~mfunctional_##subfix() { clear(); } \
void assign( T *inst, FuncPtr fn) \
{ \
	m_Func = fn; \
	m_Inst = inst; \
} \
void clear() \
{ \
	m_Func = nullptr; \
	m_Inst = nullptr; \
} \
inline bool operator ! () const \
{ \
	return m_Inst == nullptr || m_Func == nullptr; \
} \
inline operator bool () const \
{ \
	return m_Inst != nullptr && m_Func != nullptr; \
} \
private: \
	T*			m_Inst; \
	FuncPtr		m_Func;