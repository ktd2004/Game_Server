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

#include <Functional_base.h>

/** 
	ÇÔ¼öÀÚ ÅÆÇÃ¸´
*/
template<class R, class T1, class T2, class T3, class T4, class T5, class T6>
class functional_v6 : public functional_base
{
	class function_object_interface
	{
	public:
		function_object_interface() {}
		virtual ~function_object_interface() {}
		virtual bool null() = 0;
		virtual R operator ()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) = 0;
	};

	template<class O, class F>
	class member_function_object : public function_object_interface
	{
	public:
		member_function_object(const O& o, F f) : m_pO(o), m_F(f) {}
		virtual ~member_function_object() {}
		bool null()
		{
			return (bool)m_F;
		}
		R operator ()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
		{
			return ((*m_pO).*m_F)(t1, t2, t3, t4, t5, t6);
		}
	private:
		O m_pO;
		F m_F;
	};

	template<class F>
	class static_function_object : public function_object_interface
	{
	public:
		static_function_object(F f) : m_F(f) {}
		virtual ~static_function_object() {}
		bool null()
		{
			return (bool)m_F;
		}
		R operator ()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
		{
			return m_F(t1, t2, t3, t4, t5, t6);
		}
	private:
		F m_F;
	};

public:
	implement_functional(v6);

public:
	R operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
	{
		return (*m_impl)(t1, t2, t3, t4, t5, t6);
	}
};

/** 
	ÇÔ¼öÀÚ ÅÆÇÃ¸´
*/
template<class T, class R, class A, class B, class C, class D, class E, class F>
class mfunctional_v6 : public functional_base
{
public:
	typedef R (T::*FuncPtr)(A,B,C,D,E,F);
	implement_mfunctional(v6);
public:
	R operator() (A a1, B b1, C c1, D d1, E e1, F f1)
	{
		return (m_Inst->*m_Func)(a1, b1, c1, d1, e1, f1);
	}
};