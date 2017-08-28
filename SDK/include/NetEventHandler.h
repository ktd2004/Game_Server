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
#include <map>
//<
#include <NetFunction.h>
#include <ProactorFactory.h>
#include <TaskSchedulerFactory.h>

/** 
	네트웍 이벤트 발생시 호출할 콜백함수자 타입정의
*/
typedef functional_v1<bool, NetLinkPtr>	NetFunc_V1;


/** 
	원격 프로시져 관리맵 타입정의
*/
typedef std::map<unsigned int, rpc_base*> RemoteProcedureMap;	

/** 
	수신한 패킷을 분석하여 작업이벤트를 생성한후 태스크스케줄러로 전달하는 클래스.

	NetEventHandler는 Proactor의 Wrapper 클래스로 Proactor에서 수신한 패킷를 분석하여 작업이벤트로 만들어 
	태스크스케줄러에서 전달하는 기능을 수행한다.
*/
class NetEventHandler : public NetCallback
{
protected:
	RemoteProcedureMap	m_RemoteProceduerMap;	///< 원격프로시져 관리 맵
	NetFunction			m_Functor;				///< 네트웍 함수 관리자
	Proactor*			m_pProactor;			///< 네트웍 이벤트 처리를 위한 Proactor
	TaskScheduler*		m_pTaskScheduler;		///< 작업이벤트를 처리할 태스크스케줄러
	unsigned int		m_iTaskSchedulerID;		///< 테스크스케줄러ID
	NetFunc_V1			m_OnAccepted;			///< 클라이언트가 연결되면 호출할 콜백한수자
	NetFunc_V1			m_OnConnected;			///< 서버에 연결되면 호출할 콜백함수자
	NetFunc_V1			m_OnClosed;				///< 연결이 종료되면 호출할 콜백함수자
	NetFunc_V2			m_OnReceived;			///< 패킷을 수신하면 호출할 콜백함수자

	//<
	NetEventHandler(const NetEventHandler& r) {}

public:
	NetEventHandler( unsigned int iTaskSchedulerID );
	virtual ~NetEventHandler();

	/**
		클라이언트가 연결되면 호출할 콜백함수 설정한다
		@param o 클래스 객체의 주소값
		@param f 클래스 객체의 맴버함수의 주소값
		@verbatim
		class MyObj
		{
		public:
			MyObj() {}
			virtual ~MyObj() {}

			bool OnAccepted( NetLinkPtr spLink )
			{
				return true;
			}
		};

		int _tmain( int argc, char* argv[] )
		{
			MyObj inst;
			NetEventHandler evh(1);
			evh.SetAcceptedFunc( &inst, &MyObj::OnAccepted );
			getchar();
			return 0;
		}
		@endverbatim
	*/
	template<typename O, typename F>
	inline void SetAcceptedFunc(const O& o, F f)
	{ m_OnAccepted.assign( o, f ); }
	/**
		연결이 종료되면 호출할 콜백함수를 설정한다.
		@param o 클래스 객체의 주소값
		@param f 클래스 객체의 맴버함수의 주소값
		@verbatim
		class MyObj
		{
		public:
			MyObj() {}
			virtual ~MyObj() {}

			bool OnClosed( NetLinkPtr spLink )
			{
				return true;
			}
		};

		int _tmain( int argc, char* argv[] )
		{
			MyObj inst;
			NetEventHandler evh(1);
			evh.SetClosedFunc( &inst, &MyObj::OnClosed );
			getchar();
			return 0;
		}
		@endverbatim
	*/
	template<typename O, typename F>
	inline void SetClosedFunc(const O& o, F f)
	{ m_OnClosed.assign( o, f );} 
	/**
		서버에 연결되면 호출할 콜백함수를 설정한다
		@param o 클래스 객체의 주소값
		@param f 클래스 객체의 맴버함수의 주소값
		@verbatim
		class MyObj
		{
		public:
			MyObj() {}
			virtual ~MyObj() {}

			bool OnConnected( NetLinkPtr spLink )
			{
				return true;
			}
		};

		int _tmain( int argc, char* argv[] )
		{
			MyObj inst;
			NetEventHandler evh(1);
			evh.SetConnectedFunc( &inst, &MyObj::OnConnected );
			getchar();
			return 0;
		}
		@endverbatim
	*/
	template<typename O, typename F>
	inline void SetConnectedFunc(const O& o, F f)
	{ m_OnConnected.assign( o, f ); }
	/**
		네트웍에서 패킷을 수신하면 호출할 콜백함수를 설정한다. 만약 콜백할 함수가 설정되면 네트웍에서
		발생한 모든 패킷을 우선 처리하므로 네트웍이벤트함수와 원격프로시져 호출이 수행되지 않는다.
		@param o 클래스 객체의 주소값
		@param f 클래스 객체의 맴버함수의 주소값
		@verbatim
		class MyObj
		{
		public:
			MyObj() {}
			virtual ~MyObj() {}

			bool OnReceieved( NetLinkPtr spLink, Buffer* pBuf )
			{
				return true;
			}
		};

		int _tmain( int argc, char* argv[] )
		{
			MyObj inst;
			NetEventHandler evh(1);
			evh.SetReceivedFunc( &inst, &MyObj::OnReceieved );
			getchar();
			return 0;
		}
		@endverbatim
	*/
	template<typename O, typename F>
	inline void SetReceivedFunc(const O& o, F f)
	{ m_OnReceived.assign( o, f ); }

