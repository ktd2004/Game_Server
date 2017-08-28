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
	RPC.NET�� Proactor�� ������ ������/��Ŷ ������ ������ �����ϴ� �����Լ��� �����ϴ� ���ӽ����̽�.
*/
struct RendezvousExAgentFactory
{
	/**
		������/��Ŷ ������ ������ �����Ѵ�.
		@param rSelf �ڽ��� ��Ʈ�� ����
		@param iTaskSchedulerID �ۼ�����Ŷ�� ó���� �½�ũ�����ٷ��� ID
		@param iBindCPU �ۼ��� ��Ŷ�� ó���� �½�ũ�����ٷ��� ����� CPU ��ȣ
		@param iMaxBufferSize �ۼ�����Ŷ�� �ִ� ũ��
		@param iKeepConnection �������� ��Ŷ �ۼ��� ����(ms)
	*/
	static P2PAgent*	Create( const Network_IF& rSelf, unsigned int iTaskSchedulerID, unsigned short iBindCPU, unsigned int iMaxBufferSize = 4096, unsigned long iKeepConnection = 1000 );

	static void			Stop( void );
};