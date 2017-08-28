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

//<
#include <NetLink.h>

/** 
	RPC.NET�� Proactor�� NetEventHandler���� �߻��� ��Ʈ���̺�Ʈ�� ó���ϴ� �߻�Ŭ����.

	Proactor�� NetEventHandler�� ���Ͽ��� �߻��� Ŭ���̾�Ʈ ����, ���� ����, ��Ŷ �ۼ���, ����� ���� ��Ʈ���̺�Ʈ����
	ó���ϱ� ���ؼ� Listen(), Bind(), Connect() �Լ� ȣ��� ���޹��� NetCallback ��ü�� ��Ʈ���̺�Ʈ �Լ��� ȣ���Ѵ�.
*/
struct NetCallback
{
	/**
		Ŭ���̾�Ʈ�� ����Ǹ� �߻��Ǵ� ��Ʈ�� �̺�Ʈ
		@param spLink ������ Ŭ���̾�Ʈ ��ũ
	*/
	virtual void OnNetAccepted( NetLinkPtr spLink ) = 0;
	/**
		������ ����Ǹ� �߻��Ǵ� ��Ʈ�� �̺�Ʈ
		@param ������ ������ ��û�� ��ũ
	*/
	virtual void OnNetConnected( NetLinkPtr spLink ) = 0;
	/**
		��Ŷ�� �����ϸ� �߻��Ǵ� ��Ʈ�� �̺�Ʈ
		@param spLink ����Ÿ�� ������ ��ũ
		@param pBuf ���Ź��� ����Ÿ�� �����ϰ� �ִ� ����
	*/
	virtual void OnNetReceived( NetLinkPtr spLink, Buffer* pBuf ) = 0;
	/**
		������ ����Ǹ� �߻��Ǵ� ��Ʈ�� �̺�Ʈ
		@param spLink ������ ����� ��ũ
	*/
	virtual void OnNetClosed( NetLinkPtr spLink ) = 0;
};