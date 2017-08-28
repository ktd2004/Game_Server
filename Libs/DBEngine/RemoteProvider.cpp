#include "stdafx.h"

#include <DBValue.h>
#include <Packet.h>
#include <Buffer.h>
#include <Query.h>
#include <IDBFactory.h>
#include <Protocol_Provider.h>
#include <RemoteProvider.h>
#include <DBCommand.h>
#include <TaskSchedulerFactory.h>
#include <TimeUtil.h>
#include <Logger.h>
#include <Errorcodes.h>

//////////////////////////////////////////////////////////////////////////

RemoteProvider::RemoteProvider()
: m_Net(1)
, m_iTaskID(-1)
, m_pTaskScheduler(nullptr)
{
	m_Net.SetReceivedFunc( this, &RemoteProvider::OnReceived );
	m_Net.SetClosedFunc  ( this, &RemoteProvider::OnClosed );
}

RemoteProvider::~RemoteProvider()
{
}

int32 RemoteProvider::Startup( const IDBProvider::Properties& Prop )
{
	m_Net.NotifyTask( Prop.iNotifyTaskSchedulerID );
	if ( !m_Net.Start(0, 1) )
	{
		goto label_error;
	}

	m_spLink = m_Net.Connect( stouint32(Prop.stRemote.sPort), Prop.stRemote.sHost, Prop.stRemote.iMaxBufSize );
	if ( !m_spLink )
	{
		goto label_error;
	}

	m_pTaskScheduler = TaskSchedulerFactory::Get( Prop.iNotifyTaskSchedulerID );
	m_iTaskID =	CREATE_TASK( Prop.iNotifyTaskSchedulerID, this, &RemoteProvider::EventTick, Prop.iTaskDuration );

	return 1;

label_error:
	Shutdown();
	return 0;
}

// ���� ť�� �� ���� �׾ �ѹ��� ����.
void RemoteProvider::Shutdown( void )
{ 
	while( 1 )
	{
		if ( !m_Reserved.empty() )
		{
			Query::QType q;
			{
				Guard lock(m_ReqQryMutex);
				q.merge( m_Reserved );
			}
			Push( q );
		}

		// ��� SQL�� iDBGW �� �����ߴٸ� ó�� ����� ���޾Ƶ� ��� ����.
		if ( Size() == 0 )
		{
			m_Net.Stop();
			break;
		}
		Sleep( 500 );
	}
}

size_t RemoteProvider::Size( void )
{
	return m_ReqQryMap.size();
}

void RemoteProvider::AddQuery( Query* pQry )
{
	pQry->m_iOid = m_iSeq++;
#define mkval(qry,tick) std::make_pair(qry->m_iOid, ReqQryData(qry, tick))
	m_ReqQryMap.insert( mkval(pQry, util::time2usec()) );
#undef mkval
	if ( m_iSeq == 0xFFFFFFFFFFFFFFFF )
	{
		m_iSeq = 0;
	}
}

Buffer* RemoteProvider::QuerySerialize( Query* pQry )
{
	DBCommand Cmd;
	pQry->MakeCommand( &Cmd );
	std::vector<DBValue>& m_Params =  Cmd.GetParams();

	int32 iBytes = 0;
	ResizeBytes(
		( 1024 + Cmd.GetCommand().size() +
		 Cmd.GetEastimateBytes() + sizeof(QueryParamsHeader) * m_Params.size()),
		iBytes );
	Buffer* mb = new Buffer(iBytes);
	Packet pk(mb);
	pk.id(0);

	ExecuteQueryHeader	QHeader;
	QHeader.iOid = pQry->m_iOid;
	QHeader.iQid = pQry->GetQid();
	QHeader.iCommandType = (int8)Cmd.GetCommandType();
	QHeader.iParamCount  = (int16)m_Params.size();
	*mb << QHeader;
	*mb << Cmd.GetCommand();

	for ( int16 i = 0; i < QHeader.iParamCount; i++ )
	{
		QueryParamsHeader PHeader;
		PHeader.iValueType = (int16)m_Params[i].m_ValueType;
		PHeader.iSize      = (int16)m_Params[i].m_Size;
		*mb << PHeader;
		mb->copy( (const char*)m_Params[i].Get(), m_Params[i].Size() );
	}
	return pk.get();
}

