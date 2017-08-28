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
#include <IDBProvider.h>

/** 
	����Ÿ ���̽� �������� �ε� ��ƿ��Ƽ ����.
*/
namespace util
{
	/** 
		���������� ����Ÿ���̽� ���ǿ��� ����Ÿ���̽� ���� ������ ���� �ʿ��� ������ �ε��Ѵ�.
		���ο��� INILoader�� ����Ͽ� ���� ������ �ε��ϵ��� �����Ǿ� �ִ�.
		@param sDBSection ����Ÿ���̽����Ǹ�
		@param config ����Ÿ���̽� ���������� ä���� ����ü
	*/
	void load_dbcfg(const std::string& sDBSection, IDBProvider::Properties& config);
	/** 
		���������� ����Ÿ���̽� ���ǿ��� ����Ÿ���̽� ���� ������ ���� �ʿ��� ������ �ε��Ѵ�.
		���ο��� INILoader�� ����Ͽ� ���� ������ �ε��ϵ��� �����Ǿ� �ִ�.
		@param sDBSection ����Ÿ���̽����Ǹ�
		@param config ����Ÿ���̽� ���������� ä���� ����ü
	*/
	void load_remotedbcfg(const std::string& sDBSection, IDBProvider::Properties& config);
	/** 
		����Ÿ���̽� ���� ���� ����ü�� ������ ����Ѵ�.
		@param config ����Ÿ���̽� ��������
	*/
	void show_dbcfg(IDBProvider::Properties& config);
	/** 
		����Ÿ���̽� ���� ���� ����ü�� ������ ����Ѵ�.
		@param config ����Ÿ���̽� ��������
	*/
	void show_remotedbcfg(IDBProvider::Properties& config);
}