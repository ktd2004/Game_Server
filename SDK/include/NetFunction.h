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
#include <map>
#include <NetLink.h>
#include <Functional.h>

/** 
	네트웍 이벤트 발생시 호출할 콜백함수자 타입정의
*/
typedef functional_v2<bool, NetLinkPtr, Buffer*>	NetFunc_V2;

/** 
	네트웍 이벤트 함수를 관리하는 맵타입정의
*/
typedef std::map<unsigned int, NetFunc_V2* > NetEventFuncMap;

/** 
	패킷ID로 식별되는 네트웍이벤트함수를 관리하는 클래스

	NetEventHandler는 NetFunction 객체를 맴버로 가지고 있으면 SetNetFunc() 탬플릿 함수를
	사용하여 네트웍 이벤트함수를 NetFunction 객체에 등록한다.
*/
class NetFunction
{
protected:
	NetEventFuncMap		m_NetFuncMap;	///< 네트웍 이벤트 함수 관리맵

public:
	NetFunction();
	virtual ~NetFunction();

	/** 
		네트웍 이벤트 함수를 등록한다.
		@param iProtocol 패킷ID
		@param fn 네트웍 이벤트 함수를 콜백하는 함수자
	*/
	bool		Reg( unsigned int iProtocol, NetFunc_V2* fn );
	/** 
		네트웍 이벤트 함수를 호출한다.
		@param iProtocol 패킷ID
		@param spLink 패킷을 전송한 네트웍 링크
		@param pBuf 네트웍 함수에 전달할 패킷을 담고 있는 버퍼
	*/
	bool		Call( unsigned int iProtocol, NetLinkPtr spLink, Buffer* pBuf );
	/** 
		네트웍 이벤트 함수를 조회한다.
		@param iProtocol 패킷ID
	*/
	NetFunc_V2*	Find( unsigned int iProtocol );
};