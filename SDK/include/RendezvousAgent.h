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

#include <P2PAgent.h>

/** 
	������/��Ŷ������ ������ �����ϴ� �����Լ��� �����ϴ� ���ӽ����̽�.
*/
struct RendezvousAgentFactory
{
	/**
		������/��Ŷ ������ ������ �����Ѵ�.
		@param rSelf �ڽ��� ��Ʈ�� ����
		@param iMaxBufferSize �ۼ�����Ŷ�� �ִ� ũ��
		@param iKeepConnection �������� ��Ŷ �ۼ��� ����(ms)
	*/
	static P2PAgent*	Create( const Network_IF& rSelf, unsigned int iMaxBufferSize = 4096, unsigned long iKeepConnection = 1000 );
	/**
		������/��Ŷ ������ ������ �����Ѵ�.
		@param rSelf �ڽ��� ��Ʈ�� ����
		@param pHandler ��Ʈ�� �̺�Ʈ�� ó���� �ڵ鷯 ��ü�� �ּҰ�
		@param rRendezvous ������,��Ŷ�����̼����� ��Ʈ�� ����
		@param iMaxBufferSize �ۼ�����Ŷ�� �ִ� ũ��
		@param iKeepConnection �������� ��Ŷ �ۼ��� ����(ms)
	*/
	static P2PAgent*	Join( const Network_IF& rSelf, P2PAgentHandler* pHandler, const Network_IF& rRendezvous,
							unsigned int iMaxBufferSize = 4096, unsigned long iKeepConnection = 1000 );
};