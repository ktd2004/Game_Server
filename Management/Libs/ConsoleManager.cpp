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
#include <Logger.h>
#include "ConsoleManager.h"

//////////////////////////////////////////////////////////////////////////

ConsoleManager::ConsoleManager()
{

}

ConsoleManager::~ConsoleManager()
{

}

ConsoleManager* ConsoleManager::instance()
{
	static ConsoleManager sInstance;
	return &sInstance;
}

NetLinkPtr ConsoleManager::Find(int32 iUniqueID)
{
	auto it = m_ConsoleMap.find(iUniqueID);
	if (it != m_ConsoleMap.end())
		return it->second->m_spLink;

	return nullptr;
}

void ConsoleManager::Remove(NetLinkPtr spLink)
{
	Console* pConsole = (Console*)spLink->UserData();
	if (pConsole != nullptr)
	{
		pConsole->m_spLink = nullptr;
		m_ConsoleMap.erase(pConsole->m_iIndex);
	}
	else
	{
		for (auto it = m_ConsoleMap.begin(); it != m_ConsoleMap.end(); ++it)
		{
			if (it->second->m_spLink == spLink)
			{
				it->second->m_spLink = nullptr;
				m_ConsoleMap.erase(it);
				return;
			}
		}
	}
}

Console* ConsoleManager::Add(NetLinkPtr spLink)
{
	Console* pConsole = (Console*)spLink->UserData();
	if (pConsole != nullptr)
	{
		auto it = m_ConsoleMap.find(pConsole->m_iIndex);
		if (it == m_ConsoleMap.end())
		{
			pConsole->m_spLink = spLink;
			m_ConsoleMap.insert(std::map<int32, ConsolePtr>::value_type(pConsole->m_iIndex, pConsole));
		}
	}
	else
	{
		pConsole = new Console(m_iSeq++);
		pConsole->m_spLink = spLink;
		m_ConsoleMap.insert(std::map<int32, ConsolePtr>::value_type(pConsole->m_iIndex, pConsole));
	}

	return pConsole;
}
