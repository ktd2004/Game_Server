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
	태스크스케줄러에서 처리되는 작업이벤트의 최상위 클래스.

	태스크스케줄러는 작업이벤트큐를 가지고 있으며 작업이벤트 큐에 전달될 메시지는 TaskEventMsg 를 상속받아 OnHandle() 함수를 구현해야 한다.
	작업이벤트를 수신한 태스크스케줄러는 OnHandle() 함수를 호출한후 작업이벤트를 삭제한다.

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
	int		m_iTaskID;		///< 태스크 스케줄러가 실행한 태크스ID

public:
	enum eTaskID
	{ eInvalid_Task = -1, };

	TaskEventMsg() : m_iTaskID(eInvalid_Task) {}
	TaskEventMsg(int iTask) : m_iTaskID(iTask) {}
	TaskEventMsg( const TaskEventMsg& rEv ) : m_iTaskID(rEv.m_iTaskID) {}
	virtual ~TaskEventMsg() {}

	/**
		작업이벤트에 저장된 태스크ID 조회한다.
	*/
	inline int		Task( void ) { return m_iTaskID; }
	/**
		작업이벤트에 저장된 태스크ID 설정한다. 태스크스케줄러는 작업이벤트를 수신하면 태스크ID와 매칭되는 태스크 꺼내와 실행한다.
		@param iTask 태스트스케줄러가 실행할 태스크ID
	*/
	inline void		Task( int iTask ) { m_iTaskID = iTask; }
	/**
		태스크스케줄러가 작업이벤트를 실행하면 호출되는 이벤트함수이다.
		태스크스케줄러로 전달할 작업이벤트는 TaskEventMsg 를 상속받아 OnHandle 함수를 구현해야 한다.
	*/
	virtual void	OnHandle( void ) = 0;
};
