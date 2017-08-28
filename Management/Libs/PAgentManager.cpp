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
#include <INILoader.h>
#include <StringUtil.h>
#include <MacroFunc.h>
#include "PAgentManager.h"

//////////////////////////////////////////////////////////////////////////

PAgentManager* PAgentManager::instance()
{
	static PAgentManager inst;
	return &inst;
}

PAgentManager::PAgentManager()
{
}

PAgentManager::PAgentManager( const PAgentManager& copy )
{
}

PAgentManager::~PAgentManager()
{
}

bool PAgentManager::Load( void )
{
	if ( !INILoader::instance()->IsLoaded() )
	{
		return false;
	}

	INILoader::SectionMap& Section = INILoader::instance()->GetSectionMap();
	foreach_do( INILoader::SectionMap, it, Section )
	{
		if ( it->first.find("DB") != std::string::npos ||
			util::upper(it->first) == SAGENT )
		{
			continue;
		}
		INILoader::KeyMap& Key = it->second;

		int32 iPAID       = Env_i( it->first, "id", 0 );
		std::string sPath = Env_s( it->first, "exec_path", "d:\\Server\\" );
		if ( sPath[sPath.size()-1] != '\\' )
		{ sPath += "\\"; }

		// 여기에 정의된 서버명으로 DB(SERVER_CONFIG.sSection)의 조회키로 사용된다.
		std::string Servers = Env_s( it->first, "managed_server_name", null_string );
		stringlist ServerList;
		util::tokenizer( Servers, ServerList, ',' );
		if( ServerList.empty() )
		{
			continue;
		}

		foreach_do( stringlist, token, ServerList )
		{
			std::string sKey;
			sKey = *token + "_id";
			int32 iSID = Env_i( it->first, sKey, 0 );
			//			printf( "%s=%d\n", cstr(sKey),iSID );

			sKey = *token + "_exec";
			std::string sExec = Env_s( it->first, sKey, null_string );
			//			printf( "%s=%s\n", cstr(sKey),cstr(sExec) );

			sKey = *token + "_args";
			std::string sArgs = Env_s( it->first, sKey, null_string );
			//			printf( "%s=%s\n", cstr(sKey),cstr(sArgs) );

			sKey = *token + "_auto_restart";
			bool bAutoRestart = Env_b( it->first, sKey, false );
			//			printf( "%s=%s\n", cstr(sKey),bool2str(bAutoRestart) );

			sKey = *token + "_world";
			std::string sWorld = Env_s( it->first, sKey, null_string );
			//			printf( "%s=%s\n", cstr(sKey),cstr(sWorld) );

			if ( !Add( 
				it->first,		// 프로세스관리자이름
				iPAID,			// 프로세스관리자ID
				iSID,			// 서버ID
				*token,			// 별칭
				sPath,			// 실행파일경로
				sExec,			// 실행파일이름
				sArgs,			// 아큐먼트
				sWorld,			// 월드이름
				bAutoRestart ) )// 자동실행여부
			{
				break;
			}
		}
	}

	return true;
}

ServerProcessPtr PAgentManager::Add(
	const std::string& sPAgentName,
	uint32 iPAID,
	uint32 iSID,
	const std::string& sAlias,
	const std::string& sExecPath,
	const std::string& sExec,
	const std::string& sArgs,
	const std::string& sWorld,
	bool bAutoRestart )
{
	ServerProcessPtr spServer = new ServerProcess(
		iPAID,
		iSID,
		sAlias,
		sExecPath,
		sExec,
		sArgs,
		sWorld,
		bAutoRestart );

	return Add(sPAgentName, spServer);
}

ServerProcessPtr PAgentManager::Add( const std::string& sPAgentName, ServerProcessPtr spServer )
{
	ServerProcessManagerPtr spServerProcessManager;

	if ( m_PAgentManagerMap.count(spServer->m_iPAID) )
	{
		spServerProcessManager = m_PAgentManagerMap[spServer->m_iPAID];
		return spServerProcessManager->Add( spServer );
	}
	else
	{
		spServerProcessManager = new ServerProcessManager(sPAgentName,spServer->m_iPAID);
		if ( !m_PAgentManagerMap.insert(PAgentManagerMap::value_type(spServer->m_iPAID, spServerProcessManager)).second )
			return false;;
		if ( !m_PAgentManagerNameMap.insert(PAgentManagerNameMap::value_type(sPAgentName, spServerProcessManager)).second )
			return false;
		return spServerProcessManager->Add( spServer );
	}
}

ServerProcessMap* PAgentManager::GetServerProcessMap( uint32 iPAID )
{
	notfound_map_ret(PAgentManagerMap, it, m_PAgentManagerMap, iPAID, nullptr);
	return &(it->second->GetMap());
}

PAgentManagerMap& PAgentManager::GetPAgentManagerMap( void )
{
	return m_PAgentManagerMap;
}

void PAgentManager::Remove( uint32 iPAID, uint32 iSID )
{
	notfound_map_ret( PAgentManagerMap, it, m_PAgentManagerMap, iPAID, );
	it->second->Remove( iSID );
}

void PAgentManager::Remove( const std::string& sPAgentName, const std::string& sAlias )
{
	notfound_map_ret( PAgentManagerNameMap, it, m_PAgentManagerNameMap, sPAgentName, );
	it->second->Remove( sAlias );
}

ServerProcessPtr PAgentManager::Find( uint32 iPAID, uint32 iSID )
{
	notfound_map_ret(PAgentManagerMap, it, m_PAgentManagerMap, iPAID, nullptr);
	return it->second->Find( iSID );
}

ServerProcessPtr PAgentManager::Find( const std::string& sPAgentName, const std::string& sAlias )
{
	notfound_map_ret(PAgentManagerNameMap, it, m_PAgentManagerNameMap, sPAgentName, nullptr);
	return it->second->Find( sAlias );
}

ServerProcessManagerPtr PAgentManager::GetServerProcessManager( uint32 iPAID )
{
	notfound_map_ret(PAgentManagerMap, it, m_PAgentManagerMap, iPAID, nullptr);
	return it->second;
}

ServerProcessManagerPtr PAgentManager::GetServerProcessManager( const std::string& sPAgentName )
{
	notfound_map_ret(PAgentManagerNameMap, it, m_PAgentManagerNameMap, sPAgentName, nullptr);
	return it->second;
}

void PAgentManager::Dump( void )
{
	foreach_do( PAgentManagerMap, it, m_PAgentManagerMap )
	{
		it->second->Dump();
	}
}