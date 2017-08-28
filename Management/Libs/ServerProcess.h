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
#include <NetLink.h>

PTR(ServerProcess)
class ServerProcess : public single_ref
{
public:
	uint32			m_iSeq;			///< 일련번호
	uint32			m_iPAID;		///< 프로세스관리자ID
	uint32			m_iSID;			///< 서버ID
	uint32			m_iPID;			///< 프로세스ID
	std::string		m_sAlias;		///< 서버이름
	std::string		m_sExecPath;	///< 실행파일경로
	std::string		m_sExec;		///< 실행파일명
	std::string		m_sArgs;		///< 실행아큐먼트
	std::string		m_sWorld;		///< 월드이름
	bool			m_bAutoStart;	///< 자동실행
	bool			m_bBlock;		///< 클라이언트접속금지
	int32			m_iMaxClient;	///< 최대클라이언트수
	bool			m_bSavePerf;	///< 성능정보저장여부
	uint32			m_iState;		///< 서버상태
	uint32			m_iUsers;		///< 서버에접속한유저수
	NetLinkPtr		m_spServerLink;	///< 프로세스관리자에 연결된 서버

public:
	ServerProcess();
	ServerProcess( const ServerProcess& r );
	ServerProcess(
		uint32 iPAID,
		uint32 iSID,
		const std::string& sAlias,
		const std::string& sExecPath,
		const std::string& sExec, 
		const std::string& sArgs,
		const std::string& sWorld,
		bool bAutoRestart=false );
	virtual ~ServerProcess();

	ServerProcess& operator = (const ServerProcess& r);
};