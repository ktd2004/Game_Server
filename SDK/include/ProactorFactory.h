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
	IOCP����� Proactor �������� ������ ��Ʈ���̺�Ʈ�� ó���ϴ� Ŭ����
	
	TCP/IP, UDP ���� ����, ����Ÿ �ۼ����� ���� �������̽��� �����ϸ�, ���Ͽ��� �߻��� �̺�Ʈ��
	NetEventHandler�� �����ϴ� ����� �����Ѵ�.
*/
struct Proactor
{
	struct Property
	{
		std::string		sHost;			///< ȣ��Ʈ�ּ�
		unsigned short	iPort;			///< ��Ʈ
		long			iCreatedSockets;///< ���� ��� NetLink�� ����
		NetCallback*	pNetCallback;	///< ��Ʈ�� �̺�Ʈ ó�� ��ü
		size_t			iMaxBufSize;	///< ���� ���� ũ��
		std::function<void(NetLinkPtr, bool)>* fnOnCreated;	///< NetLink ������ ȣ��Ǵ� �ݹ�
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
		Proactor ���� ������ �Ҵ�� ����ID�� ��ȸ�Ѵ�. 
	*/
	virtual unsigned int	ID( void ) = 0;
	/**
		TCP/IP ������ ������ �غ� �Ѵ�.
		@param rProp Ŭ���̾�Ʈ�� ������ ��Ʈ���ּ�, ���Ӵ�� NetLink�� ����, ��Ʈ�� �̺�Ʈ�� ó���� ��ü�� �����Ѵ�.

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
		���� ȣ��Ʈ�� �����Ѵ�.
		@param rProp Ŭ���̾�Ʈ�� ������ ��Ʈ���ּ�

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
		UDP ������ �����Ͽ� ���ε��Ѵ�.
		@param rProp Ŭ���̾�Ʈ�� ������ ��Ʈ���ּ�

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
		���ݿ��� ������ Ŭ���̾�Ʈ�� ��� �����Ѵ�.
	*/
	virtual void			KickAll( unsigned short iKickReason ) = 0;
	/** 
		Proactor�� �����Ѵ�.
	*/
	virtual void			Finalize( void ) = 0;
	/** 
		Proactor�� IOCP �ڵ��� ���´�.
	*/
	virtual HANDLE			GetHandle( void ) = 0;
	/** 
		Proactor�� NetLink ���� ���� ���´�.
		@param iLinkType NetLink���� ����
		@li eLINKTYPE_UDP			UDP NetLink ��
		@li eLinkType_TCP_LISTEN	������ ���� ��� NetLink ��
		@li eLINKTYPE_TCP_REMOTE	�������� Ŭ���̾�Ʈ�� �����Ǵ� NetLink ��
		@li eLINKTYPE_TCP_CONNECT	Ŭ���̾�Ʈ NetLink ��
	*/
	virtual NetLinkMap&		GetNetLinkMap( unsigned short iLinkType ) = 0;
	/** 
		Proactor�� NetLink �����ʿ� NetLink�� ����Ѵ�.
		@param spLink NetLink �����ʿ� ����� NetLink
	*/
	virtual bool			RegLink( NetLinkPtr spLink ) = 0;
	/** 
		Proactor�� NetLink �����ʿ��� NetLink�� �����Ѵ�.
		@param spLink NetLink �����ʿ��� ������ NetLink
	*/
	virtual bool			UnregLink( NetLinkPtr spLink ) = 0;
	/** 
		Proactor�� NetLink ���� �ʿ� ��ϵ� NetLink�� ������ ��ȸ�Ѵ�.
		@param iLinkType NetLink���� ����
		@li eLINKTYPE_UDP			UDP NetLink ��
		@li eLinkType_TCP_LISTEN	������ ���� ��� NetLink ��
		@li eLINKTYPE_TCP_REMOTE	�������� Ŭ���̾�Ʈ�� �����Ǵ� NetLink ��
		@li eLINKTYPE_TCP_CONNECT	Ŭ���̾�Ʈ NetLink ��
	*/
	virtual size_t			GetCount( unsigned short iLinkType ) = 0;
};

/** 
	Proactor�� ����, ����, ���� �� �������� ������ �����ϴ� �����Լ��� �����ϴ� ���ӽ����̽�.
*/
namespace ProactorFactory
{
	/** 
		Proactor�� �����ϸ� Proactor �����ʿ� ���� �켱������ ���� ��ϵǾ� ���� ������ ����޴´�.
		@param iTerminatePriority ���� �켱����
		@param iBindCPU ��Ʈ�������带 ������ CPU
		@param iThreadCnt ��Ʈ�������� ����
	*/
	Proactor*		Create( int iTerminatePriority, unsigned short iThreadCnt, unsigned short iBindCPU = 0 );
	/** 
		Proactor ��ü�� ��ȸ�Ѵ�.
		@param iProactorID Proactor�� ����ID
	*/
	Proactor*		Get( unsigned int iProactorID );
	/** 
		Proactor�� �����Ѵ�.
		@param iProactorID Proactor�� ����ID, ���� 0 �̸� Proactor �����ʿ� ��ϵ� ��� Proactor�� �����Ѵ�.
	*/
	bool			Start( unsigned int iProactorID  = 0 );
	/** 
		Proactor�� ����켱���� �� ���� ���������� �����Ѵ�.
		@param iProactorID Proactor�� ����ID, ���� 0 �̸� Proactor �����ʿ� ��ϵ� ��� Proactor�� �����Ѵ�.
	*/
	bool			Stop( unsigned int iProactorID  = 0 );
};