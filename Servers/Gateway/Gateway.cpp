// Auth.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "GatewayApp.h"
#include <MemoryEx.h>
#include <AAA.h>

#define CONFIG_FILE		".\\servers.ini"
#define SECTION_NAME	GTWS


int _tmain( int argc, _TCHAR* argv[] )
{
	RUN_SERVER_APP(GatewayApp);
}
