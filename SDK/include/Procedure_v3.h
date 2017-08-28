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

#include <Procedure_base.h>

/** 
	Ŭ���� �ɹ��Լ��� �񵿱� ���ν����� �����ϴ� �Լ��� ���ø�.
	
	Ŭ������ �ɹ��Լ��� �ݹ��ϴ� �Լ��� �����Ѵ�.
*/
template<typename T1, typename T2, typename T3>
struct procedure_v3 : public procedure_base
{
	typename tr::base_type<T1>::type	arg1;		///< �ݹ� �Լ��ڿ� ������ �Ű����� 1
	typename tr::base_type<T2>::type	arg2;		///< �ݹ� �Լ��ڿ� ������ �Ű����� 2
	typename tr::base_type<T3>::type	arg3;		///< �ݹ� �Լ��ڿ� ������ �Ű����� 3
	functional_v4<bool, interface_ref*, T1, T2, T3>	m_fn;	///< �ݹ� �Լ���

	procedure_v3()
	{
	}

	procedure_v3(Buffer* pBuf)
	{
		stream(pBuf);
	}
	/**
		�񵿱� ���ν��� ȣ��� ������ �Ű������� ���� ����������� �����Ѵ�.
		@param pBuf �Ű������� �����ϰ� �ִ� ������ �ּҰ�
	*/
	void stream(Buffer* pBuf)
	{
		*pBuf >> arg1 >> arg2 >> arg3;
	}
	/**
		�񵿱� ���ν����� ��ϵ� �Լ��ڸ� ȣ���Ѵ�.
		@param pCaller �񵿱� ���ν����� ȣ���� ��ü�� �ּҰ����� interface_ref �� ��ӹ޾ƾ� �Ѵ�.
	*/
	bool call(interface_ref* pCaller)
	{
		if (m_fn)
		{
			return m_fn(
				pCaller,
				proc_get_args<T1>(arg1, tr::integral_constant<bool, tr::is_ptr<T1>::value>()),
				proc_get_args<T2>(arg2, tr::integral_constant<bool, tr::is_ptr<T2>::value>()),
				proc_get_args<T3>(arg3, tr::integral_constant<bool, tr::is_ptr<T3>::value>()) );
		}
		return false;
	}
};