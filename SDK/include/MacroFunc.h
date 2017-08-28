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
//<
#include <DataTypes.h>

///< 거짓이면 y리턴
#define isfalse_ret(x,y)			do { if (!(x)) { return y; } } while(0)
///< 참이면 y리턴
#define istrue_ret(x,y)				do { if (x) { return y; } } while(0)
///< 거짓이면 y실행하고 z리턴
#define isfalse_do_ret(x,y,z)		do { if (!(x)) { y; return z; } } while(0)
///< 참이면 y실행하고 z리턴
#define istrue_do_ret(x,y,z)		do { if (x) { y; return z; } } while(0)
///< null 이면 y리턴
#define isnull_ret(x,y)				isfalse_ret(x,y)
///< null 이면 y실행하고 z리턴
#define isnull_do_ret(x,y,z)		isfalse_do_ret(x,y,z)
///< 거짓이면 continue
#define isfalse_continue(x)			if (!(x)) continue;
///< 참이면 continue
#define istrue_continue(x)			if ((x)) continue;
///< 거짓이면 y실행
#define isfalse_do(x,y)				if (!(x)) { y; }
///< 참이면 y실행
#define istrue_do(x,y)				if ((x)) { y; }
///< 참이면 y샐행 그렇지 않으면 z실행
#define istrue_else_do(x,y,z)		do { if (x) { y; } else { z; } } while(0)
///< 거짓이면 y샐행 그렇지 않으면 z실행
#define isfalse_else_do(x,y,z)		do { if (!(x)) { y; } else { z; } } while(0)
///< 참이면 goto y
#define istrue_goto(x,y)			do { if (x) { goto y; } } while(0)
///< 거짓이면 goto y
#define isfalse_goto(x,y)			do { if (!(x)) { goto y; } } while(0)
///< 참이면 y실행한 후 goto z
#define istrue_do_goto(x,y,z)		do { if (x) { y; goto z; } } while(0)
///< 거짓이면 y실행한 후 goto z
#define isfalse_do_goto(x,y,z)		do { if (!(x)) { y; goto z; } } while(0)
///< 참이면 y리턴 그렇지 않으면 z리턴
#define istrue_else_ret(x,y,z)		do { if (x) { return y; } else { return z; } } while(0)
///< 거짓이면 y리턴 그렇지 않으면 z리턴
#define isfalse_else_ret(x,y,z)		do { if (!(x)) { return y; } else { return z; } } while(0)
///< 참이면 y실행 그렇지 않으면 z실행
#define istrue_else_do(x,y,z)		do { if (x) { y; } else { z; } } while(0)
///< null 체크후 삭제
#define safe_delete(obj)			do { if ( obj ) delete obj; obj = nullptr; } while(0)
///< null 체크후 삭제
#define safe_array_delete(p)		do{if (p) delete[] p; p = nullptr;}while(0)
///< 핸드값이 유효하면 y리턴
#define isnull_handle_ret(h, y)		do { if (h == INVALID_HANDLE_VALUE || h == nullptr) return y; } while(0)
///< 핸들값이 유효하면 핸들삭제
#define safe_delete_handle(h)		do { if (h != INVALID_HANDLE_VALUE && h == nullptr) ::CloseHandle(h); } while(0)
///< null 이면 "null"리턴 그렇지 않은 문자열리턴
#define nvl_s(str)					str ? str : "null"
///< 참이면 "true" 리턴 그렇지 않으면 "false" 리턴
#define bool_s(b)					b ? "true" : "false"
///< std::string의 문자열을 const char* 로 반환
#define cstr(s)						s.c_str()
///< std::string 이 null 인지 조사한다.
#define is_empty_cstr_ret_null(x)	(x.empty() ? nullptr : x.c_str())

#include <stdlib.h>
///< 문자열 int로 변환
#define	toint32(pszBuf)		::atoi(pszBuf)
///< 문자열 unsigned int로 변환
#define touint32(pszBuf)	(unsigned int)::_atoi64(pszBuf)
///< 문자열 __int64로 변환
#define toint64(pszBuf)		::_atoi64( pszBuf );
///< 문자열 unsigned __int64로 변환
#define touint64(pszBuf)	(unsigned __int64)::_atoi64(pszBuf)
///< 문자열 float로 변환
#define tofloat(pszBuf)		(float)::atof(pszBuf)
///< 문자열 double로 변환
#define todouble(pszBuf)	::atof(pszBuf)
///< 문자열 long로 변환
#define tolong(pszBuf)		(long)::atol(pszBuf)
///< 문자열 unsigned long로 변환
#define toulong(pszBuf)		(unsigned long)::atol(pszBuf)
///< 문자열 int로 변환
#define	stoint32(s)			::atoi(cstr(s))
///< 문자열 unsigned int로 변환
#define stouint32(s)	(unsigned int)::_atoi64(cstr(s))
///< 문자열 __int64로 변환
#define stoint64(s)			::_atoi64( cstr(s) )
///< 문자열 unsigned __int64로 변환
#define stouint64(s)		(unsigned __int64)::_atoi64(cstr(s))
///< 문자열 float로 변환
#define stofloat(s)			(float)::atof(cstr(s))
///< 문자열 double로 변환
#define stodouble(s)		::atof(cstr(s))
///< 문자열 long로 변환
#define stolong(s)			(long)::atol(cstr(s))
///< 문자열 unsigned long로 변환
#define stoulong(s)			(unsigned long)::atol(cstr(s))

