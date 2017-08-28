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

#include <MacroFunc.h>
#include <Application.h>

extern ServerApp* theApp;

namespace Application
{
	void CheckStateDuration( uint32 tDuration )
	{
		if ( theApp )
		{ theApp->CheckStateDuration(tDuration);}
	}

	std::string  GetProgramName()
	{
		istrue_ret ( theApp, theApp->GetProgramName() );
		return "";
	}

	std::string GetSectionName()
	{
		istrue_ret ( theApp, theApp->GetSectionName() );
		return "";
	}

	std::string GetSetupFileName()
	{
		istrue_ret ( theApp, theApp->GetSetupFileName() );
		return "";
	}

	bool IsAgentMode()
	{
		istrue_ret( theApp, theApp->IsAgentMode() );
		return false;
	}

	std::string GetAgentAddr()
	{
		istrue_ret( theApp, theApp->GetAgentAddr() );
		return std::string("localhost");
	}

	int32 GetAgentPort()
	{
		istrue_ret( theApp, theApp->GetAgentPort() );
		return 3233;
	}

	int32 GetSID()
	{
		istrue_ret( theApp, theApp->GetSID() );
		return -1;
	}

	void QuitApplication(int32 iExitCode)
	{
		if ( theApp )
		{ theApp->QuitApplication(iExitCode);}
	}

	int32 ExitCode()
	{
		istrue_ret( theApp, theApp->ExitCode() );
		return 0;
	}

	void SetBlockClient( bool bBlock )
	{
		if ( theApp )
		{
			theApp->SetBlockClient(bBlock);
		}
	}

	bool GetBlockClient( void )
	{
		istrue_ret( theApp, theApp->GetBlockClient() );
		return false;
	}

	void SetMaxClient( uint32 iMaxClient )
	{
		if ( theApp )
		{
			theApp->SetMaxClient(iMaxClient);
		}
	}

	uint32 GetMaxClient( void )
	{
		istrue_ret( theApp, theApp->GetMaxClient() );
		return 0;
	}

	void SetUsers( uint32 iUsers )
	{
		if ( theApp )
		{
			theApp->SetUsers(iUsers);
		}
	}

	uint32 GetUsers( void )
	{
		istrue_ret( theApp, theApp->GetUsers() );
		return 0;
	}

	ServerApp::Properties* GetProperty( void )
	{
		if ( theApp )
		{
			return &theApp->GetProperty();
		}
		else
		{
			return nullptr;
		}
	}
};
