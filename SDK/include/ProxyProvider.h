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
	RemoteProvider���� ��Ʈ������ ��û�� ������ �����ϰ� ����� ����
*/
class ProxyProvider
	: public IDBProvider
{
	spRef<IDBProvider>	m_spDBProvider;			///< ����Ÿ���̽� ����
	NetEventHandler		m_Net;					///< ��Ʈ�� �ڵ鷯
public:
	ProxyProvider();
	virtual ~ProxyProvider();
	/** 
		����Ÿ���̽��� �����ϰ� SQL�� ������ �غ� �Ѵ�.
		������ 1 ����, ���н� 0 �̿��� �� ����
		@param Prop ����Ÿ���̽� ������
	*/
	int32	Startup( const IDBProvider::Properties& Prop ) override;
	/** 
		����Ÿ���̽����� ������ �����Ѵ�.
	*/
	void	Shutdown( void ) override;
	/** 
		������ SQL�� ������ ��ȸ�Ѵ�.
	*/
	size_t	Size( void ) override;
	/** 
		SQL ������ ��û�Ѵ�.
		@param pQry ������ ���� ������Ʈ
	*/
	int32	Push( Query* pQry ) override;
	/** 
		SQL ������ ��û�Ѵ�.
		@param QryList ������ ���� ������Ʈ�� ����� ����Ʈ
	*/
	int32	Push( Query::QType& QryList ) override;
	/**
		�ɹ��Լ��� ��� ������ ����.
	*/
	int32	Push( Query* pQry, uint32 iMSec ) override;
	/**
		�ɹ��Լ��� ��� ������ ����.
	*/
	Query*	Pop( QUID iQid, uint32 iMSec ) override;
	/**
		�ɹ��Լ��� ��� ������ ����.
	*/
	bool	Pop( Query::QType& QryList ) override;
	/**
		�ɹ��Լ��� ��� ������ ����.
	*/
	void	Wait( int32 iMSec );

private:
	void	EventTick( void );
	bool	OnReceived( NetLinkPtr spLink, Buffer* pBuf );
	Query*	QueryDeserialize( NetLinkPtr spLink, Buffer* pBuf );
};
