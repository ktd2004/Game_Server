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

// dbengine
#include <DataTypes.h>
#include <DBValue.h>
#include <IDBResult.h>


/////////////////////////////////////////////////////////////////////////////////
PTR(DBField);
/** 
	SQL ������ �÷��� ���� �����ϴ� Ŭ����
*/
class DBField
	: public single_ref
	, public IDBField
{
	std::string	m_sName;		///< �÷� �̸�
	DBValue::ValueType	m_Vt;	///< �÷��� �ڷ���
public:
	DBField(DBValue::ValueType vt, const std::string& sName);
	virtual ~DBField();
	/** 
		�÷��� �̸��� ��ȸ
	*/
	const std::string& GetFieldName( void ) override;
	/** 
		�÷��� �ڷ���
	*/
	const DBValue::ValueType GetValueType() override;
};

/////////////////////////////////////////////////////////////////////////////////

PTR(DBRow);
/** 
	SQL ������ �൥��Ÿ�� �����ϰ� �ִ� Ŭ����
*/
class DBRow
	: public single_ref
	, public IDBRow
{
	friend class DBRecordset;
	StaticArray<DBValue> m_Cols;		///< SQL ������ �÷�����Ÿ�� �����ϴ� �����̳�
public:
	DBRow(size_t iSize);
	virtual ~DBRow();
	/** 
		SQL ������ �൥��Ÿ�� �����ϰ� �ִ� �����̳ʸ� �����Ѵ�.
	*/
	StaticArray<DBValue>&	GetStaticArray( void ) override;
	/** 
		SQL ������ ��ȸ�� �÷��� ������ ��ȸ�Ѵ�.
	*/
	int32					GetColumnCount( void ) override;
	/** 
		SQL ������ ��ȸ�� �൥��Ÿ�� �÷������� ���´�.
	*/
	DBValue&				FetchCol(int32 i) override;
};

/////////////////////////////////////////////////////////////////////////////////

PTR(DBRecordset)
/** 
	SQL ������ ��ȸ�� �������� �൥��Ÿ�� �����ϰ� �ִ� ���ڵ�� Ŭ����
*/
class DBRecordset
	: public single_ref
	, public IDBRecordset
{
	int64					m_iAffectedRows;	///< SQL ���࿡ ������ ���� ���� ����
	std::vector<DBFieldPtr> m_vFields;			///< SQL �������� ��ȸ�� �÷�����
	std::vector<DBRowPtr>	m_vRow;				///< SQL �������� ��ȸ�� �൥��Ÿ ������ �����̳�
	int32					m_iCol;				///< SQL �������� ��ȸ�� �÷�����Ÿ�� ������ �÷���ȣ
	int32					m_iRow;				///< SQL �������� ��ȸ�� �÷�����Ÿ�� ������ ���ȣ
	int32					m_iFetchRow;		///< ���� �������� ���ȣ
	int32					m_iEastimateBytes;	///< SQL �������� ����� ��� ����Ÿ�� ũ��
public:
	DBRecordset();
	virtual ~DBRecordset();
	/** 
		SQL �������� ������ ���� ���� ������ �����Ѵ�.
		@param iAffectedRows SQL �������� ������ ���� ���� ����
	*/
	void			OnAffectedRows( int64 iAffectedRows ) override;
	/** 
		SQL �������� ��ȸ�� �÷������� �����Ѵ�.
		@param Vt �÷��� �ڷ���
		@param sName �÷��� �̸�
	*/
	void			OnField( DBValue::ValueType Vt, const std::string& sName ) override;
	/** 
		SQL �������� ��ȸ�� �൥��Ÿ�� ������ ������ �����Ѵ�.
	*/
	void			OnRow( void ) override;
	/** 
		SQL �������� ��ȸ�� �÷�����Ÿ�� ������ ������ �����Ѵ�.
	*/
	void			OnCol( const DBValue& v ) override;
	/** 
		SQL �������� ��ȸ�� �÷�����Ÿ�� ������ ������ �����Ѵ�.
	*/
	IDBRow*			FetchRow( void ) override;
	/** 
		SQL �������� ��ȸ�� �÷������� �����Ѵ�.
		@param iInx ������ �÷������� ��ġ
	*/
	IDBField*		GetField( int32 iInx ) override;
	/** 
		SQL �������� ��ȸ�� �÷������� ������ ��ȸ�Ѵ�.
	*/
	int32			GetFieldCount( void ) override;
	/** 
		���� ���� ���� ���ȣ�� ���� ó������ �ǵ�����.
	*/
	void			Rewind( void ) override;
	/** 
		SQL �������� ������ ���� ���� ������ ��ȸ�Ѵ�.
	*/
	int64			GetAffectedRows( void ) override;
	/** 
		SQL �������� ����� ��� ����Ÿ�� ũ�⸦ ��ȸ�Ѵ�.
	*/
	int32			GetEastimateBytes( void ) override;
};

/////////////////////////////////////////////////////////////////////////////////
PTR(DBResult);
/** 
	SQL ������ ��ȸ�� �������� ���ڵ�¸� �����ϰ� �ִ� Ŭ����
*/
class DBResult
	: public single_ref
	, public IDBResult
{
	std::vector<DBRecordsetPtr> m_vRecordset;			///< SQL ������ ����� �����ϰ� �ִ� ���ڵ�� ��������
	int32						m_iRecordset;			///< SQL �������� ��ȸ�� ���ڵ�¸� ������ ��ȣ
	int32						m_iFetchRecordset;		///< ���� ���� ���� ���ڵ�� ��ȣ
public:
	DBResult();
	virtual ~DBResult();
	/** 
		SQL �������� ��ȸ�� ����Ÿ�� ������ ���ڵ�� ������ �����Ѵ�.
	*/
	void			OnRecordset( void ) override;
	/** 
		SQL �������� ������ ���� ���� ������ �����Ѵ�.
		@param iAffectedRows SQL �������� ������ ���� ���� ����
	*/
	void			OnAffectedRows(int64 iAffectedRows) override;
	/** 
		SQL �������� ��ȸ�� �÷������� �����Ѵ�.
		@param Vt �÷��� �ڷ���
		@param sName �÷��� �̸�
	*/
	void			OnField(DBValue::ValueType Vt, const std::string& sName) override;
	/** 
		SQL �������� ��ȸ�� �൥��Ÿ�� ������ ������ �����Ѵ�.
	*/
	void			OnRow( void ) override;
	/** 
		SQL �������� ��ȸ�� �÷�����Ÿ�� ������ ������ �����Ѵ�.
	*/
	void			OnCol(const DBValue& v) override;
	/** 
		���ڵ���� ������ ��ȸ�Ѵ�.
	*/
	int32			GetRecordsetCount( void ) override;
	/** 
		���ڵ���� �����´�.
	*/
	IDBRecordset*	FetchRecordset( void ) override;
	/** 
		���� ���� ���� ���ڵ���� ���� ó������ �ǵ�����.
	*/
	void			Rewind( void ) override;
	/** 
		SQL ���� ����� ������ ���� ��� ���� ������ ��ȸ�Ѵ�.
	*/
	int64			GetAffectedRows( void ) override;
	/** 
		SQL �������� ����� ��� ����Ÿ�� ũ�⸦ ��ȸ�Ѵ�.
	*/
	int32			GetEastimateBytes( void ) override;
};