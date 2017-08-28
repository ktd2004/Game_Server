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
#include <pdh.h>

struct IPdh
{
	struct ModuleCounter
	{
		LONG ID;				// process id
		LONG ThreadCount;		// thread ����
		double ProcessTime;		// ���μ��� ���ð�
//		double usertime;		// �������α׷� cpu ������(�ý����� ����)
//		double workingset;		// working set ������
		double VirtualBytes;	// ��� �޸�
		double PrivateBytes;
//		double pagefilebytes;	// page ������ ����Ʈ
		double UsageOfProcessor;// ��ü cpu ������
		double UsageOfMemory;	// ��ü �޸� ����
		ModuleCounter()
		{
			memset( this, 0x00, sizeof(ModuleCounter) );
		}
	};
	struct ThreadCounter
	{
		LONG id;
		LONG ContextSwitch;		// context switching
		ThreadCounter()
		{
			memset( this, 0x00, sizeof(ThreadCounter) );
		}
//		double proctime;
//		double usertime;
	};
	typedef std::vector<ThreadCounter> ThreadCounterVec;

	struct ProcessorCounter
	{
		uint32	ID;				// �ھ��ȣ ( 1, 2, 3, 4 ...)
		double	Usage;			// cpu ����
		ProcessorCounter()
		{
			memset( this, 0x00, sizeof(ProcessorCounter) );
		}
	};

	typedef std::vector<ProcessorCounter> ProcessorCounterVec;

	virtual bool Open(const std::string& sProcessName, DWORD ProcessID) = 0;
	virtual bool Close() = 0;
	virtual bool CollectData( ModuleCounter* pModuleCounter ) = 0;
	virtual bool CollectData( ThreadCounterVec* pThreadCounter ) = 0;
	virtual bool CollectData( ProcessorCounterVec* pProcessorCounter ) = 0;
	virtual bool CollectData( ModuleCounter* pModuleCounter, ThreadCounterVec* pThreadCounter, ProcessorCounterVec* pProcessorCounter ) = 0;
};

namespace Pdh
{
	IPdh*	GetPdhMonitor();
	void	PrintModuleCounter( IPdh::ModuleCounter& x );
	void	PrintProcessorCounter( IPdh::ProcessorCounterVec& x );
	void	PrintThreadCounter( IPdh::ThreadCounterVec& x );
}