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

#include <string>
#include <TaskScheduler.h>


/** 
	태스크 스케줄러 2개를 생성한 후 태스크 스케줄러 간에 비동기 프로시져를 호출하는 예제
*/

const unsigned int TS_PRIMARY = 1;
const unsigned int TS_SECONDARY = 2;
TaskScheduler* sc1 = nullptr;
TaskScheduler* sc2 = nullptr;

// 1초에 한번씩 비동기 프로시져 ItemManager::AddItem, RemoveItem 을 호출한다.
class PlayerManager : public shared_ref
{
	int		i;
public:
	PlayerManager()
	{
		i = 0;
		incref();
		// ItemManager::AddItem에 대한 결과를 받아오기 위한 비동기 프로시져 선언
		EXPORT_PROC_V2( TS_PRIMARY, 1001, this, &PlayerManager::ResultOperation, int, const std::string& );
	}
	virtual ~PlayerManager() {}

	// ItemManager의 AddItem, RemoveItem 을 비동기 프로시져로 선언한다.
	// 비동기 프로시져를 호출할때는 proc_AddItem, proc_RemoveItem 으로 호출해야 한다.
	IMPORT_PROC_V2( 1000, AddItem, int, int, 1024 );
	IMPORT_PROC_V2( 2000, RemoveItem, int, int, 1024 );

	void EventTick()
	{
		// 1초에 한번씩 ItemManager::AddItem, RemoveItem 을 호출한다.
		i++;
		if ( i % 2 == 1 )
		{
			printf( "비동기 프로시져 ItemManager::AddItem 호출\n" );
			proc_AddItem( nullptr, 1, i );
		}
		else
		{
			printf( "비동기 프로시져 ItemManager::RemoveItem 호출\n" );
			proc_RemoveItem( nullptr, 1, i );
		}
	}

	// proc_AddItem, proc_RemoveItem 수행 결과를 받아온다.
	bool ResultOperation( interface_ref* pRef, int iPlayerID, const std::string& sResult )
	{
		printf( "    -->결과(%s): iPlayerID(%d), %s\n", __FUNCTION__, iPlayerID, sResult.c_str() );
		return true;
	}
};

// PlayerManager 에서 AddItem, RemoveItem 을 호출 할 수 있도록 두 함수를 비동기 프로시져로 선언한다.
class ItemManager : public shared_ref
{
public:
	ItemManager()
	{
		incref();
		// AddItem, RemoveItem 을 비동기 프로시져로 선언한다.
		EXPORT_PROC_V2( TS_SECONDARY, 1000, this, &ItemManager::AddItem, int, int );
		EXPORT_PROC_V2( TS_SECONDARY, 2000, this, &ItemManager::RemoveItem, int, int );
	}
	virtual ~ItemManager() {}

	void EventTick()
	{
	}

	// AddItem, RemoveItem 을 수행한 결과를 알려주는 비동기 프로시져를 등록한다.
	IMPORT_PROC_V2( 1001, ResultOperation, int, const std::string&, 1024 );

	bool AddItem( interface_ref* pCaller, int iPlayerID, int iItemID )
	{
		printf( "    -->비동기 프로시져 실행(%s): iPlayerID(%d), iItemID(%d)\n", __FUNCTION__, iPlayerID, iItemID );
		std::string sResult = "additem success : " + util::intstr(iItemID);
		// PlayerManager 에 AddItem 수행 결과를 알려준다.
		proc_ResultOperation( nullptr, iPlayerID, sResult );
		return true;
	}

	bool RemoveItem( interface_ref* pRef, int iPlayerID, int iItemID)
	{
		printf( "    -->비동기 프로시져 실행(%s): iPlayerID(%d), iItemID(%d)\n", __FUNCTION__, iPlayerID, iItemID );
		std::string sResult = "removeitem success : " + util::intstr(iItemID);
		// PlayerManager 에 RemoveItem 수행 결과를 알려준다.
		proc_ResultOperation( nullptr, iPlayerID, sResult );
		return true;
	}
};

#include <Errorcodes.h>

int _tmain(int argc, _TCHAR* argv[])
{
	printf( "태스크 스케줄러 2개를 생성한 후 태스크 스케줄러 간에\n비동기 프로시져를 호출하는 예제\n" );

	// 태스크 스케줄러 1를 생성한다.
	sc1 = CREATE_TASKSCEDULER( TS_PRIMARY, 50, 1 );
	if ( sc2 == nullptr && GetLastError() != NOERROR )
	{
		printf( "error: %s\n", GetLastErrorMessage(GetLastError()) );
		return 1;
	}
	// 태스크 스케줄러 2를 생성한다.
	sc2 = CREATE_TASKSCEDULER( TS_SECONDARY, 50, 2 );
	if ( sc2 == nullptr && GetLastError() != NOERROR )
	{
		printf( "error: %s\n", GetLastErrorMessage(GetLastError()) );
		return 1;
	}
	
	PlayerManager inst1;
	ItemManager inst2;
	
	// 1초에 한번씩 EventTick 을 호출한다.
 	CREATE_TASK( TS_PRIMARY, &inst1, &PlayerManager::EventTick, 1000 ); 
 	CREATE_TASK( TS_SECONDARY, &inst2, &ItemManager::EventTick, 1000 ); 

	// 태스크 스케줄러를 시작한다.
	START_TASKSCHEDULER( TS_PRIMARY );
	START_TASKSCHEDULER( TS_SECONDARY );

	getchar();

	STOP_TASKSCHEDULER( 0 );
	getchar();

	return 0;
}

