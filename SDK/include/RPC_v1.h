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

#include <RPC_base.h>

/** 
	Ŭ������ �ɹ��Լ��� ���� ���ν����� �����ϴ� �Լ��� ���ø����� Ŭ������ �ɹ��Լ��� �ݹ��ϴ�
	�Լ��� �����Ѵ�.
*/
template<typename T1>
struct rpc_v1 : public rpc_base
{
	typename tr::base_type<T1>::type	arg1;	///< �ݹ� �Լ��ڿ� ������ �Ű����� 1
	functional_v2<bool, NetLinkPtr, T1>		m_fn;	///< �ݹ� �Լ���

	rpc_v1()
	{
	}

	rpc_v1(Buffer* pBuf)
	{
		stream(pBuf);
	}
	/**
		���ۿ� ����� �Ű������� �Ű����� ����������� �����Ѵ�.
		@param pBuf �Ű������� �����ϰ� �ִ� ������ �ּҰ�
	*/
	void stream(Buffer* pBuf)
	{
		*pBuf >> arg1;
	}
	/**
		���� ���ν����� ��ϵ� �Լ��ڸ� ȣ���Ѵ�.
		@param pCaller �񵿱� ���ν����� ȣ���� ��ü�� �ּҰ����� interface_ref �� ��ӹ޾ƾ� �Ѵ�.
	*/
	bool call(NetLink* pCaller)
	{
		if (m_fn)
		{
			return m_fn(
				pCaller,
				proc_get_args<T1>(arg1, tr::integral_constant<bool, tr::is_ptr<T1>::value>()) );
		}
		return false;
	}
};
