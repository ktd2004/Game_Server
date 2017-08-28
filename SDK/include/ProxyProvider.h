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
#include <NetLink.h>
#include <NetEventHandler.h>

/** 
	RemoteProvider에서 네트웍으로 요청한 쿼리를 실행하고 결과를 전달
*/
class ProxyProvider
	: public IDBProvider
{
	spRef<IDBProvider>	m_spDBProvider;			///< 데이타베이스 연결
	NetEventHandler		m_Net;					///< 네트웍 핸들러
public:
	ProxyProvider();
	virtual ~ProxyProvider();
	/** 
		데이타베이스에 연결하고 SQL을 수행할 준비를 한다.
		성공시 1 리턴, 실패시 0 이외의 값 리턴
		@param Prop 데이타베이스 설정값
	*/
	int32	Startup( const IDBProvider::Properties& Prop ) override;
	/** 
		데이타베이스와의 연결을 종료한다.
	*/
	void	Shutdown( void ) override;
	/** 
		실행할 SQL의 갯수를 조회한다.
	*/
	size_t	Size( void ) override;
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
		맴버함수는 기능 구현이 없다.
	*/
	int32	Push( Query* pQry, uint32 iMSec ) override;
	/**
		맴버함수는 기능 구현이 없다.
	*/
	Query*	Pop( QUID iQid, uint32 iMSec ) override;
	/**
		맴버함수는 기능 구현이 없다.
	*/
	bool	Pop( Query::QType& QryList ) override;
	/**
		맴버함수는 기능 구현이 없다.
	*/
	void	Wait( int32 iMSec );

private:
	void	EventTick( void );
	bool	OnReceived( NetLinkPtr spLink, Buffer* pBuf );
	Query*	QueryDeserialize( NetLinkPtr spLink, Buffer* pBuf );
};
