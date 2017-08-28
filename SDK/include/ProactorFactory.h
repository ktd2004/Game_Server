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
#include <NetCallback.h>
#include <Functional.h>
#include <RPC_Func.h>
#include <functional>

/** 
	IOCP기반의 Proactor 패턴으로 구현된 네트웍이벤트를 처리하는 클래스
	
	TCP/IP, UDP 소켓 생성, 데이타 송수신을 위한 인터페이스를 제공하며, 소켓에서 발생한 이벤트를
	NetEventHandler로 전달하는 기능을 수행한다.
*/
struct Proactor
{
	struct Property
	{
		std::string		sHost;			///< 호스트주소
		unsigned short	iPort;			///< 포트
		long			iCreatedSockets;///< 접속 대기 NetLink의 개수
		NetCallback*	pNetCallback;	///< 네트웍 이벤트 처리 객체
		size_t			iMaxBufSize;	///< 수신 버퍼 크기
		std::function<void(NetLinkPtr, bool)>* fnOnCreated;	///< NetLink 생성시 호출되는 콜백
		Property()
			: sHost("127.0.0.1")
			, iPort(65533)
			, iCreatedSockets(100)
			, pNetCallback(nullptr)
			, iMaxBufSize(4096)
			, fnOnCreated(nullptr)
		{}
		Property( const Property& r )
		{
			this->sHost				= r.sHost;
			this->iPort				= r.iPort;
			this->iCreatedSockets	= r.iCreatedSockets;
			this->pNetCallback		= r.pNetCallback;
			this->iMaxBufSize		= r.iMaxBufSize;
			this->fnOnCreated		= r.fnOnCreated;
		}
	};

