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
#include <IDBCommand.h>
/** 
	SQL ������ ���� ������ �����ϴ� Ŭ����
*/
class DBCommand 
	: public IDBCommand
{
	DBCommandType			m_eType;		///< ������ SQL �� ����
	std::string				m_SQL;			///< SQL ����
	std::vector<DBValue>	m_InParam;		///< SQL�� ���ε��� �Ķ���͸� ������ �����̳�
	std::vector<DBValue>*	m_pParam;		///< ProxyProvider ���ο��� ������ �Ķ���� �����̳ʸ� �����ϱ� ���ؼ� ���
public:
	DBCommand();
	virtual ~DBCommand();
	/** 
		������ SQL�� �����Ѵ�.
		@param eCmdType SQL ����
		@param SQL ������ SQL
	*/
	int32						SetCommand( DBCommandType eCmdType, const std::string& SQL ) override;
	/** 
		������ SQL�� ������ �Ķ���͸� �����Ѵ�.
		@param v �Ķ����
		@param escape �Ķ���Ͱ� ���ڿ��� ��� �̽������� ���ڿ� ó���� �Ѵ�.
	*/
	int32						AddParam( const DBValue& v, bool escape = false ) override;
	/** 
		������ SQL�� ������ �Ķ���� �����̳ʸ� �����Ѵ�.
		@param v �Ķ���� �����̳�
	*/
	int32						AddParam( std::vector<DBValue>& v ) override;
	/** 
		�Ķ������ ������ ��ȸ�Ѵ�.
	*/
	int32						GetParamCount( void ) override;
	/** 
		����� �Ķ���� ����Ÿ�� ����Ʈ���� ��ȸ�Ѵ�.
	*/
	int32						GetEastimateBytes( void ) override;
	/** 
		����� �Ķ���� �����̳ʸ� �����Ѵ�.
	*/
	std::vector<DBValue>&		GetParams( void ) override;
	/** 
		����� SQL ������ �����Ѵ�.
	*/
	const std::string&			GetCommand( void ) override;
	/** 
		����� SQL ������ ������ ��ȸ�Ѵ�.
	*/
	IDBCommand::DBCommandType	GetCommandType( void ) override;
};