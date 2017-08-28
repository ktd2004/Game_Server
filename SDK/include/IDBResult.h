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

#include <DBValue.h>

/** 
	SQL ������ �÷��� ���� �����ϴ� Ŭ����
*/
struct IDBField
{
	/** 
		�÷��� �̸��� ��ȸ
	*/
	virtual const std::string&			GetFieldName( void ) = 0;
	/** 
		�÷��� �ڷ���
	*/
	virtual const DBValue::ValueType	GetValueType( void ) = 0;
};

/** 
	SQL ������ �൥��Ÿ�� �����ϰ� �ִ� Ŭ����
*/
struct IDBRow
{
	/** 
		SQL ������ �൥��Ÿ�� �����ϰ� �ִ� �����̳ʸ� �����Ѵ�.
	*/
	virtual StaticArray<DBValue>&	GetStaticArray( void ) = 0;
	/** 
		SQL ������ ��ȸ�� �÷��� ������ ��ȸ�Ѵ�.
	*/
	virtual int32					GetColumnCount( void ) = 0;
	/** 
		SQL ������ ��ȸ�� �൥��Ÿ�� �÷������� ���´�.
	*/
	virtual DBValue&				FetchCol( int32 i ) = 0;
};

/** 
	SQL ������ ��ȸ�� �������� �൥��Ÿ�� �����ϰ� �ִ� ���ڵ�� Ŭ����
*/
struct IDBRecordset
{
	/** 
		SQL �������� ������ ���� ���� ������ �����Ѵ�.
		@param iAffectedRows SQL �������� ������ ���� ���� ����
	*/
	virtual void		OnAffectedRows( int64 iAffectedRows ) = 0;
	/** 
		SQL �������� ��ȸ�� �÷������� �����Ѵ�.
		@param Vt �÷��� �ڷ���
		@param sName �÷��� �̸�
	*/
	virtual void		OnField( DBValue::ValueType Vt, const std::string& sName ) = 0;
	/** 
		SQL �������� ��ȸ�� �൥��Ÿ�� ������ ������ �����Ѵ�.
	*/
	virtual void		OnRow( void ) = 0;
	/** 
		SQL �������� ��ȸ�� �÷�����Ÿ�� ������ ������ �����Ѵ�.
	*/
	virtual void		OnCol( const DBValue& v ) = 0;
	/** 
		SQL �������� ��ȸ�� �÷������� ������ ��ȸ�Ѵ�.
	*/
	virtual int32		GetFieldCount( void ) = 0;
	/** 
		SQL �������� ������ ���� ���� ������ ��ȸ�Ѵ�.
	*/
	virtual int64		GetAffectedRows( void ) = 0;
	/** 
		SQL �������� ����� ��� ����Ÿ�� ũ�⸦ ��ȸ�Ѵ�.
	*/
	virtual int32		GetEastimateBytes( void ) = 0;
	/** 
		SQL �������� ��ȸ�� �÷�����Ÿ�� ������ ������ �����Ѵ�.
	*/
	virtual IDBRow*		FetchRow( void ) = 0;
	/** 
		���� ���� ���� ���ȣ�� ���� ó������ �ǵ�����.
	*/
	virtual void		Rewind( void ) = 0;
	/** 
		SQL �������� ��ȸ�� �÷������� �����Ѵ�.
		@param iInx ������ �÷������� ��ġ
	*/
	virtual IDBField*	GetField( int32 iInx ) = 0; 
};

/** 
	SQL ������ ��ȸ�� �������� ���ڵ�¸� �����ϰ� �ִ� Ŭ����
*/
class IDBResult
{
public:
	IDBResult() {}
	virtual ~IDBResult() {}
	/** 
		SQL �������� ��ȸ�� ����Ÿ�� ������ ���ڵ�� ������ �����Ѵ�.
	*/
	virtual void			OnRecordset() = 0;
	/** 
		SQL �������� ������ ���� ���� ������ �����Ѵ�.
		@param iAffectedRows SQL �������� ������ ���� ���� ����
	*/
	virtual void			OnAffectedRows(int64 iAffectedRows) = 0;
	/** 
		SQL �������� ��ȸ�� �÷������� �����Ѵ�.
		@param Vt �÷��� �ڷ���
		@param sName �÷��� �̸�
	*/
	virtual void			OnField(DBValue::ValueType Vt, const std::string& sName) = 0;
	/** 
		SQL �������� ��ȸ�� �൥��Ÿ�� ������ ������ �����Ѵ�.
	*/
	virtual void			OnRow() = 0;
	/** 
		SQL �������� ��ȸ�� �÷�����Ÿ�� ������ ������ �����Ѵ�.
	*/
	virtual void			OnCol(const DBValue& v) = 0;
	/** 
		���ڵ���� ������ ��ȸ�Ѵ�.
	*/
	virtual int32			GetRecordsetCount() = 0;
	/** 
		���� ���� ���� ���ڵ���� ���� ó������ �ǵ�����.
	*/
	virtual void			Rewind() = 0;
	/** 
		SQL ���� ����� ������ ���� ��� ���� ������ ��ȸ�Ѵ�.
	*/
	virtual int64			GetAffectedRows( void ) = 0;
	/** 
		SQL �������� ����� ��� ����Ÿ�� ũ�⸦ ��ȸ�Ѵ�.
	*/
	virtual int32			GetEastimateBytes() = 0;
	/** 
		���ڵ���� �����´�.
	*/
	virtual IDBRecordset*	FetchRecordset() = 0;
};