	/**
		Proactor 생성 시점에 할당된 고유ID를 조회한다. 
	*/
	virtual unsigned int	ID( void ) = 0;
	/**
		TCP/IP 연결을 구축할 준비를 한다.
		@param rProp 클라이언트가 접속할 네트웍주소, 접속대기 NetLink의 개수, 네트웍 이벤트를 처리할 객체를 설정한다.

		@verbatim

		class MyObj : public NetCallback
		{
		public:
			MyObj() {}
			virtual ~MyObj() {}
			void OnNetAccepted( NetLinkPtr spLink )
			{
			}
			void OnNetConnected( NetLinkPtr spLink )
			{
			}
			void OnNetReceived( NetLinkPtr spLink, Buffer* pBuf )
			{
			}
			void OnNetClosed( NetLinkPtr spLink )
			{
			}
		};

		int _tmain( int argc, char* argv[] )
		{
			MyObj evh;

			Proactor* pNetwork = ProactorFactory::Create( 0, 2, 1 );
			ProactorFactory::Start( pNetwork->ID() );

			Proactor::Property propCfg;
			propCfg.sHost = "localhost";
			propCfg.iPort = 25533;
			propCfg.iCreatedSockets = 100;
			propCfg.pNetCallback = &evh;
			propCfg.iMaxBufSize = 4096;

			pNetwork->Listen( propCfg );
		}
		@endverbatim
	*/
	virtual bool			Listen( const Proactor::Property& rProp ) = 0;
	/**
		원격 호스트에 접속한다.
		@param rProp 클라이언트가 접속할 네트웍주소

		@verbatim

		class MyObj : public NetCallback
		{
		public:
			MyObj() {}
			virtual ~MyObj() {}
			void OnNetAccepted( NetLinkPtr spLink )
			{
			}
			void OnNetConnected( NetLinkPtr spLink )
			{
			}
			void OnNetReceived( NetLinkPtr spLink, Buffer* pBuf )
			{
			}
			void OnNetClosed( NetLinkPtr spLink )
			{
			}
		};

		int _tmain( int argc, char* argv[] )
		{
			MyObj evh;

			Proactor* pNetwork = ProactorFactory::Create( 0, 2, 1 );
			ProactorFactory::Start( pNetwork->ID() );

			Proactor::Property propCfg;
			propCfg.sHost = "localhost";
			propCfg.iPort = 25533;
			propCfg.pNetCallback = &evh;
			propCfg.iMaxBufSize = 4096;

			NetLinkPtr spLink = pNetwork->Connect( propCfg );
		}
		@endverbatim
	*/
	virtual NetLinkPtr		Connect( const Proactor::Property& rProp ) = 0;
	/**
		UDP 소켓을 생성하여 바인드한다.
		@param rProp 클라이언트가 접속할 네트웍주소

		@verbatim

		class MyObj : public NetCallback
		{
		public:
			MyObj() {}
			virtual ~MyObj() {}
			void OnNetAccepted( NetLinkPtr spLink )
			{
			}
			void OnNetConnected( NetLinkPtr spLink )
			{
			}
			void OnNetReceived( NetLinkPtr spLink, Buffer* pBuf )
			{
			}
			void OnNetClosed( NetLinkPtr spLink )
			{
			}
		};

		int _tmain( int argc, char* argv[] )
		{
			MyObj evh;

			Proactor* pNetwork = ProactorFactory::Create( 0, 2, 1 );
			ProactorFactory::Start( pNetwork->ID() );

			Proactor::Property propCfg;
			propCfg.sHost = "localhost";
			propCfg.iPort = 25533;
			propCfg.pNetCallback = &evh;
			propCfg.iMaxBufSize = 4096;

			NetLinkPtr spLink = pNetwork->Bind( propCfg );
		}
		@endverbatim
	*/
	virtual NetLinkPtr		Bind( const Proactor::Property& rProp ) = 0;
	/** 
		원격에서 접속한 클라이언트를 모두 종료한다.
	*/
	virtual void			KickAll( unsigned short iKickReason ) = 0;
	/** 
		Proactor를 종료한다.
	*/
	virtual void			Finalize( void ) = 0;
	/** 
		Proactor의 IOCP 핸들을 얻어온다.
	*/
	virtual HANDLE			GetHandle( void ) = 0;
	/** 
		Proactor의 NetLink 관리 맵을 얻어온다.
		@param iLinkType NetLink맵의 종류
		@li eLINKTYPE_UDP			UDP NetLink 맵
		@li eLinkType_TCP_LISTEN	서버의 접속 대기 NetLink 맵
		@li eLINKTYPE_TCP_REMOTE	서버에서 클라이언트에 대응되는 NetLink 맵
		@li eLINKTYPE_TCP_CONNECT	클라이언트 NetLink 맵
	*/
	virtual NetLinkMap&		GetNetLinkMap( unsigned short iLinkType ) = 0;
	/** 
		Proactor에 NetLink 관리맵에 NetLink를 등록한다.
		@param spLink NetLink 관리맵에 등록할 NetLink
	*/
	virtual bool			RegLink( NetLinkPtr spLink ) = 0;
	/** 
		Proactor에 NetLink 관리맵에서 NetLink를 제거한다.
		@param spLink NetLink 관리맵에서 제거할 NetLink
	*/
	virtual bool			UnregLink( NetLinkPtr spLink ) = 0;
	/** 
		Proactor의 NetLink 관리 맵에 등록된 NetLink의 개수를 조회한다.
		@param iLinkType NetLink맵의 종류
		@li eLINKTYPE_UDP			UDP NetLink 맵
		@li eLinkType_TCP_LISTEN	서버의 접속 대기 NetLink 맵
		@li eLINKTYPE_TCP_REMOTE	서버에서 클라이언트에 대응되는 NetLink 맵
		@li eLINKTYPE_TCP_CONNECT	클라이언트 NetLink 맵
	*/
	virtual size_t			GetCount( unsigned short iLinkType ) = 0;
};

/** 
	Proactor의 생성, 시작, 종료 및 성능정보 수집을 수행하는 전역함수를 제공하는 네임스페이스.
*/
namespace ProactorFactory
{
	/** 
		Proactor가 생성하면 Proactor 관리맵에 종료 우선순위에 따라 등록되어 종료 순서를 보장받는다.
		@param iTerminatePriority 종료 우선순위
		@param iBindCPU 네트웍쓰레드를 실행한 CPU
		@param iThreadCnt 네트웍쓰레드 개수
	*/
	Proactor*		Create( int iTerminatePriority, unsigned short iThreadCnt, unsigned short iBindCPU = 0 );
	/** 
		Proactor 객체를 조회한다.
		@param iProactorID Proactor의 고유ID
	*/
	Proactor*		Get( unsigned int iProactorID );
	/** 
		Proactor를 실행한다.
		@param iProactorID Proactor의 고유ID, 만약 0 이면 Proactor 관리맵에 등록된 모든 Proactor를 실행한다.
	*/
	bool			Start( unsigned int iProactorID  = 0 );
	/** 
		Proactor를 종료우선순위 에 따라 순차적으로 종료한다.
		@param iProactorID Proactor의 고유ID, 만약 0 이면 Proactor 관리맵에 등록된 모든 Proactor를 종료한다.
	*/
	bool			Stop( unsigned int iProactorID  = 0 );
};