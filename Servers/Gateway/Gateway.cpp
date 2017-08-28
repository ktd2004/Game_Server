// Auth.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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