int32 RemoteProvider::Push( Query* pQry )
{
	{
		Guard lock(m_ReqQryMutex);
		if ( !m_spLink )
		{ 
			m_Reserved.push_back( pQry );
			return 1;
		}
		else
		{
			AddQuery( pQry );	
		}
	}

	Buffer* mb = QuerySerialize( pQry );
	SendQuery( m_spLink, mb );
	return 1;
}

int32 RemoteProvider::Push( Query::QType& QryList )
{
	Query::QType::iterator it;
	{
		Guard lock( m_ReqQryMutex );
		if ( !m_spLink )
		{ 
			m_Reserved.merge( QryList );
			return 1;
		}
		else
		{
			for ( it = QryList.begin(); it != QryList.end(); ++it )
			{
				AddQuery( *it );
			}
		}
	}

	for ( it = QryList.begin(); it != QryList.end(); ++it )
	{ 
		Buffer* mb = QuerySerialize( *it );
		SendQuery( m_spLink, mb );
	}

	QryList.clear();
	return 1;
}

int32 RemoteProvider::Push( Query* pQry, uint32 iMSec )
{
	pQry->Sync();
	if ( Push(pQry) <= 0 )
	{
		safe_delete(pQry);
		return -1;
	}

	// �̺�Ʈ�� �ö����� ��ٸ���.
	if ( iMSec == 0 )
	{
		m_SyncEvt.Wait();
	}
	else
	{
		m_SyncEvt.Wait( iMSec );
	}

	return 1;
}

Query* RemoteProvider::Pop(QUID iQid, uint32 usec)
{
	SetLastError( DBPROVIDER_METHOD_IS_NOT_SUPPORTED );
	return nullptr;
}

bool RemoteProvider::Pop(Query::QType& QryList)
{
	SetLastError( DBPROVIDER_METHOD_IS_NOT_SUPPORTED );
	return false;
}

void RemoteProvider::Wait(int32 usec)
{
	SetLastError( DBPROVIDER_METHOD_IS_NOT_SUPPORTED );
}


//////////////////////////////////////////////////////////////////////////

bool RemoteProvider::OnConnected( NetLinkPtr spLink )
{
	m_spLink = spLink;
	if ( !Push( m_Reserved ) )
	{ return false; }

	return true;
}

bool RemoteProvider::OnClosed( NetLinkPtr spLink )
{
	m_spLink = nullptr;
	return true;
}

