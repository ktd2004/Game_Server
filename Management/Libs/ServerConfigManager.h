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

#include "INISequence.h"
#include <DBEngine.h>

class ServerConfigManager
{
	spRef<IDBEngine>	m_spDb;
	INISequence			m_INISeq;
	std::string			m_sResult;

public:
	ServerConfigManager();
	virtual ~ServerConfigManager();

	bool Connect( IDBProvider::Properties& DBProp );
	bool SaveDB(
		const std::string& sFileName,
		const std::string& sWorld,
		bool bDebug = false );
	bool SaveDB(
		const std::string sWorld,
		const std::string& sSID,
		const std::string& sSection,
		const std::string& sKey,
		const std::string& sValue,
		bool bDebug = false );
	bool SaveFile(
		const std::string& sFileName,
		const std::string& sWorld,
		const std::string& sSID,
		const std::string& sSection );
	bool DisplayConfig(
		const std::string sWorld,
		const std::string sSID,
		const std::string sSection );

private:
	void OnResultSet( IDBResult* pResult );
	void OnResultSetServerProcess(IDBResult* pResult);

};
