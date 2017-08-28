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
	�½�ũ�����ٷ����� ó���Ǵ� �۾��̺�Ʈ�� �ֻ��� Ŭ����.

	�½�ũ�����ٷ��� �۾��̺�Ʈť�� ������ ������ �۾��̺�Ʈ ť�� ���޵� �޽����� TaskEventMsg �� ��ӹ޾� OnHandle() �Լ��� �����ؾ� �Ѵ�.
	�۾��̺�Ʈ�� ������ �½�ũ�����ٷ��� OnHandle() �Լ��� ȣ������ �۾��̺�Ʈ�� �����Ѵ�.

	@verbatim
	class MyMsg : public TaskEventMsg
	{
	public:
		MyMsg() {}
		virtual ~MyMsg() {}

		void OnHandle( void )
		{
			// do something...
		}
	};
	@endverbatim
*/
class TaskEventMsg : public OVERLAPPED
{
protected:
	int		m_iTaskID;		///< �½�ũ �����ٷ��� ������ ��ũ��ID

public:
	enum eTaskID
	{ eInvalid_Task = -1, };

	TaskEventMsg() : m_iTaskID(eInvalid_Task) {}
	TaskEventMsg(int iTask) : m_iTaskID(iTask) {}
	TaskEventMsg( const TaskEventMsg& rEv ) : m_iTaskID(rEv.m_iTaskID) {}
	virtual ~TaskEventMsg() {}

	/**
		�۾��̺�Ʈ�� ����� �½�ũID ��ȸ�Ѵ�.
	*/
	inline int		Task( void ) { return m_iTaskID; }
	/**
		�۾��̺�Ʈ�� ����� �½�ũID �����Ѵ�. �½�ũ�����ٷ��� �۾��̺�Ʈ�� �����ϸ� �½�ũID�� ��Ī�Ǵ� �½�ũ ������ �����Ѵ�.
		@param iTask �½�Ʈ�����ٷ��� ������ �½�ũID
	*/
	inline void		Task( int iTask ) { m_iTaskID = iTask; }
	/**
		�½�ũ�����ٷ��� �۾��̺�Ʈ�� �����ϸ� ȣ��Ǵ� �̺�Ʈ�Լ��̴�.
		�½�ũ�����ٷ��� ������ �۾��̺�Ʈ�� TaskEventMsg �� ��ӹ޾� OnHandle �Լ��� �����ؾ� �Ѵ�.
	*/
	virtual void	OnHandle( void ) = 0;
};
