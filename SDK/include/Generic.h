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

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "winmm.lib" )

#if defined(_M_IX86)

#if defined(_DEBUG)
#pragma message ("import generic_x86_mdd")
#pragma comment(lib, "generic_x86_mdd.lib")
#else
#pragma message ("import generic_x86_md")
#pragma comment(lib, "generic_x86_md.lib")
#endif

#elif defined(_M_X64)

#if defined(_DEBUG)
#pragma message ("import generic_x64_mdd")
#pragma comment(lib, "generic_x64_mdd.lib")
#else
#pragma message ("import generic_x64_md")
#pragma comment(lib, "generic_x64_md.lib")
#endif

#endif

#include <Sync.h>
#include <Signal.h>
#include <Md5Hash.h>
#include <StringUtil.h>
#include <TimeUtil.h>
#include <FileUtil.h>
#include <Performance.h>
#include <Random.h>
#include <Logger.h>
#include <Errorcodes.h>
#include <INILoader.h>
#include <CrashDump.h>
#include <Buffer.h>
#include <CSVTable.h>
