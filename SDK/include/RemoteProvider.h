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

#include <Sync.h>
#include <Signal.h>
#include <IDBProvider.h>
#include <NetEventHandler.h>

/** 
	ProxyProvider�� ��Ʈ������ ���� ������ ��û�ϰ� ����� �׽�ũ�����ٷ��� ����.
*/
class RemoteProvider
	: public IDBProvider
{
	friend class QueryHandler;
public:
	/** 
		���ۿ�û�� ���� ����Ÿ ����ü
	*/
	struct ReqQryData
	{
		Query*     pQry;			///< ������ ���� ������Ʈ
		uint64     iReqTick;		///< ���� ��û�� �ð�
		IDBResult* pResult;			///< ���� ���� ���
		ReqQryData()
			: pQry(nullptr), iReqTick(0)
		{}
		ReqQryData(Query* qry, uint64 iTick)
			: pQry(qry), iReqTick(iTick)
		{}
	};
protected:
	typedef std::map<uint64, ReqQryData> ReqQryMap;

	NetEventHandler		m_Net;				///< ��Ʈ�� �ڵ鷯
	Sync				m_ReqQryMutex;		///< ���� ��û ���
	ReqQryMap			m_ReqQryMap;		///< ���� ��û ��
	Sync				m_ResultMutex;		///< ���� ���� �Ϸ� ���
	Query::QType		m_Result;			///< ���� �Ϸ� ť
	int32				m_iTaskID;			///< �׽�ũID
	NetLinkPtr			m_spLink;			///< ���� ProxyProvider
	Query::QType		m_Reserved;			///< ���� ���� ť
	Signal				m_SyncEvt;			///< ����ȭ ���� �̺�Ʈ
	uint64				m_iSeq;				///< ���� �Ϸù�ȣ
	TaskScheduler*		m_pTaskScheduler;	///< ���� �Ϸ� ó���� �� �׽�ũ�����ٷ� ID

public:
	RemoteProvider();
	virtual ~RemoteProvider();
	/** 
		����Ÿ���̽��� �����ϰ� SQL�� ������ �غ� �Ѵ�.
		������ 1 ����, ���н� 0 �̿��� �� ����
		@param Prop ����Ÿ���̽� ������
	*/
	int32	Startup( const IDBProvider::Properties& Prop ) override;
	/** 
		����Ÿ���̽����� ������ �����Ѵ�.
	*/
	void    Shutdown( void ) override;
	/** 
		������ SQL�� ������ ��ȸ�Ѵ�.
	*/
	size_t  Size( void ) override;
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
		SQL ������ ��û�Ѵ�.
		@param pQry ������ ���� ������Ʈ
		@param iMSec ���ð�(msec)
	*/
	int32	Push( Query* pQry, uint32 iMSec ) override;
	/**
		�ɹ��Լ��� ��� ������ ����.
	*/
	Query*	Pop( QUID iQid, uint32 usec ) override;
	/**
		�ɹ��Լ��� ��� ������ ����.
	*/
	bool	Pop( Query::QType& QryList ) override;
	/**
		�ɹ��Լ��� ��� ������ ����.
	*/
	void	Wait( int32 usec );

private:
	void	EventTick();
	bool	OnConnected( NetLinkPtr spLink );
	bool	OnReceived( NetLinkPtr spLink, Buffer* pBuf );
	bool	OnClosed( NetLinkPtr spLink );
	Buffer* QuerySerialize( Query* pQry );
	Query*	QueryDeserialize( Buffer* pBuf );
	void	AddQuery( Query* pQry );
	bool	SendQuery( NetLinkPtr spLink, Buffer* pBuf );
};
