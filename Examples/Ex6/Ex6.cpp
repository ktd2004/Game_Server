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

#include "stdafx.h"

#include <Proactor.h>
#include <Errorcodes.h>
#include <PdhMonitor.h>
#include <ServerFrame.h>
#include <Management.h>


struct UserData 
{
	int				m_iVal;
	std::string		m_sText;
	float			m_fVal;
};

Buffer& operator<<( Buffer& pk, const UserData& r )
{
	pk << r.m_iVal << r.m_sText << r.m_fVal;
	return pk;
}

Buffer& operator>>( Buffer& pk, UserData& r )
{
	pk >> r.m_iVal >> r.m_sText >> r.m_fVal;
	return pk;
}

class TestApp
	: public ServerApp
	, public NetEventHandler
	, public PAgentReceiver
{
	TaskScheduler*		m_pTaskScheduler;
	NetLinkPtr			m_spServerUDPLink;
	NetLinkPtr			m_spServerLink;

public:
	TestApp(int argc, char* argv[])
		: ServerApp(argc, argv)
		, NetEventHandler(1)
	{
		SetAcceptedFunc(this, &TestApp::OnAccepted);
		SetConnectedFunc(this, &TestApp::OnConnected);
		SetClosedFunc(this, &TestApp::OnClosed);

		SetNetFunc( 10008, this, &TestApp::Test_NetFunc );

		EXPORT_RPC_V2( 10000, this, &TestApp::AddItem, unsigned int, unsigned int );
		EXPORT_RPC_V1( 10001, this, &TestApp::SetNickName, const std::string& );
		EXPORT_RPC_V1( 10002, this, &TestApp::PrintUserData, const UserData& );
		EXPORT_RPC_V1( 10003, this, &TestApp::UDP_Func, const UserData& );
		EXPORT_RPC_V1( 10004, this, &TestApp::BufferTest, Buffer&);
	}

	~TestApp()
	{
	}

	IMPORT_RPC_V2( 10000, AddItem, unsigned int, unsigned int, 1024 );
	IMPORT_RPC_V1( 10001, SetNickName, const std::string&, 1024 );
	IMPORT_RPC_V1( 10002, PrintUserData, const UserData&, 1024 );
	IMPORT_RPC_V1( 10003, UDP_Func, const UserData&, 1024);
	IMPORT_RPC_V1( 10004, BufferTest, Buffer&, 1024);

	bool BufferTest(NetLinkPtr spLink, Buffer& r)
	{
		int iVal;
		std::string s;
		r >> iVal >> s;
		printf("BufferTest) val = %d, text = %s\n", iVal, cstr(s));

		return true;
	}

	void DoCommand( const std::string& sCmd ) override
	{
		m_spServerLink->Kick( eKICK_ADMIN, __FUNCTION__ );
	}

	bool OnSetProperty( ServerApp::Properties& appCfg ) override
	{
		
		//////////////////////////////////////////////////////////////////////////
		// config load
		appCfg.iSID						= 12001;
		appCfg.iTaskDuration			= 10;
		appCfg.bLogArchive				= false;
		appCfg.sLogArchivePath			= "\\.";
		appCfg.iTaskSchedulersCount		= 1;
		appCfg.iNotifyTaskSchedulerID	= 1;
		appCfg.sLogBindAddr				= "localhost";
		appCfg.iLogSPort				= 4001;
		appCfg.sLogSAddr				= "localhost";
		appCfg.pPAgentHandler			= nullptr;

		return true;
	}

	bool OnInitialized(int32 argc, char* argv[]) override
	{
		ServerApp::Properties& appCfg = GetProperty();

		//////////////////////////////////////////////////////////////////////////
		if ( argc > 1 && strcmp(argv[1], "server") == 0 )
		{
			if ( !Listen( 2333, "localhost", 2) )
			{
				return false;
			}
		}
		else if ( argc > 1 && strcmp(argv[1], "client") == 0 )
		{
			m_spServerLink = Connect( 2333, "localhost" );
		}

//		CREATE_TIMER(1, this, &TestApp::OnTimer, 1000, nullptr );

		return true;
	}

	void OnQuitApplication() override
	{
		Stop();
		QuitApplication(eNORMAL_EXIT);
	}

	bool OnAccepted( NetLinkPtr spLink )
	{
		prn_inf( ">> accept remote addr %s", spLink->GetAddr().c_str() );
		m_spServerLink = spLink;;
		return true;
	}

	bool OnConnected( NetLinkPtr spLink )
	{
		if ( m_spServerLink == spLink )
		{
			Buffer* pBuf = new Buffer( 1024 );
			Packet pk(pBuf);
			pk.id(10008);
			pk << "this is test net function";
			m_spServerLink->Send( pk.get() );

			printf( "remote call ...AddItem\n" );
			rpc_AddItem( m_spServerLink, 23344, 12302 );
			printf( "remote call ...SetNickName\n" );
			rpc_SetNickName( m_spServerLink, "google.co.kr" );

			printf( "remote call ...PrintUserData\n" );
			UserData r;
			r.m_iVal = 333;
			r.m_sText = "this is user data";
			r.m_fVal = 0.444f;
			rpc_PrintUserData( m_spServerLink, r );

			Buffer buf;
			buf << r.m_iVal;
			buf << r.m_sText;
			rpc_BufferTest(m_spServerLink, buf);

			if ( m_spServerUDPLink )
			{
				Packet pk( new Buffer(2048) );
				pk.id( 10003 );
				pk << r;

				SOCKADDR_IN addr;
				NetLink::toaddr(addr, "127.0.0.1", 2201);
				m_spServerUDPLink->Send( pk.get(), &addr );

			}

			CREATE_TIMER(1, this, &TestApp::OnTimer, 1000, nullptr );
		}

		return true;
	}

	bool OnBlockClient( bool bBlock ) override
	{
		prn_dbg( ">> block %s", bool2str(bBlock) );
		return true;
	}
	bool OnMaxClient( uint32 iMaxClient ) override
	{
		prn_dbg( ">> max client %d", iMaxClient );
		return true;
	}

	bool OnClosed( NetLinkPtr spLink )
	{
		m_spServerLink = nullptr;
		return true;
	}

	bool AddItem( NetLinkPtr spLink, unsigned int iPlayerID, unsigned int iItemID )
	{
		prn_dbg( ">> iPlayerID(%u), iItem(%u)", iPlayerID, iItemID );
		return true;
	}

	bool SetNickName( NetLinkPtr spLink, const std::string& sNickName )
	{
		prn_dbg( ">> NickName(%s)", sNickName.c_str() );
		return true;
	}

	bool PrintUserData( NetLinkPtr spLink, const UserData& r )
	{
		prn_dbg( ">> UserData(m_iVal = %d, m_sText = %s, m_fVal = %f)", r.m_iVal, r.m_sText.c_str(), r.m_fVal );
		return true;
	}

	bool UDP_Func( NetLinkPtr spLink, const UserData& r )
	{
		SOCKADDR_IN* from = (SOCKADDR_IN*)spLink.param();
		if ( from )
		{
			prn_dbg( ">> recv ip (%s)", NetLink::tostr(from).c_str() );
		}
		prn_dbg( ">> UserData(m_iVal = %d, m_sText = %s, m_fVal = %f)", r.m_iVal, r.m_sText.c_str(), r.m_fVal );
		return true;
	}

	bool Test_NetFunc( NetLinkPtr spLink, Buffer* pBuf )
	{
		std::string sData;
		*pBuf >> sData;

		prn_dbg( ">> %s", sData.c_str() );
		return true;
	}

	bool OnTimer( void* pUsrData )
	{
		prn_snd( "this is test" );
//		rpc_SetNickName( m_spServerLink, "naver.com" );
		
		Proactor_Counter t = performance::proactor_counter();
		std::string bar( 30, '-' );
		prn_inf( "%s", cstr(bar) );
		prn_inf( "buffers         %d", t.iBuffers );
		prn_inf( "tcp links       %d", t.iTCPNetLinks );
		prn_inf( "tcp sent ctx    %d", t.iTCPSentCtxs );
		prn_inf( "udp links       %d", t.iUDPNetLinks );
		prn_inf( "udp sent ctx    %d", t.iUDPSentCtxs );
		prn_inf( "recv event      %d", t.iNetReceiveEvents );
		prn_inf( "recv netfunc    %d", t.iNetFunctorEvents );
		prn_inf( "recv remotecall %d", t.iNetRemoteCallEvents );

// 		TaskScheduler_Counter t = performance::taskscheduler_counter();
// 		printf( "%s : %lu\n", util::lpad("최대 작업완료 소요시간", 30).c_str(), t.iMaxTaskElapsedTime );
// 		printf( "%s : %lu\n", util::lpad("최소 작업완료 소요시간", 30).c_str(), t.iMinTaskElapsedTime );
// 		printf( "%s : %lu\n", util::lpad("최대 타이머완료 소요시간", 30).c_str(), t.iMaxTimerElapsedTime );
// 		printf( "%s : %lu\n", util::lpad("최소 타이머완료 소요시간", 30).c_str(), t.iMinTimerElapsedTime );
// 		printf( "%s : %lu\n", util::lpad("최대 이벤트완료 소요시간", 30).c_str(), t.iMaxEventElapsedTime );
// 		printf( "%s : %lu\n", util::lpad("최소 이벤트완료 소요시간", 30).c_str(), t.iMinEventElapsedTime );

		return true;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	TestApp		ts(argc, argv);
	if ( !ts.Initialize() )
	{
		return ts.ExitCode();
	}

	ts.RunEventLoop();
	return ts.ExitCode(); 
}

