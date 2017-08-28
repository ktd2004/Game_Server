#include "stdafx.h"

//
#include <Query.h>
#include <DBValue.h>
#include <QueryResultHelper.h>
#include <IDBCommand.h>
#include <IDBFactory.h>
#include <DBProvider.h>
#include <ProxyProvider.h>
#include <Protocol_Provider.h>

#include <Packet.h>
#include <TaskScheduler.h>

class ProxyQuery : public Query
{
public:
	std::vector<DBValue> m_Params;
	uint64			m_iOid;
	NetLinkPtr		m_spLink;
	int8			m_iCommandType;
	int32			m_iCompressOverSize;

public:
	ProxyQuery(QUID iQid, int8 iCommandType)
		: Query(iQid)
		, m_iOid(0)
		, m_iCommandType(iCommandType)
	{}

	virtual ~ProxyQuery()
	{
	}

	void MakeCommand( IDBCommand* pCmd )
	{
		pCmd->SetCommand( (IDBCommand::DBCommandType)m_iCommandType, m_Sql );
		pCmd->AddParam(m_Params);
	}

	void OnQuery( IDBResult* spResult )
	{
		int32 iRecordsetCount = spResult->GetRecordsetCount();

//		prn_sys( "EXEC %s, Recordset %d", cstr(m_Sql), iRecordsetCount );

		QueryResultHeader RHeader;
		if ( iRecordsetCount == 0 )
		{
			Packet pk(new Buffer(1024));
			// 쿼리 오브젝트 식별자
			pk << m_iOid;
			/// 헤더 설정
			pk << RHeader;
			pk.id( 0 );
			m_spLink->Send(pk.get());
			return;
		}

		int32 iBytes = 0;
		//int32 iTest = 0;
		// 512 의 배수가 되도록 패킷 사이즈를 구한다.
		ResizeBytes(
			sizeof(QueryResultHeader) * iRecordsetCount + spResult->GetEastimateBytes() + 1024,
			iBytes);
		// iBytes = 1024 * 20;
		Buffer* pBuf = new Buffer( iBytes );
		Buffer& b = *pBuf;
		Packet pk(pBuf);

		// 쿼리 오브젝트 식별자
		pk << m_iOid;

		for ( int32 r = 0; r < iRecordsetCount; r++ )
		{
			IDBRecordset* rst = spResult->FetchRecordset();
			if ( rst )
			{
//				prn_sys( "%s recordset %d", cstr(m_Sql), r );
				/// 헤더 설정
				RHeader.iRecordsetCount = (int16)spResult->GetRecordsetCount();
				RHeader.iAffectedRows = (int16)rst->GetAffectedRows();
				RHeader.iFieldCount = (int16)rst->GetFieldCount();
				RHeader.iRecordsetInx = (int8)(r + 1);
				b << RHeader;

				// 필드 정보
				for ( int16 i = 0; i < RHeader.iFieldCount; i++ )
				{
					b << rst->GetField(i)->GetFieldName();
					b << (int16)rst->GetField(i)->GetValueType();
				}

				// 레코드 정보
				IDBRow* row = nullptr;
				while ( (row = rst->FetchRow()) )
				{
					for ( int16 c = 0; c < row->GetColumnCount(); c++ )
					{
						DBValue& v = row->FetchCol(c);
						b << (int32)v.Size();
						b << (int16)v.m_ValueType;
						b.copy( (const char*)v.Get(), v.Size() );
//						prn_sys( "%s val[%s]%d", cstr(m_Sql), v.Get(), v.Size() );
					}
				}
			}
		}

		pk.id( 0 );
// 		if ( pk.get()->length() > (size_t)iTest )
// 		{
// 			prn_sys( "estimate = %d, test %d real %d", spResult->GetEastimateBytes(), iTest, pk.get()->length() );
// 		}
		m_spLink->Send(pk.get());
	}

	void OnError( int32 iErrorCode, const std::string& sErrorMsg )
	{
		int32 iBytes = 0;
		// 512 의 배수가 되도록 패킷 사이즈를 구한다.
		ResizeBytes( 1024 + sErrorMsg.size(), iBytes );
		//iBytes = 1024 * 10;
		Buffer* Buf = new Buffer( iBytes );
		Buffer& b = *Buf;
		Packet pk(Buf);

		// 쿼리 오브젝트 식별자
		b << m_iOid;

		/// 헤더 설정
		QueryResultHeader RHeader;
		RHeader.iErr = iErrorCode;

		b << RHeader;
		b << sErrorMsg;

		pk.id( 0 );
		m_spLink->Send(pk.get());
	}

	void OnResult()
	{
	}
};

//////////////////////////////////////////////////////////////////////////

ProxyProvider::ProxyProvider()
: m_Net(1)
{
	m_Net.SetReceivedFunc( this, &ProxyProvider::OnReceived );
}

