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
#include <Buffer.h>

//////////////////////////////////////////////////////////////////////////
// ��Ŷ ����

#pragma pack(push)
#pragma pack(1)
struct ExecuteQueryHeader
{
	uint64 iOid;			// ����������Ʈ �ĺ���
	QUID   iQid;			// ����ť �ĺ���
	int8   iCommandType;	// ���� ����
	int16  iParamCount;		// �ĸ����� ����
	ExecuteQueryHeader()
		: iOid(0)
		, iQid(0)
		, iCommandType(0)
		, iParamCount(0)
	{}
};
#pragma pack(pop)


Buffer& operator<< (Buffer& pk, const ExecuteQueryHeader& r);
Buffer& operator>> (Buffer& pk, ExecuteQueryHeader& r);

#pragma pack(push)
#pragma pack(1)
struct QueryParamsHeader
{
	int16 iValueType;		// �ĸ����� �ڷ���
	int32 iSize;			// �Ķ���� ũ��
	QueryParamsHeader()
		: iValueType(0)
		, iSize(0)
	{}
};
#pragma pack(pop)

Buffer& operator<< (Buffer& pk, const QueryParamsHeader& r);
Buffer& operator>> (Buffer& pk, QueryParamsHeader& r);

//////////////////////////////////////////////////////////////////////////

#pragma pack(push)
#pragma pack(1)
struct QueryResultHeader
{
	int16  iRecordsetCount;		// ���ڵ�� ����
	int16  iAffectedRows;		// ��ȸ�� �� ����
	int16  iFieldCount;			// �÷� ����
	int8   iRecordsetInx;		// ���ڵ�� �ĺ���( 1 ���� )
	int32  iErr;				// ���� �ڵ�
	QueryResultHeader()
		: iRecordsetCount(0)
		, iAffectedRows(0)
		, iFieldCount(0)
		, iRecordsetInx(0)
		, iErr(0)
	{}
};
#pragma pack(pop)

Buffer& operator<< (Buffer& pk, const QueryResultHeader& r);
Buffer& operator>> (Buffer& pk, QueryResultHeader& r);
