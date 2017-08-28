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

class DBValue;
/** 
	SQL���� ������ DBEngine���� ������ SQL�� �� �Ķ���͸� �����ϴ� �߻�Ŭ����.
*/
class IDBCommand
{
public:
	IDBCommand() {}
	virtual ~IDBCommand() {}

	/** 
		SQL���� ����
	*/
	enum DBCommandType
	{
		ctSQL = 1,		///< SQL ���ڿ�
		ctSP = 2,		///< SP ȣ��
	};
	/** 
		SQL���� �� SQL�� ������ �����Ѵ�.
		@param eCmdType SQL�� ����
		@param SQL SQL����
	*/
	virtual int32						SetCommand( DBCommandType eCmdType, const std::string& SQL ) = 0;
	/** 
		SQL���� �Ķ���͸� �����Ѵ�.
		@param v �Ķ���Ͱ�
		@param escape Ư������ ó���� ���� ����
	*/
	virtual int32						AddParam( const DBValue& v, bool escape = false ) = 0;
	/** 
		SQL���� �Ķ���͸� �����Ѵ�.
		@param v �Ķ���Ͱ�
	*/
	virtual int32						AddParam( std::vector<DBValue>& v ) = 0;
	/** 
		�Ķ������ ������ ��ȸ�Ѵ�.
	*/
	virtual int32						GetParamCount( void ) = 0;
	/** 
		SQL�� �������� ��ȸ�� ������� Bytes ���� ����Ѵ�.
	*/
	virtual int32						GetEastimateBytes( void ) = 0;
	/** 
		SQL������ ���´�.
	*/
	virtual const std::string&			GetCommand( void ) = 0;
	/** 
		SQL������ ������ ���´�.
	*/
	virtual DBCommandType				GetCommandType( void ) = 0;
	/** 
		�Ķ������ ���� ����� �����̳ʸ� ���´�.
	*/
	virtual const std::vector<DBValue>& GetParams( void ) = 0;
};