	/**
		패킷ID로 식별되는 네트웍이벤트함수를 설정한다. 만약 네트웍 이벤트 함수의 패킷ID와 원격프로시져의 ID가
		중복될 경우 네트웍이벤트함수가 호출 우선순위를 갖기 때문에 원격프로시져는 호출되지 않는다.
		@param o 클래스 객체의 주소값
		@param f 클래스 객체의 맴버함수의 주소값
		@verbatim
		class MyObj
		{
		public:
			MyObj()	{}
			virtual ~MyObj() {}

			bool OnTest_NetFunc( NetLinkPtr spLink, Buffer* pBuf )
			{
				return true;
			}
		};

		int _tmain( int argc, char* argv[] )
		{
			MyObj inst;
			NetEventHandler evh(1);
			evh.SetNetFunc( 20000, &inst, &MyObj::OnTest_NetFunc );
			getchar();
			return 0;
		}
		@endverbatim
	*/
	template<typename O, typename F>
	inline void SetNetFunc( unsigned int iProtocol, const O& o, F f)
	{ m_Functor.Reg( iProtocol, new NetFunc_V2(o, f) ); }
	/**
		NetEventHandler가 생성한 작업이벤트를 처리할 태스크스케줄러를 설정한다.
		@param iTaskSchedulerID 태스크스케줄러 ID
	*/
	void NotifyTask( unsigned int iTaskSchedulerID );
	/** 
		작업이벤트를 처리할 태스크스케줄러를 조회한다.
	*/
	TaskScheduler* NotifyTask( void );
	/** 
		네트웍에서 수신한 패킷을 분석하여 패킷리스트에 저장한다.
		@param spLink 패킷을 전송한 원격호스트
		@param pBuf 수신된 패킷
		@param lst 패킷을 분석한 결과를 저장할 버퍼리스트
	*/
	size_t Parse( NetLinkPtr spLink, Buffer* pBuf, Buffer::QType& lst );
	/** 
		원격프로시져를 등록한다.
		@param iRpcID 원격프로시져의 고유ID
		@param pProc 원격프로시져로 호출할 함수 객체
	*/
	bool RegRemoteProcedure( unsigned int iRpcID, rpc_base* pProc );
	/** 
		Proactor를 생성하여 네트웍이벤트 감시를 시작한다.
		@param iPriority 네트웍를 종료시키는 우선순서
		@param iThreadCnt 네트웍쓰레드 개수
	*/
	bool Start( unsigned int iPriority, unsigned short iThreadCnt );
	/** 
		NetEventHandler가 관리하고 있는 모든 소켓을 종료한후 Proactor를 종료한다.
	*/
	void Stop( void );
	/** 
		TCP/IP 소캣을 생성한후 클라이언트와 연결을 구축할 준비한다.
		@param iPort 클라이언트의 접속을 받아들일 포트번호
		@param sHost 클라이언트의 접속을 받아들일 주소값
		@param iCreatedSockets 접속처리를 하기 위해 미리 생성해 놓는 소켓 개수
		@param iMaxBufSize 수신버퍼의 최대크기
		@param fnOnCreated NetLink 생성시 호출되는 콜백함수
	*/
	bool Listen( unsigned short iPort, const std::string& sHost = "localhost",
		long iCreatedSocket = 100, size_t iMaxBufSize = 4096,
		std::function<void(NetLinkPtr, bool)>* fnOnCreated = nullptr);
	/** 
		TCP/IP 소캣을 생성한후 서버와 연결한다.
		@param iPort 서버의 포트번호
		@param sHost 서버의 주소값
		@param iUserDataType NetLink에 저장할 사용자 데이타 종류
		@param iMaxBufSize 수신버퍼의 최대크기
		@param fnOnCreated NetLink 생성시 호출되는 콜백함수
	*/
	NetLinkPtr Connect( unsigned short iPort, const std::string& sHost = "localhost",
		size_t iMaxBufSize = 4096,
		std::function<void(NetLinkPtr, bool)>* fnOnCreated = nullptr);
	/** 
		UDP 소켓을 생성한다.
		@param iPort 포트번호
		@param sHost 주소값
		@param iUserDataType NetLink에 저장할 사용자 데이타 종류
		@param iMaxBufSize 수신버퍼의 최대크기
		@param fnOnCreated NetLink 생성시 호출되는 콜백함수
	*/
	NetLinkPtr Bind( unsigned short iPort, const std::string& sHost = "localhost",
		size_t iMaxBufSize = 4096,
		std::function<void(NetLinkPtr, bool)>* fnOnCreated = nullptr);
	/** 
		Proactor의 NetLink 관리 맵에 등록된 NetLink의 개수를 조회한다.
		@param iLinkType NetLink맵의 종류
		@li eLINKTYPE_UDP			UDP NetLink 맵
		@li eLinkType_TCP_LISTEN	서버의 접속 대기 NetLink 맵
		@li eLINKTYPE_TCP_REMOTE	서버에서 클라이언트에 대응되는 NetLink 맵
		@li eLINKTYPE_TCP_CONNECT	클라이언트 NetLink 맵
	*/
	size_t GetCount( unsigned short iLinkType );
	/** 
		NetEventHandler에 연결된 Proactor 객체를 조회한다.
	*/
	Proactor* GetProactor( void );
	/**
		NetCallback::OnNetAccept() 함수를 구현
	*/
	void OnNetAccepted( NetLinkPtr spLink ) override;
	/**
		NetCallback::OnNetConnected() 함수를 구현
	*/
	void OnNetConnected( NetLinkPtr spLink ) override;
	/**
		NetCallback::OnNetReceived() 함수를 구현
	*/
	void OnNetReceived( NetLinkPtr spLink, Buffer* pBuf ) override;
	/**
		NetCallback::OnNetClosed() 함수를 구현
	*/
	void OnNetClosed( NetLinkPtr spLink ) override;
};