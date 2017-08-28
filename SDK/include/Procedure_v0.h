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
	클래스 맴버함수를 비동기 프로시져로 정의하는 함수자 탬플릿.
	
	클래스의 맴버함수를 콜백하는 함수를 구현한다.
*/
struct procedure_v0 : public procedure_base
{
	functional_v1<bool,interface_ref*>	m_fn;	///< 콜백 함수자
	procedure_v0()
	{
	}
	procedure_v0(Buffer* pBuf)
	{
	}
	/**
		비동기 프로시져 호출시 전달할 매개변수를 내부 저장공간으로 복사한다.
		@param pBuf 매개변수를 저장하고 있는 버퍼의 주소값
	*/
	void stream(Buffer* pBuf) 
	{
	}
	/**
		비동기 프로시져로 등록된 함수자를 호출한다.
		@param pCaller 비동기 프로시져를 호출한 객체의 주소값으로 interface_ref 를 상속받아야 한다.
	*/
	bool call(interface_ref* pCaller)
	{
		if (m_fn)
		{
			return m_fn(pCaller);
		}
		return false;
	}
};