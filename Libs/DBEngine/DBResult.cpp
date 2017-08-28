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
#include "stdafx.h"

// dbengine
#include <DBResult.h>
#include <IDBFactory.h>


/////////////////////////////////////////////////////////////////////////////////
///< 컬럼 이름
DBField::DBField( DBValue::ValueType vt, const std::string& sName )
: m_sName(sName), m_Vt(vt)
{}

DBField::~DBField()
{}

const std::string& DBField::GetFieldName( void )
{
	return m_sName;
}

const DBValue::ValueType DBField::GetValueType( void )
{
	return m_Vt;
}

/////////////////////////////////////////////////////////////////////////////////
///< 행데이타
DBRow::DBRow(size_t iSize)
: m_Cols(iSize)
{}

DBRow::~DBRow()
{}

StaticArray<DBValue>& DBRow::GetStaticArray( void )
{
	return m_Cols;
}

int32 DBRow::GetColumnCount( void )
{
	return (int32)m_Cols.size();
}

DBValue& DBRow::FetchCol( int32 i )
{
	DBValue& v = m_Cols[i];
	return v;
}

/////////////////////////////////////////////////////////////////////////////////

DBRecordset::DBRecordset()
: m_iCol(0)
, m_iRow(0)
, m_iFetchRow(0)
, m_iEastimateBytes(0)
{}

DBRecordset::~DBRecordset()
{}

void DBRecordset::OnAffectedRows( int64 iAffectedRows )
{
	m_iAffectedRows = iAffectedRows;
}

void DBRecordset::OnField( DBValue::ValueType Vt, const std::string& sName )
{
	m_iEastimateBytes += sizeof(int16);										// type
	m_iEastimateBytes += (sizeof(unsigned int) + (int32)sName.length());	// length + field name 
	m_vFields.push_back(new DBField(Vt, sName));
}

void DBRecordset::OnRow( void )
{
	m_vRow.push_back( new DBRow(m_vFields.size()) );
	m_iRow = (int32)m_vRow.size() - 1;
	m_iCol = 0;
}

void DBRecordset::OnCol( const DBValue& v )
{
	m_iEastimateBytes += sizeof(int32);		// size
	m_iEastimateBytes += sizeof(int16);		// value type
	m_iEastimateBytes += (int32)v.m_Size;	// data
	m_vRow[m_iRow]->m_Cols[m_iCol] = v;
	m_iCol++;
}

IDBRow* DBRecordset::FetchRow( void )
{
	if ( m_iFetchRow >= (int32)m_vRow.size() )
		return nullptr;
	IDBRow* p = m_vRow[m_iFetchRow];
	m_iFetchRow++;
	return p;
}

IDBField* DBRecordset::GetField( int32 iInx )
{
	if ( iInx >= (int32)m_vFields.size() )
		return nullptr;
	return m_vFields[iInx];
}

int32 DBRecordset::GetFieldCount( void )
{
	return (int32)m_vFields.size();
}

void DBRecordset::Rewind( void )
{
	m_iFetchRow = 0;
}

int64 DBRecordset::GetAffectedRows( void )
{
	return m_iAffectedRows;
}

int32 DBRecordset::GetEastimateBytes( void )
{
	return m_iEastimateBytes;
}

/////////////////////////////////////////////////////////////////////////////////
DBResult::DBResult()
: m_iRecordset(0)
, m_iFetchRecordset(0)
{}
DBResult::~DBResult()
{}

void DBResult::OnRecordset( void )
{
	m_vRecordset.push_back( new DBRecordset );
	m_iRecordset = (int32)m_vRecordset.size() - 1;
}

void DBResult::OnAffectedRows( int64 iAffectedRows )
{
	m_vRecordset[m_iRecordset]->OnAffectedRows(iAffectedRows);
}

void DBResult::OnField(DBValue::ValueType Vt, const std::string& sName)
{ 
	m_vRecordset[m_iRecordset]->OnField(Vt, sName);
}

void DBResult::OnRow( void )
{
	m_vRecordset[m_iRecordset]->OnRow();
}

void DBResult::OnCol( const DBValue& v )
{
	m_vRecordset[m_iRecordset]->OnCol(v);
}

int32 DBResult::GetRecordsetCount( void )
{
	return (int32)m_vRecordset.size();
}

IDBRecordset* DBResult::FetchRecordset( void )
{ 
	if ( m_iFetchRecordset >= (int32)m_vRecordset.size() )
		return nullptr;

	IDBRecordset* p = m_vRecordset[m_iFetchRecordset];
	m_iFetchRecordset++;
	return p;
}

void DBResult::Rewind( void )
{
	m_iFetchRecordset = 0;
}

int64 DBResult::GetAffectedRows( void )
{
	return m_vRecordset[m_iRecordset]->GetAffectedRows();
}

int32 DBResult::GetEastimateBytes( void )
{
	int32 iSum = 0;
	for ( size_t i = 0; i < m_vRecordset.size(); i++ )
	{
		iSum += (int32)m_vRecordset[i]->GetEastimateBytes();
	}
	return iSum;
}

/// 

IDBResult* IDBResultFactory::Create( void )
{
	return new DBResult;
}