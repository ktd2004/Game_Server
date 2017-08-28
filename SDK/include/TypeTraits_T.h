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
	형 추론 및 형 변환 템플릿.
*/
namespace tr
{
	/** 참,거짓 타입 정의 */
	template<class _Ty, _Ty _Val> 
	struct integral_constant
	{
		static const _Ty value = _Val;
		typedef _Ty value_type;
		typedef integral_constant<_Ty, _Val> type;
	};       
	/** 참 타입 */
	typedef integral_constant<bool, true> true_type;
	/** 거짓 타입*/
	typedef integral_constant<bool, false> false_type;

	/** 자료형이 구조체인지 조사 */
	template<bool C, typename A, typename B> struct if_ {};
	/** 자료형이 구조체 */
	template<typename A, typename B>		struct if_<true, A, B> { typedef A type; };
	/** 자료형이 구조체 아님 */
	template<typename A, typename B>		struct if_<false, A, B> { typedef B type; };

	/** 자료형이 포인터인지 조사 */
	template<typename A>
	struct is_ptr { static const bool value = false; };
	/** 자료형이 포인터인지 조사 */
	template<typename A>
	struct is_ptr<A*> { static const bool value = true; };
	/** 자료형이 포인터인지 조사 */
	template<typename A>
	struct is_ptr<const A*> { static const bool value = true; };

	/** 자료형의 포인터를 제거 */
	template<typename A>
	struct remove_ptr { typedef A type; };
	/** 자료형의 포인터를 제거 */
	template<typename A>
	struct remove_ptr<A*> { typedef A type; };
	/** 자료형의 포인터를 제거 */
	template<typename A>
	struct remove_ptr<const A*> { typedef const A type; };

	/** 자료형이 레퍼런스인지 조사 */
	template<typename A>
	struct is_ref { static const bool value = false; };
	/** 자료형이 레퍼런스인지 조사 */
	template<typename A>
	struct is_ref<A&> { static const bool value = true; };
	/** 자료형이 레퍼런스인지 조사 */
	template<typename A>
	struct is_ref<const A&> { static const bool value = true; };
	/** 자료형에서 레퍼런스 제거 */
	template<typename A>
	struct remove_ref { typedef A type; };
	/** 자료형에서 레퍼런스 제거 */
	template<typename A>
	struct remove_ref<A&> { typedef A type; };

	/** 자료형에서 const 제거 */
	template<typename A>
	struct remove_const { typedef A type; };
	/** 자료형에서 const 제거 */
	template<typename A>
	struct remove_const<const A> { typedef A type; };
	/** 자료형에서 const 제거 */
	template<typename A>
	struct remove_const<const A*> { typedef A* type; };

	/** 참,거짓 선언 */
	enum { no = 1, yes = 2 }; 
	/** 참 형 선언 */
	typedef char (& no_type )[no]; 
	/** 거짓 형 선언 */
	typedef char (& yes_type)[yes]; 
	/* 참,거짓 자료형 */
	template <bool C> struct bool_to_yesno { typedef no_type type; }; 
	/* 참,거짓 자료형 */
	template <> struct bool_to_yesno<true> { typedef yes_type type; }; 

	/** 자료형이 char인지 조사 */
	yes_type tester_scalar(const volatile char);
	/** 자료형이 unsigned char인지 조사 */
	yes_type tester_scalar(const volatile unsigned char);
	/** 자료형이 short인지 조사 */
	yes_type tester_scalar(const volatile short);
	/** 자료형이 unsigned short인지 조사 */
	yes_type tester_scalar(const volatile unsigned short);
	/** 자료형이 int인지 조사 */
	yes_type tester_scalar(const volatile int);
	/** 자료형이 unsigned int인지 조사 */
	yes_type tester_scalar(const volatile unsigned int);
	/** 자료형이 float인지 조사 */
	yes_type tester_scalar(const volatile float);
	/** 자료형이 double인지 조사 */
	yes_type tester_scalar(const volatile double);
	/** 자료형이 __int64인지 조사 */
	yes_type tester_scalar(const volatile __int64);
	/** 자료형이 unsigned __int64인지 조사 */
	yes_type tester_scalar(const volatile unsigned __int64);
	/** 자료형이 스칼라가 아닌지 조사 */
	no_type	 tester_scalar(...);	
	/** 자료형이 스칼라인지 아닌지 조사 */
	template <typename T> 
	struct is_scalar
	{ 
		static T arg; 
		static const bool value = sizeof(tester_scalar(arg)) == sizeof(yes_type);
	};

	/** 
		const, &, * 제거한 후 원시형을 정의한다.
	*/
	template<typename T>
	struct base_type { typedef T type; };
	/** 
		const, &, * 제거한 후 원시형을 정의한다.
	*/
	template<typename T>
	struct base_type<const T> { typedef T type;};
	/** 
		const, &, * 제거한 후 원시형을 정의한다.
	*/
	template<typename T>
	struct base_type<T*> { typedef T type; };
	/** 
		const, &, * 제거한 후 원시형을 정의한다.
	*/
	template<typename T>
	struct base_type<const T*> { typedef T type;};
	/** 
		const, &, * 제거한 후 원시형을 정의한다.
	*/
	template<typename T>
	struct base_type<T&> { typedef T type;};
	/** 
		const, &, * 제거한 후 원시형을 정의한다.
	*/
	template<typename T>
	struct base_type<const T&> { typedef T type;};
};

/** 
	매개변수가 주소값 일 경우 주소값을 반환한다.
*/
template<typename T> T proc_get_args(typename tr::base_type<T>::type& arg1, tr::true_type )
{
	return &arg1;
}
/** 
	매개변수가 instance일 경우 instance를 반환한다.
*/
template<typename T> typename tr::base_type<T>::type& proc_get_args(typename tr::base_type<T>::type& arg1, tr::false_type )
{
	return arg1;
}