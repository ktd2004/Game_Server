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
#pragma once

#include <DataTypes.h>
#include <DBEngine.h>
#include <Logger.h>
#include "Gateway2MasterHandler.h"
#include "Gateway2ClientHandler.h"
#include "Gateway2ChannelHandler.h"
#include "Master2GatewayHandler.h"
#include "Channel2GatewayHandler.h"

namespace AAAUtil
{
	inline bool GatewayInitialize(
		Gateway2MasterHandler* pMasterHandler,
		Gateway2ClientHandler* pClientHandler,
		Gateway2ChannelHandler* pChannelHandler,
		IDBProvider* pProvider )
	{
		if( !pMasterHandler->Initialize() )
		{
			if ( pProvider )
				pProvider->Shutdown();
			return false;
		}
		prn_inf( "Initialize master handler" );

		if (!pChannelHandler->Initialize())
		{
			if (pProvider)
				pProvider->Shutdown();
			return false;
		}
		prn_inf("Initialize channel handler");

		if( !pClientHandler->Initialize() )
		{
			pMasterHandler->Stop();
			if ( pProvider )
				pProvider->Shutdown();
			return false;
		}
		prn_inf( "Initialize client handler" );

		pMasterHandler->SetChannelHandler(pChannelHandler);
		pMasterHandler->SetClientHandler( pClientHandler );
		
		pChannelHandler->SetClientHandler( pClientHandler );

		pClientHandler->SetMasterHandler( pMasterHandler );
		pClientHandler->SetDBProvider( pProvider );

		if( !pMasterHandler->StartupMasterHandler() ||
			!pChannelHandler->StartupChannelHandler() )
		{
			pMasterHandler->Stop();
			pChannelHandler->Stop();
			if ( pProvider )
				pProvider->Shutdown();
			return false;
		}

		// delay
		Sleep(1000);
		if (!pClientHandler->StartupClientHandler())
		{
			pMasterHandler->Stop();
			pChannelHandler->Stop();
			if (pProvider)
				pProvider->Shutdown();
			return false;
		}

		return true;
	}

	inline bool MasterInitialize( Master2GatewayHandler* pMasterHandler, IDBProvider* pProvider )
	{
		if (!pMasterHandler->Initialize())
		{
			return false;
		}
		prn_inf( "Initialize master handler" );

		pMasterHandler->SetDBProvider(pProvider);

		if (!pMasterHandler->StartupMasterHandler())
		{
			pMasterHandler->Stop();
			return false;
		}

		return true;
	}

	inline bool ChannelInitialize( Channel2GatewayHandler* pChannelHandler, IDBProvider* pProvider )
	{
		if (!pChannelHandler->Initialize())
		{
			return false;
		}
		prn_inf( "Initialize channel handler" );

		pChannelHandler->SetDBProvider( pProvider );

		if ( !pChannelHandler->StartupChannelHandler() )
		{
			return false;
		}

		return true;
	}
}