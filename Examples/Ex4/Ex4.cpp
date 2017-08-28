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
	�½�ũ �����ٷ� 2���� ������ �� �½�ũ �����ٷ� ���� �񵿱� ���ν����� ȣ���ϴ� ����
*/

const unsigned int TS_PRIMARY = 1;
const unsigned int TS_SECONDARY = 2;
TaskScheduler* sc1 = nullptr;
TaskScheduler* sc2 = nullptr;

// 1�ʿ� �ѹ��� �񵿱� ���ν��� ItemManager::AddItem, RemoveItem �� ȣ���Ѵ�.
class PlayerManager : public shared_ref
{
	int		i;
public:
	PlayerManager()
	{
		i = 0;
		incref();
		// ItemManager::AddItem�� ���� ����� �޾ƿ��� ���� �񵿱� ���ν��� ����
		EXPORT_PROC_V2( TS_PRIMARY, 1001, this, &PlayerManager::ResultOperation, int, const std::string& );
	}
	virtual ~PlayerManager() {}

	// ItemManager�� AddItem, RemoveItem �� �񵿱� ���ν����� �����Ѵ�.
	// �񵿱� ���ν����� ȣ���Ҷ��� proc_AddItem, proc_RemoveItem ���� ȣ���ؾ� �Ѵ�.
	IMPORT_PROC_V2( 1000, AddItem, int, int, 1024 );
	IMPORT_PROC_V2( 2000, RemoveItem, int, int, 1024 );

	void EventTick()
	{
		// 1�ʿ� �ѹ��� ItemManager::AddItem, RemoveItem �� ȣ���Ѵ�.
		i++;
		if ( i % 2 == 1 )
		{
			printf( "�񵿱� ���ν��� ItemManager::AddItem ȣ��\n" );
			proc_AddItem( nullptr, 1, i );
		}
		else
		{
			printf( "�񵿱� ���ν��� ItemManager::RemoveItem ȣ��\n" );
			proc_RemoveItem( nullptr, 1, i );
		}
	}

	// proc_AddItem, proc_RemoveItem ���� ����� �޾ƿ´�.
	bool ResultOperation( interface_ref* pRef, int iPlayerID, const std::string& sResult )
	{
		printf( "    -->���(%s): iPlayerID(%d), %s\n", __FUNCTION__, iPlayerID, sResult.c_str() );
		return true;
	}
};

// PlayerManager ���� AddItem, RemoveItem �� ȣ�� �� �� �ֵ��� �� �Լ��� �񵿱� ���ν����� �����Ѵ�.
class ItemManager : public shared_ref
{
public:
	ItemManager()
	{
		incref();
		// AddItem, RemoveItem �� �񵿱� ���ν����� �����Ѵ�.
		EXPORT_PROC_V2( TS_SECONDARY, 1000, this, &ItemManager::AddItem, int, int );
		EXPORT_PROC_V2( TS_SECONDARY, 2000, this, &ItemManager::RemoveItem, int, int );
	}
	virtual ~ItemManager() {}

	void EventTick()
	{
	}

	// AddItem, RemoveItem �� ������ ����� �˷��ִ� �񵿱� ���ν����� ����Ѵ�.
	IMPORT_PROC_V2( 1001, ResultOperation, int, const std::string&, 1024 );

	bool AddItem( interface_ref* pCaller, int iPlayerID, int iItemID )
	{
		printf( "    -->�񵿱� ���ν��� ����(%s): iPlayerID(%d), iItemID(%d)\n", __FUNCTION__, iPlayerID, iItemID );
		std::string sResult = "additem success : " + util::intstr(iItemID);
		// PlayerManager �� AddItem ���� ����� �˷��ش�.
		proc_ResultOperation( nullptr, iPlayerID, sResult );
		return true;
	}

	bool RemoveItem( interface_ref* pRef, int iPlayerID, int iItemID)
	{
		printf( "    -->�񵿱� ���ν��� ����(%s): iPlayerID(%d), iItemID(%d)\n", __FUNCTION__, iPlayerID, iItemID );
		std::string sResult = "removeitem success : " + util::intstr(iItemID);
		// PlayerManager �� RemoveItem ���� ����� �˷��ش�.
		proc_ResultOperation( nullptr, iPlayerID, sResult );
		return true;
	}
};

#include <Errorcodes.h>

int _tmain(int argc, _TCHAR* argv[])
{
	printf( "�½�ũ �����ٷ� 2���� ������ �� �½�ũ �����ٷ� ����\n�񵿱� ���ν����� ȣ���ϴ� ����\n" );

	// �½�ũ �����ٷ� 1�� �����Ѵ�.
	sc1 = CREATE_TASKSCEDULER( TS_PRIMARY, 50, 1 );
	if ( sc2 == nullptr && GetLastError() != NOERROR )
	{
		printf( "error: %s\n", GetLastErrorMessage(GetLastError()) );
		return 1;
	}
	// �½�ũ �����ٷ� 2�� �����Ѵ�.
	sc2 = CREATE_TASKSCEDULER( TS_SECONDARY, 50, 2 );
	if ( sc2 == nullptr && GetLastError() != NOERROR )
	{
		printf( "error: %s\n", GetLastErrorMessage(GetLastError()) );
		return 1;
	}
	
	PlayerManager inst1;
	ItemManager inst2;
	
	// 1�ʿ� �ѹ��� EventTick �� ȣ���Ѵ�.
 	CREATE_TASK( TS_PRIMARY, &inst1, &PlayerManager::EventTick, 1000 ); 
 	CREATE_TASK( TS_SECONDARY, &inst2, &ItemManager::EventTick, 1000 ); 

	// �½�ũ �����ٷ��� �����Ѵ�.
	START_TASKSCHEDULER( TS_PRIMARY );
	START_TASKSCHEDULER( TS_SECONDARY );

	getchar();

	STOP_TASKSCHEDULER( 0 );
	getchar();

	return 0;
}

