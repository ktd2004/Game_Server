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
#include <map>
#include <NetLink.h>
#include <Functional.h>

/** 
	��Ʈ�� �̺�Ʈ �߻��� ȣ���� �ݹ��Լ��� Ÿ������
*/
typedef functional_v2<bool, NetLinkPtr, Buffer*>	NetFunc_V2;

/** 
	��Ʈ�� �̺�Ʈ �Լ��� �����ϴ� ��Ÿ������
*/
typedef std::map<unsigned int, NetFunc_V2* > NetEventFuncMap;

/** 
	��ŶID�� �ĺ��Ǵ� ��Ʈ���̺�Ʈ�Լ��� �����ϴ� Ŭ����

	NetEventHandler�� NetFunction ��ü�� �ɹ��� ������ ������ SetNetFunc() ���ø� �Լ���
	����Ͽ� ��Ʈ�� �̺�Ʈ�Լ��� NetFunction ��ü�� ����Ѵ�.
*/
class NetFunction
{
protected:
	NetEventFuncMap		m_NetFuncMap;	///< ��Ʈ�� �̺�Ʈ �Լ� ������

public:
	NetFunction();
	virtual ~NetFunction();

	/** 
		��Ʈ�� �̺�Ʈ �Լ��� ����Ѵ�.
		@param iProtocol ��ŶID
		@param fn ��Ʈ�� �̺�Ʈ �Լ��� �ݹ��ϴ� �Լ���
	*/
	bool		Reg( unsigned int iProtocol, NetFunc_V2* fn );
	/** 
		��Ʈ�� �̺�Ʈ �Լ��� ȣ���Ѵ�.
		@param iProtocol ��ŶID
		@param spLink ��Ŷ�� ������ ��Ʈ�� ��ũ
		@param pBuf ��Ʈ�� �Լ��� ������ ��Ŷ�� ��� �ִ� ����
	*/
	bool		Call( unsigned int iProtocol, NetLinkPtr spLink, Buffer* pBuf );
	/** 
		��Ʈ�� �̺�Ʈ �Լ��� ��ȸ�Ѵ�.
		@param iProtocol ��ŶID
	*/
	NetFunc_V2*	Find( unsigned int iProtocol );
};