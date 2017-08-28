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
	��Ʈ�� �̺�Ʈ �߻��� ȣ���� �ݹ��Լ��� Ÿ������
*/
typedef functional_v1<bool, NetLinkPtr>	NetFunc_V1;


/** 
	���� ���ν��� ������ Ÿ������
*/
typedef std::map<unsigned int, rpc_base*> RemoteProcedureMap;	

/** 
	������ ��Ŷ�� �м��Ͽ� �۾��̺�Ʈ�� �������� �½�ũ�����ٷ��� �����ϴ� Ŭ����.

	NetEventHandler�� Proactor�� Wrapper Ŭ������ Proactor���� ������ ��Ŷ�� �м��Ͽ� �۾��̺�Ʈ�� ����� 
	�½�ũ�����ٷ����� �����ϴ� ����� �����Ѵ�.
*/
class NetEventHandler : public NetCallback
{
protected:
	RemoteProcedureMap	m_RemoteProceduerMap;	///< �������ν��� ���� ��
	NetFunction			m_Functor;				///< ��Ʈ�� �Լ� ������
	Proactor*			m_pProactor;			///< ��Ʈ�� �̺�Ʈ ó���� ���� Proactor
	TaskScheduler*		m_pTaskScheduler;		///< �۾��̺�Ʈ�� ó���� �½�ũ�����ٷ�
	unsigned int		m_iTaskSchedulerID;		///< �׽�ũ�����ٷ�ID
	NetFunc_V1			m_OnAccepted;			///< Ŭ���̾�Ʈ�� ����Ǹ� ȣ���� �ݹ��Ѽ���
	NetFunc_V1			m_OnConnected;			///< ������ ����Ǹ� ȣ���� �ݹ��Լ���
	NetFunc_V1			m_OnClosed;				///< ������ ����Ǹ� ȣ���� �ݹ��Լ���
	NetFunc_V2			m_OnReceived;			///< ��Ŷ�� �����ϸ� ȣ���� �ݹ��Լ���

	//<
	NetEventHandler(const NetEventHandler& r) {}

public:
	NetEventHandler( unsigned int iTaskSchedulerID );
	virtual ~NetEventHandler();

