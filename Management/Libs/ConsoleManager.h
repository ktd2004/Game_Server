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

#include <NetLink.h>
#include "ServerProcess.h"

//////////////////////////////////////////////////////////////////////////

PTR(Console)
class Console : public single_ref
{
public:
	int32		m_iIndex;
	NetLinkPtr	m_spLink;
public:
	Console() : m_iIndex(0) {}
	Console(int32 iIndex) : m_iIndex(iIndex) {}
};

class ConsoleManager
{
private:
	std::map<int32, ConsolePtr>	m_ConsoleMap;
	int32		m_iSeq;

private:
	ConsoleManager();
	ConsoleManager(const ConsoleManager& r) {}
	virtual ~ConsoleManager();

public:
	static ConsoleManager* instance();
	NetLinkPtr Find(int32 iUniqueID);
	Console* Add(NetLinkPtr spLink);
	void Remove(NetLinkPtr spLink);
};
#define CONSOLE_MGR()		ConsoleManager::instance()