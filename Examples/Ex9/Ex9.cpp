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
#include <MemoryEx.h>
#include <DBEngine.h>
#include <FSM.h>

#define trace			printf(__FUNCTION__"\n" )

enum 
{
	eAQUIRE_MONSTER = 1,
	eDICE_CAST,
	eMOVE_PLAYER,
	eMOVE_PLAYER_SYNC,
	eCHECK_LAND,
	eMYLAND_COMMAND,
	eBATTLE_COMMAND,
	ePLACE_MONSTER_COMMAND,
};


AI_SPECFIC(AI_Aquire_Monster);
class AI_Aquire_Monster : public FSM::State
{
public:
	STATE_CODE(eAQUIRE_MONSTER);
	State* OnEvent( FSM::Machine* m )
	{
		trace;
		return this;
	}
};

AI_SPECFIC(AI_Dice_Cast);
class AI_Dice_Cast : public FSM::State
{
public:
	STATE_CODE(eDICE_CAST);
	State* OnEvent( FSM::Machine* m )
	{
		trace;
		return this;
	}
};

AI_SPECFIC(AI_Move_Player);
class AI_Move_Player : public FSM::State
{
public:
	STATE_CODE(eMOVE_PLAYER);
	State* OnEvent( FSM::Machine* m )
	{
		trace;
		return this;
	}
};

AI_SPECFIC(AI_Move_Player_Sync);
class AI_Move_Player_Sync : public FSM::State
{
public:
	STATE_CODE(eMOVE_PLAYER_SYNC);
	State* OnEvent( FSM::Machine* m )
	{
		trace;
		return this;
	}
};

AI_SPECFIC(AI_Check_Land);
AI_SPECFIC(AI_MyLand_Command);
AI_SPECFIC(AI_Battle_Command);
AI_SPECFIC(AI_Place_Monster_Command);
class AI_Check_Land : public FSM::State
{
public:
	STATE_CODE(eCHECK_LAND);
	State* OnEvent( FSM::Machine* m )
	{
		trace;
		Random rnd;
		rnd.Px( 1.0f/3.0f );
		rnd.Px( 1.0f/3.0f );
		rnd.Px( 1.0f/3.0f );

		switch ( rnd.Rand() )
		{
			case 0 : return AI_PTR(AI_MyLand_Command);
			case 1 : return AI_PTR(AI_Battle_Command);
			case 2 : return AI_PTR(AI_Place_Monster_Command);
			default : return AI_PTR(AI_MyLand_Command);
		}

		return this;
	}
};

class AI_MyLand_Command : public FSM::State
{
public:
	STATE_CODE(eMYLAND_COMMAND);
	State* OnEvent( FSM::Machine* m )
	{
		trace;
		return this;
	}
};

class AI_Battle_Command : public FSM::State
{
public:
	STATE_CODE(eBATTLE_COMMAND);
	State* OnEvent( FSM::Machine* m )
	{
		trace;
		return this;
	}
};

class AI_Place_Monster_Command : public FSM::State
{
public:
	STATE_CODE(ePLACE_MONSTER_COMMAND);
	State* OnEvent( FSM::Machine* m )
	{
		trace;
		return this;
	}
};


class player : public FSM::Machine
{
public:
	TimerHandle key;
public:
	player()
	{
		printf( __FUNCTION__"\n" );
	}
	~player()
	{
		printf( __FUNCTION__"\n" );
	}
	bool OnTimer( void* pUserData )
	{
		return true;
	}
	bool OnTimer2( void* pUserData )
	{
		CANCEL_TIMER( 1, key );
		key = nullptr;
		return false;
	}
};

AI_DELCARE(AI_Aquire_Monster);
AI_DELCARE(AI_Dice_Cast);
AI_DELCARE(AI_Move_Player);
AI_DELCARE(AI_Move_Player_Sync);
AI_DELCARE(AI_Check_Land);
AI_DELCARE(AI_MyLand_Command);
AI_DELCARE(AI_Battle_Command);
AI_DELCARE(AI_Place_Monster_Command);


