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
#include <ThreadEx.h>
#include <Ptr.h>
#include <INILoader.h>
#include <PdhMonitor.h>

/** 
	맴버함수를 쓰레드에서 실행하는 예제
*/

char*	g_global = new char[1024 * 1024];

PTR(TestObj);
class TestObj : public shared_ref
{
	char*	dummy1;
	char*   dummy2;
public:
	TestObj() { dummy1 = new char[1024 * 1024]; dummy2 = new char[1024 * 1024]; }
	virtual ~TestObj()
	{
		delete[] dummy1;
		delete[] dummy2;
		printf( __FUNCTION__"\n" );
	}

	void MyThread1( void )
	{
		for (;;)
		{
			memcpy( dummy1, g_global, 1024*1024);
		}

	}

	void MyThread2( int i )
	{
		for (;;)
		{
			memcpy( dummy2, g_global, 1024*1024);
		}
	}

	void MyThread3( int i )
	{
		IPdh::ModuleCounter module;
		IPdh::ProcessorCounterVec processor;
		IPdh::ThreadCounterVec threads;
		Pdh::GetPdhMonitor()->Open("",0);

		for (;;)
		{

			printf( "------------------------------\n" );
			Pdh::GetPdhMonitor()->CollectData( &processor );
			Pdh::GetPdhMonitor()->CollectData( &threads );
			Pdh::GetPdhMonitor()->CollectData( &module );
			Pdh::PrintProcessorCounter( processor );
			Pdh::PrintThreadCounter( threads );
			Pdh::PrintModuleCounter( module );

			Sleep(1000 * 2);
		}
	}
};


struct table_Mission
{
	DWORD	MissionID;
	DWORD	MissionScopeType;
	DWORD	StageID;
	DWORD	QuestID;
	DWORD	MissionType;
	DWORD	Value1;
	DWORD	Value2;
	DWORD	Reward;
	DWORD	RewardValue;
	time_t	TimeStart;
	time_t	TimeEnd;
	BYTE	OpenContents;	// 4: 시련의탑, 5: 몽환의 회랑, 6: 각성보스, 7: 고대몬스터, 8:데일리미션
};

class CSVTableTest : public CSVTable
{
public:
	CSVTableTest() {}

	CSV_TABLE_DEFINE(table_Mission);

public:

public:
	bool OnLoad( void ) override
	{
		CSV_LOAD_TABLE_DATA( CSVTableTest, table_Mission, "Mission.csv" );

		return true;
	}

};

CSV_LOAD_TABLE_FIELD(CSVTableTest, table_Mission, "MissionID")

	GET_FIELD_VALUE_UINT( "MissionID",			data->MissionID );
	GET_FIELD_VALUE_UINT( "MissionScopeType",	data->MissionScopeType );
	GET_FIELD_VALUE_UINT( "StageMissionGroupID",data->StageID );
	GET_FIELD_VALUE_UINT( "QuestID",			data->QuestID );
	GET_FIELD_VALUE_UINT( "MissionType",		data->MissionType );
	GET_FIELD_VALUE_UINT( "TypeValue_1",		data->Value1 );
	GET_FIELD_VALUE_UINT( "TypeValue_2",		data->Value2 );
	GET_FIELD_VALUE_UINT( "Reward",				data->Reward );
	GET_FIELD_VALUE_UINT( "RewardValue",		data->RewardValue );
	string sTime = "";
	GET_FIELD_VALUE_STR ( "TimeStart",			sTime );
	data->TimeStart = util::to_date(sTime, "YYYY-MM-DD HH:MI:SS");
	sTime = "";
	GET_FIELD_VALUE_STR ( "TimeEnd",			sTime );
	data->TimeEnd = util::to_date(sTime, "YYYY-MM-DD HH:MI:SS");
	GET_FIELD_VALUE_BYTE( "OpenContents",		data->OpenContents );

CSV_LOAD_TABLE_FIELD_END(table_Mission)

int _tmain(int argc, _TCHAR* argv[])
{
	CSVTableTest inst;
	inst.SetPath("D:\\Workspace\\dragons_client\\Master Table\\output");
	inst.Load();
	std::map<DWORD, table_Mission*> tbl = inst.Get_table_Mission();
	const table_Mission* pData = inst.Get_table_Mission(20);

/* Thread 테스트
	INILoader::instance()->Load( "D:\\Workspace\\RPC.NET.ver.0.1\\Bin\\user-PC.ini" );

	printf( "%s\n", cstr(INILoader::instance()->GetString( "DB_Account", "provider_name", "aaa" )));

	INILoader::instance()->SetString( "KKK", "test", "test data" );

	printf( "%s\n", cstr(INILoader::instance()->GetString( "KKK", "test", "aaa" )));

	INILoader::instance()->Save( "D:\\Workspace\\RPC.NET.ver.0.1\\Bin\\test.ini" );

//	INILoader::instance()->Dump();
	return 0;

	printf( "맴버함수를 쓰레드에서 실행하는 예제\n" );
	TestObjPtr inst = new TestObj;

	// 쓰레드 생성
	Thread* pThr1 = CREATE_THREAD_V0( inst.get(), &TestObj::MyThread1, 1 );
	pThr1->Resume();

	// 쓰레드 생성
	Thread* pThr2 = CREATE_THREAD_V1( inst.get(), &TestObj::MyThread2, 2, 2 );
	pThr2->Resume();

	// 쓰레드 생성
	Thread* pThr3 = CREATE_THREAD_V1( inst.get(), &TestObj::MyThread3, 3, 3 );
	pThr3->Resume();


	getchar();
	pThr1->Terminate();
	pThr1->Wait();
	delete pThr1;

	pThr2->Terminate();
	pThr2->Wait();
	delete pThr2;

	pThr3->Terminate();
	pThr3->Wait();
	delete pThr3;
*/
	return 0;
}

