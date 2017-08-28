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
#include <Query.h>
#include <StringUtil.h>

class Query;
class IDBProvider;
class IDBProvider
{
public:
	/** 
		데이타베이스 연결 및 원격 접속 정보 설정 구조체
	*/
	struct Properties
	{
		/** 
			원격 접속 정보 설정 구조체
		*/
		struct Remote
		{
			bool				bReconnect;			///< 원격 재접속 여부
			std::string			sHost;				///< 원격 주소
			std::string			sPort;				///< 원격 포트
			int32				iThreadCount;		///< 쓰레드 갯수
			long				iCreatedSockets;	///< 접속 대기 NetLink의 개수
			size_t				iMaxBufSize;		///< 수신버퍼의 크기
			Remote()
			: sHost("localhost")
			, sPort("63344")
			, bReconnect(false)
			, iThreadCount(2)
			, iCreatedSockets(100)
			, iMaxBufSize(4096)
			{
			}
		};
		/** 
			데이타베이스 연결 설정 구조체
		*/
		struct Database
		{
			std::string			sHost;				///< 데이타베이스 주소
			std::string			sPort;				///< 데이타베이스 포트
			std::string			sCategory;			///< 데이타베이스 카테고리
			std::string			sUid;				///< 데이타베이스 계정
			std::string			sPassword;			///< 데이타베이스 비밀번호
			bool				bAutoReconnect;		///< 데이타베이스 재연결 여부
			std::string			sCharset;			///< 중국 gbk, 일본 ujis, 한국 euckr
			int32				iConnectionCount;	///< 데이타베이스 연결 개수
			Database()
				: sHost("localhost")
				, sPort("3306")
				, sCategory("gamedb")
				, sUid("root")
				, sPassword("rkddkwl")
				, bAutoReconnect(true)
				, sCharset("euckr")
				, iConnectionCount(4)
			{
			}
		};

		std::string sProviderName;			///< 데이타베이스 프로바이더 이름
		Remote		stRemote;				///< 원격 연결 설정
		Database	stDatabase;				///< 데이타베이스 연결 설정
		uint32		iTaskDuration;			///< SQL 수행 결과 처리 간격(msec)
		int32		iCompressOverSize;		///< 패킷 압축 여부
		uint32		iNotifyTaskSchedulerID;		///< SQL 수행 결과를 처리하는 태스크스케줄러의 고유 ID
		Properties()
			: sProviderName(null_string)
			, iTaskDuration(1000)
			, iCompressOverSize(0)
			, iNotifyTaskSchedulerID(1)
		{}
	};

	IDBProvider() {}
	virtual ~IDBProvider() {}
	/** 
		데이타베이스에 연결하고 SQL을 수행할 준비를 한다.
		성공시 1 리턴, 실패시 0 이외의 값 리턴
		@param Prop 데이타베이스 설정값
	*/
	virtual int32	Startup( const IDBProvider::Properties& Prop ) = 0;
	/** 
		데이타베이스와의 연결을 종료한다.
	*/
	virtual void	Shutdown() = 0;
	/** 
		실행할 SQL의 갯수를 조회한다.
	*/
	virtual size_t	Size() = 0;
	/** 
		SQL 실행을 요청한다.
		@param pQry 실행할 쿼리 오브젝트
	*/
	virtual int32	Push( Query* pQry ) = 0;
	/** 
		SQL 실행을 요청한다.
		@param QryList 실행할 퀴리 오브젝트가 저장된 리스트
	*/
	virtual int32	Push( Query::QType& QryList ) = 0;
	/** 
		SQL 실행을 요청한다.
		@param pQry 실행할 쿼리 오브젝트
		@param iMSec 대기시간(msec)
	*/
	virtual int32	Push( Query* pQry, uint32 iMSec ) = 0;
	/** 
		실행 완료된 SQL 오브젝트를 꺼내온다.
		@param iQid 쿼리오브젝트의 고유ID
		@param iMSec 대기시간(msec)
	*/
	virtual Query*  Pop( QUID iQid, uint32 iMsec ) = 0;
	/** 
		실행 완료된 SQL 오브젝트를 꺼내온다.
		@param QryList 쿼리오브젝트를 저장할 리스트
	*/
	virtual bool    Pop( Query::QType& QryList ) = 0;
	/** 
		처리완료된 쿼리 오브젝트를 실행한다.
	*/
	virtual void    EventTick( void ) = 0;
};
