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

#include <Ptr.h>
#include <FastList.h>
#include <IDBResult.h>
#include <IDBCommand.h>

class IDBCommand;
class IDBResult;

/** 
	SQL ���� ������ �����ϰ� �ִ� Ŭ����.

	IDBProvider ������Ʈ�� ���޵Ǿ� SQL�� ����ǰ� ��� ���� Query ������Ʈ�� ����ȴ�.
*/
class Query
	: public listnode(Query)
{
public:
#ifdef USE_UTIL_LIST_AT_QUERY_QUEUE
	typedef util::list<Query> QType;
#else
	typedef std::list<Query*> QType;
#endif

	typedef IDBCommand::DBCommandType QueryType;
	uint64				m_iOid;			///< ����������Ʈ�� ����ID
	spRef<IDBResult>	m_spResult;		///< SQL ���� ���

protected:
	const QUID		m_iQid;				///< SQL�� �������̵�
	std::string		m_Sql;				///< ������ SQL
	QueryType		m_eCommandType;		///< SQL�� ����
	int32			m_iErrorCode;		///< �����ڵ�
	std::string		m_sErrorMsg;		///< �����޼���
	bool			m_bSyncQry;			///< ����ȭ ����

public:
	Query();
	Query(QUID iQid, IDBCommand::DBCommandType eCmd = IDBCommand::ctSP );
	virtual ~Query();

	/** 
		������ SQL�� �����Ѵ�.
		@param sql ������ SQL ����
	*/
	void				SetStmt(const std::string& sql);
	/** 
		������ SQL�� ��ȸ�Ѵ�.
	*/
	const std::string&	GetStmt( void );
	/** 
		������ SQL�� �������̵� ���´�.
	*/
	QUID				GetQid( void );
	/** 
		DBProvider���� ����������Ʈ�� SQL�� �ĸ����Ͱ��� ���ε� �ϱ� ���ؼ� ȣ��ȴ�. 
		@param pCmd �ĸ����Ͱ��� ���ε��ϱ� ���� OnSetParam �� ���޵� DBCommand ������Ʈ
	*/
	void				MakeCommand( IDBCommand* pCmd );
	/** 
		SQL ������ ������ �߻��ߴ��� ����
	*/
	bool				IsError( void );
	/** 
		SQL ������ �߻��� �����ڵ� ��ȸ
	*/
	int32				GetError( void );
	/** 
		SQL ������ �߻��� �����޼����� ��ȸ
	*/
	const std::string&	GetErrorMsg( void );
	/** 
		����ȭ SQL�� �����Ѵ�.
	*/
	void				Sync( void );
	/** 
		����ȭ SQL���� �����Ѵ�.
	*/
	bool				IsSync( void );
	/** 
		SQL�� �ĸ����Ͱ��� ���ε� �ϱ� ���ؼ� DBProvider���� ����������Ʈ�� OnSetParam �Լ��� ȣ��ȴ�.
		@param pCmd �ĸ����Ͱ��� ���ε��ϱ� ���� OnSetParam �� ���޵� DBCommand ������Ʈ
	*/
	virtual void	OnSetParam( IDBCommand* pCmd ) {}
	/** 
		SQL���� ����� �����ϱ� ���ؼ� DBProvider���� ȣ��ȴ�.
		@param pResult SQL ���� ����� ��� �ִ� ������Ʈ
	*/
	virtual void	OnQuery( IDBResult* pResult ) {}
	/** 
		SQL������ ������ �߻������� DBProvider���� ȣ��ȴ�.
		@param iErrorCode SQL ���� �����ڵ�
		@param sErrorMsg SQL ���� �����޼���
	*/
	virtual void	OnError( int32 iErrorCode, const std::string& sErrorMsg );
	/** 
		SQL���� ����� �����ϱ� ���ؼ� DBProvider�� ����� �׽�ũ�����ٷ����� ȣ��ȴ�.
		@param pResult SQL ���� ����� ��� �ִ� ������Ʈ
	*/
	virtual void	OnResult( void ) {}
};
