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
		����Ÿ���̽� ���� �� ���� ���� ���� ���� ����ü
	*/
	struct Properties
	{
		/** 
			���� ���� ���� ���� ����ü
		*/
		struct Remote
		{
			bool				bReconnect;			///< ���� ������ ����
			std::string			sHost;				///< ���� �ּ�
			std::string			sPort;				///< ���� ��Ʈ
			int32				iThreadCount;		///< ������ ����
			long				iCreatedSockets;	///< ���� ��� NetLink�� ����
			size_t				iMaxBufSize;		///< ���Ź����� ũ��
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
			����Ÿ���̽� ���� ���� ����ü
		*/
		struct Database
		{
			std::string			sHost;				///< ����Ÿ���̽� �ּ�
			std::string			sPort;				///< ����Ÿ���̽� ��Ʈ
			std::string			sCategory;			///< ����Ÿ���̽� ī�װ�
			std::string			sUid;				///< ����Ÿ���̽� ����
			std::string			sPassword;			///< ����Ÿ���̽� ��й�ȣ
			bool				bAutoReconnect;		///< ����Ÿ���̽� �翬�� ����
			std::string			sCharset;			///< �߱� gbk, �Ϻ� ujis, �ѱ� euckr
			int32				iConnectionCount;	///< ����Ÿ���̽� ���� ����
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

		std::string sProviderName;			///< ����Ÿ���̽� ���ι��̴� �̸�
		Remote		stRemote;				///< ���� ���� ����
		Database	stDatabase;				///< ����Ÿ���̽� ���� ����
		uint32		iTaskDuration;			///< SQL ���� ��� ó�� ����(msec)
		int32		iCompressOverSize;		///< ��Ŷ ���� ����
		uint32		iNotifyTaskSchedulerID;		///< SQL ���� ����� ó���ϴ� �½�ũ�����ٷ��� ���� ID
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
		����Ÿ���̽��� �����ϰ� SQL�� ������ �غ� �Ѵ�.
		������ 1 ����, ���н� 0 �̿��� �� ����
		@param Prop ����Ÿ���̽� ������
	*/
	virtual int32	Startup( const IDBProvider::Properties& Prop ) = 0;
	/** 
		����Ÿ���̽����� ������ �����Ѵ�.
	*/
	virtual void	Shutdown() = 0;
	/** 
		������ SQL�� ������ ��ȸ�Ѵ�.
	*/
	virtual size_t	Size() = 0;
	/** 
		SQL ������ ��û�Ѵ�.
		@param pQry ������ ���� ������Ʈ
	*/
	virtual int32	Push( Query* pQry ) = 0;
	/** 
		SQL ������ ��û�Ѵ�.
		@param QryList ������ ���� ������Ʈ�� ����� ����Ʈ
	*/
	virtual int32	Push( Query::QType& QryList ) = 0;
	/** 
		SQL ������ ��û�Ѵ�.
		@param pQry ������ ���� ������Ʈ
		@param iMSec ���ð�(msec)
	*/
	virtual int32	Push( Query* pQry, uint32 iMSec ) = 0;
	/** 
		���� �Ϸ�� SQL ������Ʈ�� �����´�.
		@param iQid ����������Ʈ�� ����ID
		@param iMSec ���ð�(msec)
	*/
	virtual Query*  Pop( QUID iQid, uint32 iMsec ) = 0;
	/** 
		���� �Ϸ�� SQL ������Ʈ�� �����´�.
		@param QryList ����������Ʈ�� ������ ����Ʈ
	*/
	virtual bool    Pop( Query::QType& QryList ) = 0;
	/** 
		ó���Ϸ�� ���� ������Ʈ�� �����Ѵ�.
	*/
	virtual void    EventTick( void ) = 0;
};
