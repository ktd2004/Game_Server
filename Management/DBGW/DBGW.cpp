#include "stdafx.h"
#include <MemoryEx.h>
#include "DbgwHandler.h"

#define CONFIG_FILE		".\\DBGW.ini"
#define SECTION_NAME	DBGW

int _tmain(int argc, _TCHAR* argv[])
{
	RUN_SERVER_APP(DbgwHandler);
}

