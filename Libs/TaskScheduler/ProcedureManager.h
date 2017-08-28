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
#include <Procedure.h>
#include "TaskSchedulerImpl.h"

/** 
	�񵿱� ���ν��� ������ Ŭ����
*/
class ProcedureManager
{
	struct FuncType
	{
		unsigned int		iTSID;		///< �񵿱� ���ν����� ������ �׽�ũ�����ٷ� ����ID
		TaskSchedulerImpl*	pTSch;		///< �񵿱� ���ν����� ������ �׽�ũ�����ٷ� ������
		procedure_base*		pProc;		///< �񵿱� ���ν���
	};
	typedef std::map<unsigned int, FuncType > ProcedureMap;
	ProcedureMap		m_ProcedureMap;	///< �񵿱� ���ν��� �����̳�

public:
	ProcedureManager();
	virtual ~ProcedureManager();

public:
	/** 
		�񵿱� ���ν����� ����Ѵ�.
		@param iTaskSchedulerID �񵿱����ν����� ������ �׽�ũ�����ٷ� ID
		@param pTSch �񵿱� ���ν����� ������ �׽�ũ�����ٷ� ������
		@param iProcID �񵿱� ���ν����� �������̵�
		@param pProc �񵿱� ���ν����� �ּҰ�
	*/
	bool Reg(unsigned int iTaskSchedulerID, TaskSchedulerImpl* pTSch, unsigned int iProcID, procedure_base* pProc);
	/** 
		�񵿱� ���ν����� �����Ѵ�.
		@param iProcID �񵿱� ���ν����� �������̵�
		@param pRef �񵿱� ���ν����� ȣ���� ȣ����
		@param pBuf �񵿱� ���ν����� ���޵� �Ű������� ����ִ� ����
	*/
	bool Call(unsigned iProcID, interface_ref* pRef, Buffer* pBuf);
};