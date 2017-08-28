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

#include <P2PAgent.h>

/** 
	랑데뷰/패킷릴레이 서버를 생성하는 전역함수를 제공하는 네임스페이스.
*/
struct RendezvousAgentFactory
{
	/**
		랑데뷰/패킷 릴레이 서버를 생성한다.
		@param rSelf 자신의 네트웍 정보
		@param iMaxBufferSize 송수신패킷의 최대 크기
		@param iKeepConnection 연결유지 패킷 송수신 간격(ms)
	*/
	static P2PAgent*	Create( const Network_IF& rSelf, unsigned int iMaxBufferSize = 4096, unsigned long iKeepConnection = 1000 );
	/**
		랑데뷰/패킷 릴레이 서버에 연결한다.
		@param rSelf 자신의 네트웍 정보
		@param pHandler 네트웍 이벤트를 처리할 핸들러 객체의 주소값
		@param rRendezvous 랑데뷰,패킷릴레이서버의 네트웍 정보
		@param iMaxBufferSize 송수신패킷의 최대 크기
		@param iKeepConnection 연결유지 패킷 송수신 간격(ms)
	*/
	static P2PAgent*	Join( const Network_IF& rSelf, P2PAgentHandler* pHandler, const Network_IF& rRendezvous,
							unsigned int iMaxBufferSize = 4096, unsigned long iKeepConnection = 1000 );
};