int _tmain(int argc, _TCHAR* argv[])
{
	player py;
// 	CREATE_TASKSCEDULER(1, 100, 0);
// 	START_TASKSCHEDULER(0);
// 
// 	py.key = CREATE_TIMER(1, &py, &player::OnTimer, 2000, nullptr );
// 	CREATE_TIMER(1, &py, &player::OnTimer2, 4000, nullptr );
// 	getchar();
// 	STOP_TASKSCHEDULER(0);
//	return 0;


	AI_DEF( ai_do(AI_Aquire_Monster),			ai_next(AI_Dice_Cast) );
	AI_DEF( ai_do(AI_Dice_Cast),				ai_next(AI_Move_Player) );
	AI_DEF( ai_do(AI_Move_Player),				ai_end(AI_Move_Player_Sync) );

	AI_DEF( ai_do(AI_Move_Player_Sync),			ai_next(AI_Check_Land) );
	AI_DEF( ai_if(AI_Check_Land),				ai_next(AI_MyLand_Command) );
	AI_DEF( ai_if(AI_Check_Land),				ai_next(AI_Battle_Command) );
	AI_DEF( ai_if(AI_Check_Land),				ai_next(AI_Place_Monster_Command) );
	AI_DEF( ai_do(AI_MyLand_Command),			ai_end(AI_Aquire_Monster) );
	AI_DEF( ai_do(AI_Battle_Command),			ai_end(AI_Aquire_Monster)  );
	AI_DEF( ai_do(AI_Place_Monster_Command),	ai_end(AI_Aquire_Monster) );

	while ( 1 )
	{
		printf( "%s\n", std::string(50, '-').c_str() );
		py.Transition(AI_PTR(AI_Aquire_Monster) );
		AI_CALL( &py );

		py.Transition(AI_PTR(AI_Move_Player_Sync) );
		AI_CALL( &py );
		Sleep(1000);
	}


	return 0;
// 	util::folders fld;
// 	fld.home("c:\\Workspace\\Summoner\\root_branch\\Examples");
// 	if ( util::scandir( fld, 0 ) == util::eSuccess )
// 	{
// 		fld.dump( fld );
// 	}
/*
// 	FSM_REG_V1( 0, s_empty );
// 	FSM_REG_V1( 1, s_test );
// 
// 	player p;
// 	FSM::State* s = p.GetState();
// 	printf( "current state = %s\n", s->GetName() );
// 	FSM_CALL( &p, 0, 1 );
// 	s = p.GetState();
// 	printf( "current state = %s\n", s->GetName() );
// 
// 	FSM_CALL( &p, 1, 0 );
// 	s = p.GetState();
// 	printf( "current state = %s\n", s->GetName() );
// 
// 	getchar();
// 
// 	return 0;

	player p;
	FSM::State* s = p.GetState();
	printf( "current state = %s\n", s->GetName() );
	FSM_CALL( &p, 0, 1 );
	s = p.GetState();
	printf( "current state = %s\n", s->GetName() );

	FSM_CALL( &p, 1, 0 );
	s = p.GetState();
	printf( "current state = %s\n", s->GetName() );

	getchar();

	return 0;
*/
/** for bind test
	std::string s = "select * from test.server_config where iseq = \\? and sname = '\\?'";

	size_t iOffset = 0;
	while ( 1 )
	{
		iOffset = bind_variable( s, "1", iOffset );
		if ( iOffset == std::string::npos )
		{
			break;
		}
	}


	printf( "offset = %d, %s\n", iOffset, cstr(s) );
	getchar();
	return 0;
*/

/* for db remote
	CREATE_TASKSCEDULER( 1, 100, 0 );
	if ( GetLastError() != NOERROR )
	{
		prn_err( "%s", errmsg );
		return 1;
	}
	START_TASKSCHEDULER( 1 );

	INILoader::instance()->Load( ".\\Test.ini" );

	IDBProvider::Properties config;
	config.iNotifyTaskSchedulerID = 1;
	config.stDatabase.iConnectionCount = 1;
	config.stRemote.iThreadCount = 1;

	// 데이타 베이스 설정
	util::load_dbcfg("DB_Account", config);
	// 서버 접속 설정
	util::load_remotedbcfg("DB_Account", config);

	// DB 설정 출력
	prn_sbj( "database" );
	util::show_dbcfg(config);
	util::show_remotedbcfg(config);
	prn_bar();

	IDBProvider* pProvider = IDBProviderFactory::CreateDirect();
	if( !pProvider )
	{
		return false;
	}

	int32 rst = pProvider->Startup( config );
	if( !rst )
	{
		prn_err( "%s", errmsg );
		return false;
	}

	TestQry qry;

	IDBExecuter::Run( pProvider, 0, IDBCommand::ctSQL, "select * from test.server_config where sWorld = ?", db_string("DBGW"), 
		&qry, db_exec_qry_callback(TestQry,&TestQry::OnTestQuery) );

	IDBExecuter::Run( pProvider, 0, IDBCommand::ctSP, "SP_SelectPlayer( ?, ? )",
		23,
		db_string("NickName"),
		&qry, db_exec_rst_callback(TestQry, &TestQry::OnTestResult) );

//	pProvider->Push( new TestQry );


	getchar();

	pProvider->Shutdown();
	delete pProvider;
	return 0;

*/
/* for db direct
	IDBEngine* db = IDBEngineFactory::CreateMySQL();

	IDBProvider::Properties dbcfg;
	dbcfg.m_Database.sHost = "127.0.0.1";
	dbcfg.m_Database.sPort = "3306";
	dbcfg.bRemote = false;
	dbcfg.m_Database.sProviderName = "testdb";
	dbcfg.m_Database.sCategory = "test";
	dbcfg.m_Database.sUid = "root";
	dbcfg.m_Database.sPassword = "j2soft123!";
	dbcfg.m_Database.bAutoReconnect = false;
	int32 rst = db->Connect( dbcfg );

	TestQry* pQry = new TestQry;
	db->Execute( pQry );

	safe_delete( pQry );
	safe_delete( db );
	return 0;
*/
	return 0;
}