bool RemoteProvider::OnReceived( NetLinkPtr spLink, Buffer* pBuf )
{
	Buffer::QType lst;
	if ( m_Net.Parse(spLink, pBuf, lst) )
	{
		while ( !lst.empty() )
		{
			Buffer* p = lst.front(); lst.pop_front();
			Query* pQry = QueryDeserialize( p );
			if ( pQry )
			{ 
				Guard lock(m_ResultMutex);
				m_Result.push_back( pQry );
				if ( pQry->IsSync() )
				{ 
//					prn_sys( "Sync Query, signal" );
					m_SyncEvt.Wakeup();
				}
			}
			p->release(); p = nullptr;
		}
		if ( m_pTaskScheduler )
			m_pTaskScheduler->Signal( m_iTaskID );
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
Query* RemoteProvider::QueryDeserialize( Buffer* pBuf )
{
	// ��� �����ϰ�
	pBuf->rd_ptr( sizeof(Packet::header) );

	uint64      iOid = 0;
	Query*      pQry = nullptr;
	IDBResult*  pResult = nullptr;
	int32       iSize = 0;
	uint64      iTick = util::time2usec();
	// ���� ������Ʈ �ĺ���
	*pBuf >> iOid;
	
	ReqQryMap::iterator it;
	{
		Guard lock(m_ReqQryMutex);
		it = m_ReqQryMap.find( iOid );
		if ( it == m_ReqQryMap.end() )
		{	// ���� ��û �ؽ��ʿ� ��Ͼȵ� �����̴�
			// ���� �̻��� ����Ÿ�� �����Ѵ�.
			prn_err( "received query result is invalid oid %I64d", iOid );
			return nullptr;
		}
		pQry = it->second.pQry;
		iTick = it->second.iReqTick - iTick;
		m_ReqQryMap.erase(it);
	}

	while ( (iSize = (int32)pBuf->length()) > 0 )
	{
		// ����� ������
		QueryResultHeader RHeader;
		*pBuf >> RHeader;

		// ���� �ڵ带 ���� �Ʒ� ������ �������� �����Ѵ�.
		if ( RHeader.iErr != 0 )
		{
			std::string sErrMsg;
			*pBuf >> sErrMsg;
			pQry->OnError( RHeader.iErr, sErrMsg );
			return pQry;
		}

		// Recordset �����ϰ�
		if (nullptr == pResult)
		{ pResult = IDBResultFactory::Create(); }
		pResult->OnRecordset();

		// Field �����ϰ�
		for ( int16 f = 0; f < RHeader.iFieldCount; f++ )
		{
			std::string sFieldName;
			int16 iValueType = 0;
			*pBuf >> sFieldName >> iValueType;
			pResult->OnField( (DBValue::ValueType)iValueType, sFieldName );
		}

		// Row �����ϰ�
		for ( int16 r = 0; r < RHeader.iAffectedRows; r++ )
		{
			pResult->OnRow();
			// Col �����ϰ�
			for ( int16 c = 0; c < RHeader.iFieldCount; c++ )
			{
				DBValue v;
				int16 iValueType = 0;
				int32 iSize = 0;
				*pBuf >> iSize >> iValueType;

				switch( iValueType )
				{
				case DBValue::vtBinary :
				case DBValue::vtString :
					v.Set( pBuf->rd_ptr(), iSize );
					v.m_ValueType = (DBValue::ValueType)iValueType;
					v.m_Size = iSize;
					pBuf->rd_ptr(iSize);
					break;
				default :
					v.m_ValueType = (DBValue::ValueType)iValueType;
					v.m_Size = iSize;
					memcpy( v.Get(), pBuf->rd_ptr(), iSize );
					pBuf->rd_ptr(iSize);
					break;
				}

				if ( 0 != iSize )
				{
					pResult->OnCol(v);
				}
				else
				{
					pResult->OnCol(null_string);
				}
			}
		}

		pResult->OnAffectedRows( RHeader.iAffectedRows );

		// ��� ���ڵ���� �� �����ϸ� �������Ʈ�� �̵���Ų��.
		if ( RHeader.iRecordsetInx == RHeader.iRecordsetCount )
		{
			pQry->m_spResult = pResult;
			pQry->OnQuery( pResult );
			return pQry;
		}
	}

	return pQry;
}

void RemoteProvider::EventTick( void )
{
	Query::QType q;
	if ( !m_Result.empty() )
	{
		Guard lock(m_ResultMutex);
		q.merge( m_Result );

	}
	while( !q.empty() )
	{
		spRef<Query> spQry(q.front());
		spQry->OnResult();
		q.pop_front();
	}
}

// m_spLink->Send() �Լ��� ���� ������� �ʴ� ������ OnClosed ���� m_spLink = nullptr 
// �ϴ� ������ SendPacket �� ȣ���ص� reference count �� �÷� m_spLink �� �������� �ʵ���
// ��ȣ�ϱ� ���ؼ� �̴�.
bool RemoteProvider::SendQuery( NetLinkPtr spLink, Buffer* pBuf )
{ 
	if ( spLink )
	{
		return spLink->Send( pBuf );
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

IDBProvider* IDBProviderFactory::CreateRemote( void )
{
	return new RemoteProvider;
}