#pragma once

//<
#include <NetLink.h>

/**
	p2p 네트웍 이벤트 핸들러

	원격 pc 에서 발생하는 네트웍 이벤트를 처리하는 인터페이스를 정의한다.
*/
struct P2PAgentHandler
{
	/**
		원격 Pc 가 접속하면 호출되는 이벤트 함수
		@param spLink 홀펀칭이 완료된 원격 pc의 NetLink
	*/
	virtual bool			OnConnected( NetLinkPtr spLink ) = 0;
	/**
		원격 Pc와 연결이 종료되면 호출되는 이벤트 함수
		@param spLink 연결이 종료된 원격 pc의 NetLink
	*/
	virtual bool			OnClosed( NetLinkPtr spLink ) = 0;
	/**
		원격 Pc에서 p2p 패킷을 수신하면 호출되는 이벤트 함수
		@param spLink 패킷을 전송한 원격 pc의 NetLink
		@param 수신된 패킷을 저장한 버퍼의 주소값
	*/
	virtual bool			OnReceived( NetLinkPtr spLink, Buffer* pBuf ) = 0;
};

/** 
	원격 pc 와 홀펀칭, 연결 유지를 수행하면 원격 pc 로 reliable/unreliable 패킷 송수신하는 인터페이스를 제공한다.
*/
struct P2PAgent
{
	/** 
		홀펀칭이 완료된 원격 pc 들이 저장된 NetLink 관리맵을 조회한다.
	*/
	virtual NetLinkMap&		GetNetLinkMap(void ) = 0;
	/** 
		P2P 패킷을 송수신하는 UDP 소캣의 네트웍 정보를 얻어온다.
	*/
	virtual Network_IF&		Self( void ) = 0;
	/** 
		원격 pc 와 홀펀칭을 수행한다.
		@param nif 원격의 P2PAgent의 네트웍 주소
	*/
	virtual NetLink*		Connect( const Network_IF& nif ) = 0;
	/** 
		원격 pc 와 연결을 종료한다.
		@param pLink 원격 pc의 NetLink
	*/
	virtual bool			Close( NetLink* pLink ) = 0;
	/** 
		원격 pc 를 찾는다.
		@param nif 원격의 P2PAgent의 네트웍 주소
	*/
	virtual NetLink*		Find( const Network_IF& nif ) = 0;
	/** 
		p2p 패킷 릴레이 서버의 NetLink를 조회한다
	*/
	virtual NetLink*		Relay( void ) = 0;
	/** 
		원격 pc 들이 저장된 NetLink 관리맵을 모두 지운다.
	*/
	virtual void			Clear( void ) = 0;
	/** 
		홀펀칭, 연결유지, reliable/unreliable 패킷을 송수신한다.
		@param iWait 패킷 수신 대기 시간
	*/
	virtual void			Process( unsigned long iWait ) = 0;
};

/** 
	P2PAgent 를 생성하는 전역함수를 제공하는 네임스페이스.
*/
struct P2PAgentFactory
{
	/** 
		P2PAgent 객체를 생성한다.
		@param rSelf 자신의 네트웍 정보
		@param pHandler 네트웍 이벤트를 처리할 핸들러 객체의 주소값
		@param iMaxBufferSize 송수신패킷의 최대 크기
		@param iKeepConnection 연결유지 패킷 송수신 간격(ms)
	*/
	static P2PAgent*		Create( const Network_IF& rSelf, P2PAgentHandler* pHandler,
								unsigned int iMaxBufferSize = 4096, unsigned long iKeepConnection = 1000 );
};