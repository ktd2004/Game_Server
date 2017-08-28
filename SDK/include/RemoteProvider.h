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
	ProxyProvider에 네트웍으로 쿼리 실행을 요청하고 결과를 테스크스케줄러에 전달.
*/
class RemoteProvider
	: public IDBProvider
{
	friend class QueryHandler;
public:
	/** 
		전송요청한 쿼리 데이타 구조체
	*/
	struct ReqQryData
	{
		Query*     pQry;			///< 수행할 쿼리 오브젝트
		uint64     iReqTick;		///< 전송 요청한 시간
		IDBResult* pResult;			///< 쿼리 수행 결과
		ReqQryData()
			: pQry(nullptr), iReqTick(0)
		{}
		ReqQryData(Query* qry, uint64 iTick)
			: pQry(qry), iReqTick(iTick)
		{}
	};
protected:
	typedef std::map<uint64, ReqQryData> ReqQryMap;

	NetEventHandler		m_Net;				///< 네트웍 핸들러
	Sync				m_ReqQryMutex;		///< 쿼리 요청 잠금
	ReqQryMap			m_ReqQryMap;		///< 쿼리 요청 맵
	Sync				m_ResultMutex;		///< 쿼리 수행 완료 잠금
	Query::QType		m_Result;			///< 쿼리 완료 큐
	int32				m_iTaskID;			///< 테스크ID
	NetLinkPtr			m_spLink;			///< 원격 ProxyProvider
	Query::QType		m_Reserved;			///< 쿼리 예약 큐
	Signal				m_SyncEvt;			///< 동기화 쿼리 이벤트
	uint64				m_iSeq;				///< 쿼리 일련번호
	TaskScheduler*		m_pTaskScheduler;	///< 쿼리 완료 처리를 할 테스크스케줄러 ID

public:
	RemoteProvider();
	virtual ~RemoteProvider();
	/** 
		데이타베이스에 연결하고 SQL을 수행할 준비를 한다.
		성공시 1 리턴, 실패시 0 이외의 값 리턴
		@param Prop 데이타베이스 설정값
	*/
	int32	Startup( const IDBProvider::Properties& Prop ) override;
	/** 
		데이타베이스와의 연결을 종료한다.
	*/
	void    Shutdown( void ) override;
	/** 
		실행할 SQL의 갯수를 조회한다.
	*/
	size_t  Size( void ) override;
	/** 
		SQL 실행을 요청한다.
		@param pQry 실행할 쿼리 오브젝트
	*/
	int32	Push( Query* pQry ) override;
	/** 
		SQL 실행을 요청한다.
		@param QryList 실행할 퀴리 오브젝트가 저장된 리스트
	*/
	int32	Push( Query::QType& QryList ) override;
	/**
		SQL 실행을 요청한다.
		@param pQry 실행할 쿼리 오브젝트
		@param iMSec 대기시간(msec)
	*/
	int32	Push( Query* pQry, uint32 iMSec ) override;
	/**
		맴버함수는 기능 구현이 없다.
	*/
	Query*	Pop( QUID iQid, uint32 usec ) override;
	/**
		맴버함수는 기능 구현이 없다.
	*/
	bool	Pop( Query::QType& QryList ) override;
	/**
		맴버함수는 기능 구현이 없다.
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
