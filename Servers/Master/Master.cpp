// Master.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <MemoryEx.h>
#include <AAA.h>
#include "MasterApp.h"

#define CONFIG_FILE		".\\servers.ini"
#define SECTION_NAME	MSTS

int _tmain( int argc, _TCHAR* argv[] )
{
	RUN_SERVER_APP(MasterApp);
}