///< 문자열 함수
#define basename(x)			util::getfilename(x).c_str()
#define dirname(x)			util::getworkpath(x).c_str()
#define null_string			""
#define bool2str(b)			b ? "true" : "false"
#define align_i(val,width)	util::rpad(util::intstr(val), width)
#define align_s(val,width)	util::lpad(val, width)
#define align_b(val,width)	util::lpad(util::upper(bool2str(val)), width)

///< 컨테이너 순회
#define foreach_do(type, it, container)					for (type::iterator it = container.begin(); it != container.end(); ++it )
///< const 컨테이너 순회
#define foreach_const_do(type, it, container)					for (type::const_iterator it = container.begin(); it != container.end(); ++it )
///< 맵 검색 실패시 리턴
#define notfound_map_ret(type,it,container,key,ret)		type::iterator it = container.find(key); if (it == container.end()) return ret;
///< 맵 검색 성공시 리턴
#define found_map_ret(type,it,container,key,ret)		type::iterator it = container.find(key); if (it != container.end()) return ret;

///< 값을 1 증가 시킨후 1 증가 시킨 값을 리턴한다.
#define locked_inc(dst)								InterlockedIncrement((long*)&dst)
///< 값을 1 감소 시킨후 1 감소 시킨 값을 리턴한다.
#define locked_dec(dst)								InterlockedDecrement((long*)&dst)
///< 값을 교환한후 비교시점의 값을 리턴한다.
#define locked_exchange(dst,ex)						InterlockedExchange((long*)&dst, (long)ex)
///< 값을 더히기 전의 값을 리턴한다.
#define locked_exchange_add(dst,add)				InterlockedExchangeAdd((long*)&dst, (long)add)
///< 값을 빼기 전의 값을 리턴한다.
#define locked_exchange_sub(dst,sub)				InterlockedExchangeAdd((long*)&dst, -(long)sub)
///< 값을 비교해서 참일 경우 값을 교한한후 교환하기 전의 값을 리턴한다.
#define locked_compare_exchange(dst,ex,comp)		InterlockedCompareExchange((long*)&dst, (long)ex, (long)comp)
///< 값을 비교해서 참일 경우 값을 교환한후 교환했는지 여부를 리턴한다.
#define locked_cmp_exchange_ret_bool(dst,ex,comp)	(long)comp == InterlockedCompareExchange((long*)&dst, (long)ex, (long)comp)
///< 값이 같은지 비교한 결과를 리턴한다.
#define locked_compare(dst,comp)					(long)comp == InterlockedCompareExchange((long*)&dst, (long)dst, (long)comp)
#if defined(WIN32)
///< 포인터 교환
#define locked_exchange_pointer(old_value, new_value)		InterlockedExchangePointer((PVOID*)&old_value, (PVOID)new_value)
///< 포인터를 비교해서 참일 경우 포인터을 교환한후 교환하기 전의 값을 리턴한다.
#define locked_cmp_exchange_pointer(old_value, ex, comp)	InterlockedCompareExchangePointer((PVOID*)&old_value, (PVOID)ex, (PVOID)comp)
#else
///< 포인터 교환
#define locked_exchange_pointer(old_value, new_value)		InterlockedExchange64((LONGLONG*)&old_value, (LONGLONG)new_value)
///< 포인터를 비교해서 참일 경우 포인터을 교환한후 교환하기 전의 값을 리턴한다.
#define locked_cmp_exchange_pointer(old_value, ex, comp)	InterlockedCompareExchange64((LONGLONG*)&old_value, (LONGLONG)ex, (LONGLONG)comp)
#endif

///< 초기화 코드 시작
#define initializer_begin			namespace { struct dumy { dumy()
///< 초기화 코드 종료
#define end_initializer				} instance_dumy; }

///< 패킷 크기를 512, 1024, 2048 단위로 재조정
#define ResizeBytes(before, after) \
	do{ \
		uint32 iInx = (before - 1) >> 9; \
		after = (iInx + 1) << 9; \
	} while(0)

///< 비트값을 설정되어 있는지 검사
#define is_bit_on(flag,bit)		(((flag) & (bit)) != 0)
///< 비트값이 설정되있지 않은지 검사
#define is_bit_off(flag,bit)	(((flag) & (bit)) == 0)
///< 비트값을 설정
#define set_bit(flag,bit)		(flag |= (bit))
///< 비트값을 비설정
#define clr_bit(flag,bit)		(flag &= ~(bit))

///< 에러메세지 조회
#define errmsg					GetLastErrorMessage(GetLastError())

///< NetLink의 원격 주소
#define net_addr(netlink)		netlink->GetAddr().c_str()