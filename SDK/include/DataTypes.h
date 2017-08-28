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

#include <Compile.h>

#ifndef _WINSOCK2API_
#include <winsock2.h>
#include <windows.h>
#endif

#include <MacroFunc.h>

///< 
///< 자료형별 출력 포멧 정리
///< uint   : "%u"
///< int64  : "%I64u"				(-9223372036854775808 ~ 9223372036854775807)
///< llong  : "%I64u" or "%lld"		(-9223372036854775808 ~ 9223372036854775807)
///< uint64 : "%I64d"	or "%llu"	(0 ~ 18446744073709551615)
///< float  : "%f"					(소수점 6자리까지)	4bytes
///< double : "%lf"					(소수점 15자리까지)	8bytes
///< long   : "%ld"					(-2147483648 ~ 2147483648 ) 4bytes

typedef char				int8;
typedef unsigned char		uint8;
typedef unsigned char		uchar;
typedef short				int16;
typedef unsigned short		uint16;
typedef int					int32;
typedef unsigned int		uint32;
typedef __int64				int64;
typedef unsigned __int64	uint64;
typedef long long			llong64;
typedef unsigned long long	ullong64;
typedef unsigned long		ulong32;
typedef long				long32;
typedef uint32				QUID;

///< stl 관련 자료형 정의
#include <list>
#include <queue>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <vector>
#include <iostream>
#include <deque>
#include <sstream>
#include <limits>
#include <memory>
#include <vector>
#include <limits>
#include <memory>
using namespace std;

typedef  std::list<std::string>				stringlist;
typedef  std::list<std::wstring>			wstringlist;
typedef	 std::vector<std::string>			stringvector;
typedef	 std::vector<std::wstring>			wstringvector;
typedef  std::map<uint32, std::string>		uint32_string_map;
typedef  std::map<std::string, uint32>		string_uint32_map;
typedef  std::map<uint64, std::string>		uint64_string_map;
typedef  std::map<std::string, uint64>		string_uint64_map;
typedef  std::map<std::string, std::string> string_string_map;
typedef	 std::map<std::string, double>		string_double_map;
typedef	 std::map<double, std::string>		double_string_map;
typedef	 const std::string					const_string;

#define	nullstring							std::string("")						
