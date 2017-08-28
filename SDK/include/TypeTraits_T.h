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


/** 
	�� �߷� �� �� ��ȯ ���ø�.
*/
namespace tr
{
	/** ��,���� Ÿ�� ���� */
	template<class _Ty, _Ty _Val> 
	struct integral_constant
	{
		static const _Ty value = _Val;
		typedef _Ty value_type;
		typedef integral_constant<_Ty, _Val> type;
	};       
	/** �� Ÿ�� */
	typedef integral_constant<bool, true> true_type;
	/** ���� Ÿ��*/
	typedef integral_constant<bool, false> false_type;

	/** �ڷ����� ����ü���� ���� */
	template<bool C, typename A, typename B> struct if_ {};
	/** �ڷ����� ����ü */
	template<typename A, typename B>		struct if_<true, A, B> { typedef A type; };
	/** �ڷ����� ����ü �ƴ� */
	template<typename A, typename B>		struct if_<false, A, B> { typedef B type; };

	/** �ڷ����� ���������� ���� */
	template<typename A>
	struct is_ptr { static const bool value = false; };
	/** �ڷ����� ���������� ���� */
	template<typename A>
	struct is_ptr<A*> { static const bool value = true; };
	/** �ڷ����� ���������� ���� */
	template<typename A>
	struct is_ptr<const A*> { static const bool value = true; };

	/** �ڷ����� �����͸� ���� */
	template<typename A>
	struct remove_ptr { typedef A type; };
	/** �ڷ����� �����͸� ���� */
	template<typename A>
	struct remove_ptr<A*> { typedef A type; };
	/** �ڷ����� �����͸� ���� */
	template<typename A>
	struct remove_ptr<const A*> { typedef const A type; };

	/** �ڷ����� ���۷������� ���� */
	template<typename A>
	struct is_ref { static const bool value = false; };
	/** �ڷ����� ���۷������� ���� */
	template<typename A>
	struct is_ref<A&> { static const bool value = true; };
	/** �ڷ����� ���۷������� ���� */
	template<typename A>
	struct is_ref<const A&> { static const bool value = true; };
	/** �ڷ������� ���۷��� ���� */
	template<typename A>
	struct remove_ref { typedef A type; };
	/** �ڷ������� ���۷��� ���� */
	template<typename A>
	struct remove_ref<A&> { typedef A type; };

	/** �ڷ������� const ���� */
	template<typename A>
	struct remove_const { typedef A type; };
	/** �ڷ������� const ���� */
	template<typename A>
	struct remove_const<const A> { typedef A type; };
	/** �ڷ������� const ���� */
	template<typename A>
	struct remove_const<const A*> { typedef A* type; };

	/** ��,���� ���� */
	enum { no = 1, yes = 2 }; 
	/** �� �� ���� */
	typedef char (& no_type )[no]; 
	/** ���� �� ���� */
	typedef char (& yes_type)[yes]; 
	/* ��,���� �ڷ��� */
	template <bool C> struct bool_to_yesno { typedef no_type type; }; 
	/* ��,���� �ڷ��� */
	template <> struct bool_to_yesno<true> { typedef yes_type type; }; 

	/** �ڷ����� char���� ���� */
	yes_type tester_scalar(const volatile char);
	/** �ڷ����� unsigned char���� ���� */
	yes_type tester_scalar(const volatile unsigned char);
	/** �ڷ����� short���� ���� */
	yes_type tester_scalar(const volatile short);
	/** �ڷ����� unsigned short���� ���� */
	yes_type tester_scalar(const volatile unsigned short);
	/** �ڷ����� int���� ���� */
	yes_type tester_scalar(const volatile int);
	/** �ڷ����� unsigned int���� ���� */
	yes_type tester_scalar(const volatile unsigned int);
	/** �ڷ����� float���� ���� */
	yes_type tester_scalar(const volatile float);
	/** �ڷ����� double���� ���� */
	yes_type tester_scalar(const volatile double);
	/** �ڷ����� __int64���� ���� */
	yes_type tester_scalar(const volatile __int64);
	/** �ڷ����� unsigned __int64���� ���� */
	yes_type tester_scalar(const volatile unsigned __int64);
	/** �ڷ����� ��Į�� �ƴ��� ���� */
	no_type	 tester_scalar(...);	
	/** �ڷ����� ��Į������ �ƴ��� ���� */
	template <typename T> 
	struct is_scalar
	{ 
		static T arg; 
		static const bool value = sizeof(tester_scalar(arg)) == sizeof(yes_type);
	};

	/** 
		const, &, * ������ �� �������� �����Ѵ�.
	*/
	template<typename T>
	struct base_type { typedef T type; };
	/** 
		const, &, * ������ �� �������� �����Ѵ�.
	*/
	template<typename T>
	struct base_type<const T> { typedef T type;};
	/** 
		const, &, * ������ �� �������� �����Ѵ�.
	*/
	template<typename T>
	struct base_type<T*> { typedef T type; };
	/** 
		const, &, * ������ �� �������� �����Ѵ�.
	*/
	template<typename T>
	struct base_type<const T*> { typedef T type;};
	/** 
		const, &, * ������ �� �������� �����Ѵ�.
	*/
	template<typename T>
	struct base_type<T&> { typedef T type;};
	/** 
		const, &, * ������ �� �������� �����Ѵ�.
	*/
	template<typename T>
	struct base_type<const T&> { typedef T type;};
};

/** 
	�Ű������� �ּҰ� �� ��� �ּҰ��� ��ȯ�Ѵ�.
*/
template<typename T> T proc_get_args(typename tr::base_type<T>::type& arg1, tr::true_type )
{
	return &arg1;
}
/** 
	�Ű������� instance�� ��� instance�� ��ȯ�Ѵ�.
*/
template<typename T> typename tr::base_type<T>::type& proc_get_args(typename tr::base_type<T>::type& arg1, tr::false_type )
{
	return arg1;
}