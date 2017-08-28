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

#include <Proactor.h>
#include <ServerFrame.h>
#include <DBEngine.h>
#include <Management.h>

class LogHandler
	: public ServerApp
	, public NetEventHandler
{
private:
	uint32					m_iBindPort;
	std::string				m_sBindAddr;

public:
	LogHandler(int argc, char* argv[]);
	virtual ~LogHandler();

	bool OnLogTransmit( NetLinkPtr spLink, uint32 iLevel, const std::string& sLog );

protected:
	bool OnSetProperty( ServerApp::Properties& appCfg ) override;
	bool OnInitialized(int32 argc, char* argv[]) override;
	void OnQuitApplication( void ) override;
	bool OnAccepted( NetLinkPtr spLink );
	bool OnConnected( NetLinkPtr spLink );
	bool OnClosed( NetLinkPtr spLink );
};