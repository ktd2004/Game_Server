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
#include <NetLink.h>

/** 
	RPC.NET의 Proactor와 NetEventHandler에서 발생한 네트웍이벤트를 처리하는 추상클래스.

	Proactor와 NetEventHandler는 소켓에서 발생한 클라이언트 접속, 서버 연결, 패킷 송수신, 종료와 같은 네트웍이벤트들을
	처리하기 위해서 Listen(), Bind(), Connect() 함수 호출시 전달받은 NetCallback 객체의 네트웍이벤트 함수를 호출한다.
*/
struct NetCallback
{
	/**
		클라이언트가 연결되면 발생되는 네트웍 이벤트
		@param spLink 접속한 클라이언트 링크
	*/
	virtual void OnNetAccepted( NetLinkPtr spLink ) = 0;
	/**
		서버에 연결되면 발생되는 네트웍 이벤트
		@param 서버에 연결을 요청한 링크
	*/
	virtual void OnNetConnected( NetLinkPtr spLink ) = 0;
	/**
		패킷을 수신하면 발생되는 네트웍 이벤트
		@param spLink 데이타를 수신한 링크
		@param pBuf 수신받은 데이타를 저장하고 있는 버퍼
	*/
	virtual void OnNetReceived( NetLinkPtr spLink, Buffer* pBuf ) = 0;
	/**
		연결이 종료되면 발생되는 네트웍 이벤트
		@param spLink 연결이 종료된 링크
	*/
	virtual void OnNetClosed( NetLinkPtr spLink ) = 0;
};