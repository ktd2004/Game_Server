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
#include "stdafx.h"
#include <DataTypes.h>
#include <Performance.h>

//<
Proactor_Counter::Proactor_Counter()
{
	memset( this, 0x00, sizeof(Proactor_Counter) );
}

TaskScheduler_Counter::TaskScheduler_Counter()
{
	memset( this, 0x00, sizeof(TaskScheduler_Counter) );
}

//<
namespace
{
	static Performance_Counter	g_Performance_Counter;
};

//<
namespace performance
{
	Performance_Counter& counter( void )
	{
		return g_Performance_Counter;
	}

#define PROACTOR_COUNTER_VAR	g_Performance_Counter.proactor

	Proactor_Counter proactor_counter( void )
	{
		Proactor_Counter c = PROACTOR_COUNTER_VAR;
		locked_exchange(PROACTOR_COUNTER_VAR.iTCPReceiveBytes, 0);
		locked_exchange(PROACTOR_COUNTER_VAR.iTCPReceives, 0);
		locked_exchange(PROACTOR_COUNTER_VAR.iTCPSentBytes, 0);
		locked_exchange(PROACTOR_COUNTER_VAR.iTCPSents, 0);
		locked_exchange(PROACTOR_COUNTER_VAR.iUDPReceiveBytes, 0);
		locked_exchange(PROACTOR_COUNTER_VAR.iUDPReceives, 0);
		locked_exchange(PROACTOR_COUNTER_VAR.iUDPSentBytes, 0);
		locked_exchange(PROACTOR_COUNTER_VAR.iUDPSents, 0);
		locked_exchange(PROACTOR_COUNTER_VAR.iInvalidLengths, 0);
		locked_exchange(PROACTOR_COUNTER_VAR.iInsufficentLengths, 0);
		locked_exchange(PROACTOR_COUNTER_VAR.iResizeBufferLengths, 0);
		return c;
	}

	TaskScheduler_Counter taskscheduler_counter( void )
	{
		return g_Performance_Counter.taskscheduler;
	}
};