	/**
		Ŭ���̾�Ʈ�� ����Ǹ� ȣ���� �ݹ��Լ� �����Ѵ�
		@param o Ŭ���� ��ü�� �ּҰ�
		@param f Ŭ���� ��ü�� �ɹ��Լ��� �ּҰ�
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
		������ ����Ǹ� ȣ���� �ݹ��Լ��� �����Ѵ�.
		@param o Ŭ���� ��ü�� �ּҰ�
		@param f Ŭ���� ��ü�� �ɹ��Լ��� �ּҰ�
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
		������ ����Ǹ� ȣ���� �ݹ��Լ��� �����Ѵ�
		@param o Ŭ���� ��ü�� �ּҰ�
		@param f Ŭ���� ��ü�� �ɹ��Լ��� �ּҰ�
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
		��Ʈ������ ��Ŷ�� �����ϸ� ȣ���� �ݹ��Լ��� �����Ѵ�. ���� �ݹ��� �Լ��� �����Ǹ� ��Ʈ������
		�߻��� ��� ��Ŷ�� �켱 ó���ϹǷ� ��Ʈ���̺�Ʈ�Լ��� �������ν��� ȣ���� ������� �ʴ´�.
		@param o Ŭ���� ��ü�� �ּҰ�
		@param f Ŭ���� ��ü�� �ɹ��Լ��� �ּҰ�
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
		��ŶID�� �ĺ��Ǵ� ��Ʈ���̺�Ʈ�Լ��� �����Ѵ�. ���� ��Ʈ�� �̺�Ʈ �Լ��� ��ŶID�� �������ν����� ID��
		�ߺ��� ��� ��Ʈ���̺�Ʈ�Լ��� ȣ�� �켱������ ���� ������ �������ν����� ȣ����� �ʴ´�.
		@param o Ŭ���� ��ü�� �ּҰ�
		@param f Ŭ���� ��ü�� �ɹ��Լ��� �ּҰ�
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
		NetEventHandler�� ������ �۾��̺�Ʈ�� ó���� �½�ũ�����ٷ��� �����Ѵ�.
		@param iTaskSchedulerID �½�ũ�����ٷ� ID
	*/
	void NotifyTask( unsigned int iTaskSchedulerID );
	/** 
		�۾��̺�Ʈ�� ó���� �½�ũ�����ٷ��� ��ȸ�Ѵ�.
	*/
	TaskScheduler* NotifyTask( void );
	/** 
		��Ʈ������ ������ ��Ŷ�� �м��Ͽ� ��Ŷ����Ʈ�� �����Ѵ�.
		@param spLink ��Ŷ�� ������ ����ȣ��Ʈ
		@param pBuf ���ŵ� ��Ŷ
		@param lst ��Ŷ�� �м��� ����� ������ ���۸���Ʈ
	*/
	size_t Parse( NetLinkPtr spLink, Buffer* pBuf, Buffer::QType& lst );
	/** 
		�������ν����� ����Ѵ�.
		@param iRpcID �������ν����� ����ID
		@param pProc �������ν����� ȣ���� �Լ� ��ü
	*/
	bool RegRemoteProcedure( unsigned int iRpcID, rpc_base* pProc );
	/** 
		Proactor�� �����Ͽ� ��Ʈ���̺�Ʈ ���ø� �����Ѵ�.
		@param iPriority ��Ʈ���� �����Ű�� �켱����
		@param iThreadCnt ��Ʈ�������� ����
	*/
	bool Start( unsigned int iPriority, unsigned short iThreadCnt );
	/** 
		NetEventHandler�� �����ϰ� �ִ� ��� ������ �������� Proactor�� �����Ѵ�.
	*/
	void Stop( void );
	/** 
		TCP/IP ��Ĺ�� �������� Ŭ���̾�Ʈ�� ������ ������ �غ��Ѵ�.
		@param iPort Ŭ���̾�Ʈ�� ������ �޾Ƶ��� ��Ʈ��ȣ
		@param sHost Ŭ���̾�Ʈ�� ������ �޾Ƶ��� �ּҰ�
		@param iCreatedSockets ����ó���� �ϱ� ���� �̸� ������ ���� ���� ����
		@param iMaxBufSize ���Ź����� �ִ�ũ��
		@param fnOnCreated NetLink ������ ȣ��Ǵ� �ݹ��Լ�
	*/
	bool Listen( unsigned short iPort, const std::string& sHost = "localhost",
		long iCreatedSocket = 100, size_t iMaxBufSize = 4096,
		std::function<void(NetLinkPtr, bool)>* fnOnCreated = nullptr);
	/** 
		TCP/IP ��Ĺ�� �������� ������ �����Ѵ�.
		@param iPort ������ ��Ʈ��ȣ
		@param sHost ������ �ּҰ�
		@param iUserDataType NetLink�� ������ ����� ����Ÿ ����
		@param iMaxBufSize ���Ź����� �ִ�ũ��
		@param fnOnCreated NetLink ������ ȣ��Ǵ� �ݹ��Լ�
	*/
	NetLinkPtr Connect( unsigned short iPort, const std::string& sHost = "localhost",
		size_t iMaxBufSize = 4096,
		std::function<void(NetLinkPtr, bool)>* fnOnCreated = nullptr);
	/** 
		UDP ������ �����Ѵ�.
		@param iPort ��Ʈ��ȣ
		@param sHost �ּҰ�
		@param iUserDataType NetLink�� ������ ����� ����Ÿ ����
		@param iMaxBufSize ���Ź����� �ִ�ũ��
		@param fnOnCreated NetLink ������ ȣ��Ǵ� �ݹ��Լ�
	*/
	NetLinkPtr Bind( unsigned short iPort, const std::string& sHost = "localhost",
		size_t iMaxBufSize = 4096,
		std::function<void(NetLinkPtr, bool)>* fnOnCreated = nullptr);
	/** 
		Proactor�� NetLink ���� �ʿ� ��ϵ� NetLink�� ������ ��ȸ�Ѵ�.
		@param iLinkType NetLink���� ����
		@li eLINKTYPE_UDP			UDP NetLink ��
		@li eLinkType_TCP_LISTEN	������ ���� ��� NetLink ��
		@li eLINKTYPE_TCP_REMOTE	�������� Ŭ���̾�Ʈ�� �����Ǵ� NetLink ��
		@li eLINKTYPE_TCP_CONNECT	Ŭ���̾�Ʈ NetLink ��
	*/
	size_t GetCount( unsigned short iLinkType );
	/** 
		NetEventHandler�� ����� Proactor ��ü�� ��ȸ�Ѵ�.
	*/
	Proactor* GetProactor( void );
	/**
		NetCallback::OnNetAccept() �Լ��� ����
	*/
	void OnNetAccepted( NetLinkPtr spLink ) override;
	/**
		NetCallback::OnNetConnected() �Լ��� ����
	*/
	void OnNetConnected( NetLinkPtr spLink ) override;
	/**
		NetCallback::OnNetReceived() �Լ��� ����
	*/
	void OnNetReceived( NetLinkPtr spLink, Buffer* pBuf ) override;
	/**
		NetCallback::OnNetClosed() �Լ��� ����
	*/
	void OnNetClosed( NetLinkPtr spLink ) override;
};