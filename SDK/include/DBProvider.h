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

//
#include <Ptr.h>
#include <IDBProvider.h>
#include <ThreadFactory.h>
#include <QueryQueue.h>
#include <TaskScheduler.h>

class DBProvider
	: public IDBProvider
{
	typedef std::vector<spRef<QueryQueue>>	QueryQueueVec;
	typedef std::vector<spRef<IDBEngine>>	DBEngineQueVec;
	typedef std::list<Thread*>				ThreadSpawnList;

	class QuitMsg :
		public Query
	{
	public:
		QuitMsg(QUID iQid) : Query(iQid)
		{}
	};

	ThreadSpawnList m_SpawnList;
	QueryQueueVec	m_QueryQue;
	QueryQueueVec	m_ResultQue;
	DBEngineQueVec	m_EngineQue;
	int32			m_iTaskID;
	TaskScheduler*	m_pTaskScheduler;

	enum
	{
		eDBPROVIDER_SHUTDOWN		= -999,
		eDBPROVIDER_NO_WAIT			= 0,
		eDBPROVIDER_WAIT_ONE_SECOND	= 1000,
	};
public:
	DBProvider();
	virtual ~DBProvider();
	/** 
		����Ÿ���̽��� �����ϰ� SQL�� ������ �غ� �Ѵ�.
		������ 1 ����, ���н� 0 �̿��� �� ����
		@param Prop ����Ÿ���̽� ������
	*/
	int32	Startup( const IDBProvider::Properties& Prop ) override;
	/** 
		����Ÿ���̽����� ������ �����Ѵ�.
	*/
	void	Shutdown( void ) override;
	/** 
		������ SQL�� ������ ��ȸ�Ѵ�.
	*/
	size_t	Size( void ) override;
	/** 
		SQL ������ ��û�Ѵ�.
		@param pQry ������ ���� ������Ʈ
	*/
	int32	Push( Query* pQry ) override;
	/** 
		SQL ������ ��û�Ѵ�.
		@param QryList ������ ���� ������Ʈ�� ����� ����Ʈ
	*/
	int32	Push( Query::QType& QryList ) override;
	/** 
		���� �Ϸ�� SQL ������Ʈ�� �����´�.
		@param iQid ����������Ʈ�� ����ID
		@param iMSec ���ð�(msec)
	*/
	Query*  Pop( QUID iQid, uint32 iMsec ) override;
	/** 
		���� �Ϸ�� SQL ������Ʈ�� �����´�.
		@param QryList ����������Ʈ�� ������ ����Ʈ
	*/
	bool    Pop( Query::QType& QryList ) override;

	/** �Ʒ� �ɹ��Լ��� ��� ������ ����. */
	int32	Push( Query* pQry, uint32 iMSec ) override;

private:
	void    EventTick( void );
	void	Running( int32 iInx );
	size_t	LoadBalance( void );
};
