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

#include <IDBProvider.h>

class Query;
/** 
	DBEngine �߻�Ŭ����.
*/
class IDBEngine
{
public:

	IDBEngine() {}
	virtual ~IDBEngine() {}
	/** 
		����Ÿ ���̽� ����
		@param rProp ����Ÿ���̽� ���� ����
	*/
	virtual int32		Connect( const IDBProvider::Properties& rProp ) = 0;
	/** 
		����Ÿ ���̽� ����
	*/
	virtual void		Close() = 0;
	/** 
		����Ÿ ���̽� ���� ���� Ȯ��
	*/
	virtual bool		IsConnected() = 0;
	/** 
		SQL ����
		@param qry ������ ���� ������Ʈ
		@return : 0 �̿ܿ��� ����
	*/
	virtual int32		Execute( Query* qry ) = 0;
	/** 
		SQL ���� ��� ��ȸ
	*/
	virtual const char*	ErrorMsg( void ) = 0;
};