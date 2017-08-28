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

/** 
	Proactor �� ���� ������ �����ϴ� ����ü.

	��Ŷ �ۼ��� Ƚ��, ��Ŷ �ۼ��� ����Ʈ, ���� ������ ���� ��Ʈ�� ���������� �����Ѵ�.
	@verbatim
	Proacotor_Counter p = performance::proactor_counter();

	@endverbatim
*/
struct Proactor_Counter
{
	//< ���� ���
	volatile long	iTCPReceiveBytes;		///< TCP/IP ���� ����Ʈ
	volatile long	iTCPReceives;			///< TCP/IP ���� Ƚ��
	volatile long	iTCPSentBytes;			///< TCP/IP ���� ����Ʈ
	volatile long	iTCPSents;				///< TCP/IP ���� Ƚ��
	volatile long	iInvalidLengths;		///< ��Ŷ ���� ���� Ƚ��
	volatile long	iInsufficentLengths;	///< ������ ��Ŷ Ƚ��
	volatile long	iResizeBufferLengths;	///< ��Ŷ ���� ���� ũ�� ������ ���� ���� ���Ҵ� Ƚ��

	//< Proactor ���
	volatile long	iBuffers;				///< Buffer ����
	volatile long	iTCPNetLinks;			///< TCP/IP NetLink ����
	volatile long	iTCPAcceptCtxs;			///< TCP/IP Ŭ���̾�Ʈ ���� ��� ��û Ƚ��
	volatile long	iTCPReceiveCtxs;		///< TCP/IP ��Ŷ ���� ��û Ƚ��
	volatile long	iTCPSentCtxs;			///< TCP/IP ��Ŷ ���� ��û Ƚ��
	volatile long	iTCPUnlinkCtxs;			///< TCP/IP Ŭ���̾�Ʈ ���� ��û Ƚ��
	volatile long	iTCPConnectCtxs;		///< TCP/IP ���� ���� ��û Ƚ��

	//< ���� ���
	volatile long	iUDPReceiveBytes;		///< UDP ���� ����Ʈ
	volatile long	iUDPReceives;			///< UDP ���� Ƚ��
	volatile long	iUDPSentBytes;			///< UDP ���� ����Ʈ
	volatile long	iUDPSents;				///< UDP ���� Ƚ��

	//< Proactor ���
	volatile long	iUDPNetLinks;			///< UDP NetLink ����
	volatile long	iUDPReceiveCtxs;		///< UDP ��Ŷ ���� ��û Ƚ��
	volatile long	iUDPSentCtxs;			///< UDP ��Ŷ ���� ��û Ƚ��

	//< �½�ũ �����ٷ� ���
	volatile long	iNetAcceptEvents;		///< Ŭ���̾�Ʈ ���� �۾��̺�Ʈ ����
	volatile long	iNetConnectEvents;		///< ���� ���� �۾��̺�Ʈ ����
	volatile long	iNetReceiveEvents;		///< ��Ŷ ���� �۾��̺�Ʈ ����
	volatile long	iNetFunctorEvents;		///< ��Ʈ�� �Լ� ȣ�� �۾��̺�Ʈ ����
	volatile long	iNetClosedEvents;		///< ���� ���� �۾��̺�Ʈ ����
	volatile long	iNetRemoteCallEvents;	///< RPC ȣ�� �۾��̺�Ʈ ����

	//< RPC ���
	volatile long	iRemoteProcedures;		///< RPC ��� ����
	Proactor_Counter();
};

/** 
	TaskScheduler �� ���� ������ �����ϴ� ����ü.

	@verbatim
	TaskScheduler_Counter p = performance::taskscheduler_counter();

	@endverbatim
*/
struct TaskScheduler_Counter
{
	volatile long	iMaxTaskElapsedTime;		///< �ִ� �۾��Ϸ� �ҿ�ð�
	volatile long	iMinTaskElapsedTime;		///< �ּ� �۾��Ϸ� �ҿ�ð�
	volatile long	iMaxTimerElapsedTime;		///< �ִ� Ÿ�̸ӿϷ� �ҿ�ð�
	volatile long	iMinTimerElapsedTime;		///< �ּ� Ÿ�̸ӿϷ� �ҿ�ð�
	volatile long	iMaxEventElapsedTime;		///< �ִ� �̺�Ʈ�Ϸ� �ҿ�ð�
	volatile long	iMinEventElapsedTime;		///< �ּ� �̺�Ʈ�Ϸ� �ҿ�ð�

	TaskScheduler_Counter();
};

/** 
	RPC.NET�� �������� ����ü
*/
struct Performance_Counter
{
	Proactor_Counter		proactor;
	TaskScheduler_Counter	taskscheduler;
};

/** 
	RPC.NET �� ���������� ������ �������̽��� �����ϴ� ���ӽ����̽�.
*/
namespace performance
{
	/** 
		���ڱݸ޸�Ǯ, Proactor, �½�ũ�����ٷ��� �������� ����ü�� ���´�.
	*/
	Performance_Counter&	counter( void );
	/** 
		Proactor�� �������� ����ü�� �����Ѵ�.
	*/
	Proactor_Counter		proactor_counter();

	/** 
		�½�ũ�����ٷ��� �������� ����ü�� �����Ѵ�.
	*/
	TaskScheduler_Counter	taskscheduler_counter();
};

#define PROACTOR_COUNTER		performance::counter().proactor
#define TASKSCHEDULER_COUNTER	performance::counter().taskscheduler