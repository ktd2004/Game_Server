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
	uint32			m_iSeq;			///< �Ϸù�ȣ
	uint32			m_iPAID;		///< ���μ���������ID
	uint32			m_iSID;			///< ����ID
	uint32			m_iPID;			///< ���μ���ID
	std::string		m_sAlias;		///< �����̸�
	std::string		m_sExecPath;	///< �������ϰ��
	std::string		m_sExec;		///< �������ϸ�
	std::string		m_sArgs;		///< �����ť��Ʈ
	std::string		m_sWorld;		///< �����̸�
	bool			m_bAutoStart;	///< �ڵ�����
	bool			m_bBlock;		///< Ŭ���̾�Ʈ���ӱ���
	int32			m_iMaxClient;	///< �ִ�Ŭ���̾�Ʈ��
	bool			m_bSavePerf;	///< �����������忩��
	uint32			m_iState;		///< ��������
	uint32			m_iUsers;		///< ������������������
	NetLinkPtr		m_spServerLink;	///< ���μ��������ڿ� ����� ����

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