ProxyProvider::~ProxyProvider()
{
}

int32 ProxyProvider::Startup( const IDBProvider::Properties& Prop )
{
	m_spDBProvider = IDBProviderFactory::CreateDirect();
	if ( !m_spDBProvider->Startup(Prop) )
	{
		goto label_error;
	}

	m_Net.NotifyTask( Prop.iNotifyTaskSchedulerID );
	if ( !m_Net.Start(0, Prop.stRemote.iThreadCount) )
	{
		goto label_error;
	}

	if ( !m_Net.Listen(stouint32(Prop.stRemote.sPort), Prop.stRemote.sHost, Prop.stRemote.iCreatedSockets, Prop.stRemote.iMaxBufSize) )
	{
		goto label_error;
	}

	return 1;

label_error:
	Shutdown();
	return 0;
}

void ProxyProvider::Shutdown( void )
{
	while ( 1 )
	{
		if ( m_spDBProvider->Size() == 0 )
		{
			m_Net.Stop();
			m_spDBProvider->Shutdown();
			break;
		}
		Sleep( 500 );
	}
}

size_t ProxyProvider::Size( void )
{
	return m_spDBProvider->Size();
}

int32 ProxyProvider::Push( Query* pQry )
{
	return m_spDBProvider->Push( pQry );
}

int32 ProxyProvider::Push( Query::QType& QryList )
{
	return m_spDBProvider->Push( QryList );
}

int32 ProxyProvider::Push( Query* pQry, uint32 iMSec )
{
	SetLastError( DBPROVIDER_METHOD_IS_NOT_SUPPORTED );
	return -1;
}

Query* ProxyProvider::Pop( QUID iQid, uint32 iMSec )
{
	SetLastError( DBPROVIDER_METHOD_IS_NOT_SUPPORTED );
	return NULL;
}

bool ProxyProvider::Pop( Query::QType& QryList )
{
	SetLastError( DBPROVIDER_METHOD_IS_NOT_SUPPORTED );
	return false;
}

void ProxyProvider::Wait( int32 iMSec )
{
	SetLastError( DBPROVIDER_METHOD_IS_NOT_SUPPORTED );
}

//////////////////////////////////////////////////////////////////////////

bool ProxyProvider::OnReceived( NetLinkPtr spLink, Buffer* pBuf )
{
	Buffer::QType	lst;
	if ( m_Net.Parse(spLink, pBuf, lst) )
	{
		Query::QType QryList;
		while ( !lst.empty() )
		{
			Buffer* p = lst.front(); lst.pop_front();
			Query* pQry = QueryDeserialize( spLink, p );
			if ( pQry )
			{ QryList.push_back(pQry); }
			p->release(); p = nullptr;
		}
		m_spDBProvider->Push( QryList );

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

Query* ProxyProvider::QueryDeserialize( NetLinkPtr spLink, Buffer* pBuf )
{
	ExecuteQueryHeader QHeader;
	std::string SQL;
	pBuf->rd_ptr( sizeof(Packet::header) );	// 헤더 제거하고.
	*pBuf >> QHeader;
	*pBuf >> SQL;

	ProxyQuery *Qry = new ProxyQuery(QHeader.iQid, QHeader.iCommandType);
	Qry->m_spLink = spLink;
	Qry->m_iOid   = QHeader.iOid;
	Qry->SetStmt(SQL);

	if ( QHeader.iParamCount > 0 )
	{ Qry->m_Params.resize(QHeader.iParamCount); }

	// Param Bind
	for (int16 i = 0; i < QHeader.iParamCount; i++ )
	{
		QueryParamsHeader PHeader;
		*pBuf >> PHeader;
		switch( PHeader.iValueType )
		{
		case DBValue::vtBinary :
		case DBValue::vtString :
			Qry->m_Params[i].Set( pBuf->rd_ptr(), PHeader.iSize );
			Qry->m_Params[i].m_ValueType = (DBValue::ValueType)PHeader.iValueType;
			Qry->m_Params[i].m_Size = PHeader.iSize;
			pBuf->rd_ptr(PHeader.iSize);
			break;
		default :
			Qry->m_Params[i].m_ValueType = (DBValue::ValueType)PHeader.iValueType;
			Qry->m_Params[i].m_Size = PHeader.iSize;
			memcpy( Qry->m_Params[i].Get(), pBuf->rd_ptr(), PHeader.iSize );
			pBuf->rd_ptr(PHeader.iSize);
			break;
		}
	}

	return Qry;
}

void ProxyProvider::EventTick( void )
{
}

//////////////////////////////////////////////////////////////////////////

IDBProvider* IDBProviderFactory::CreateProxy( void )
{
	return new ProxyProvider;
}

