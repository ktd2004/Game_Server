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

///< �����̸� y����
#define isfalse_ret(x,y)			do { if (!(x)) { return y; } } while(0)
///< ���̸� y����
#define istrue_ret(x,y)				do { if (x) { return y; } } while(0)
///< �����̸� y�����ϰ� z����
#define isfalse_do_ret(x,y,z)		do { if (!(x)) { y; return z; } } while(0)
///< ���̸� y�����ϰ� z����
#define istrue_do_ret(x,y,z)		do { if (x) { y; return z; } } while(0)
///< null �̸� y����
#define isnull_ret(x,y)				isfalse_ret(x,y)
///< null �̸� y�����ϰ� z����
#define isnull_do_ret(x,y,z)		isfalse_do_ret(x,y,z)
///< �����̸� continue
#define isfalse_continue(x)			if (!(x)) continue;
///< ���̸� continue
#define istrue_continue(x)			if ((x)) continue;
///< �����̸� y����
#define isfalse_do(x,y)				if (!(x)) { y; }
///< ���̸� y����
#define istrue_do(x,y)				if ((x)) { y; }
///< ���̸� y���� �׷��� ������ z����
#define istrue_else_do(x,y,z)		do { if (x) { y; } else { z; } } while(0)
///< �����̸� y���� �׷��� ������ z����
#define isfalse_else_do(x,y,z)		do { if (!(x)) { y; } else { z; } } while(0)
///< ���̸� goto y
#define istrue_goto(x,y)			do { if (x) { goto y; } } while(0)
///< �����̸� goto y
#define isfalse_goto(x,y)			do { if (!(x)) { goto y; } } while(0)
///< ���̸� y������ �� goto z
#define istrue_do_goto(x,y,z)		do { if (x) { y; goto z; } } while(0)
///< �����̸� y������ �� goto z
#define isfalse_do_goto(x,y,z)		do { if (!(x)) { y; goto z; } } while(0)
///< ���̸� y���� �׷��� ������ z����
#define istrue_else_ret(x,y,z)		do { if (x) { return y; } else { return z; } } while(0)
///< �����̸� y���� �׷��� ������ z����
#define isfalse_else_ret(x,y,z)		do { if (!(x)) { return y; } else { return z; } } while(0)
///< ���̸� y���� �׷��� ������ z����
#define istrue_else_do(x,y,z)		do { if (x) { y; } else { z; } } while(0)
///< null üũ�� ����
#define safe_delete(obj)			do { if ( obj ) delete obj; obj = nullptr; } while(0)
///< null üũ�� ����
#define safe_array_delete(p)		do{if (p) delete[] p; p = nullptr;}while(0)
///< �ڵ尪�� ��ȿ�ϸ� y����
#define isnull_handle_ret(h, y)		do { if (h == INVALID_HANDLE_VALUE || h == nullptr) return y; } while(0)
///< �ڵ鰪�� ��ȿ�ϸ� �ڵ����
#define safe_delete_handle(h)		do { if (h != INVALID_HANDLE_VALUE && h == nullptr) ::CloseHandle(h); } while(0)
///< null �̸� "null"���� �׷��� ���� ���ڿ�����
#define nvl_s(str)					str ? str : "null"
///< ���̸� "true" ���� �׷��� ������ "false" ����
#define bool_s(b)					b ? "true" : "false"
///< std::string�� ���ڿ��� const char* �� ��ȯ
#define cstr(s)						s.c_str()
///< std::string �� null ���� �����Ѵ�.
#define is_empty_cstr_ret_null(x)	(x.empty() ? nullptr : x.c_str())

#include <stdlib.h>
///< ���ڿ� int�� ��ȯ
#define	toint32(pszBuf)		::atoi(pszBuf)
///< ���ڿ� unsigned int�� ��ȯ
#define touint32(pszBuf)	(unsigned int)::_atoi64(pszBuf)
///< ���ڿ� __int64�� ��ȯ
#define toint64(pszBuf)		::_atoi64( pszBuf );
///< ���ڿ� unsigned __int64�� ��ȯ
#define touint64(pszBuf)	(unsigned __int64)::_atoi64(pszBuf)
///< ���ڿ� float�� ��ȯ
#define tofloat(pszBuf)		(float)::atof(pszBuf)
///< ���ڿ� double�� ��ȯ
#define todouble(pszBuf)	::atof(pszBuf)
///< ���ڿ� long�� ��ȯ
#define tolong(pszBuf)		(long)::atol(pszBuf)
///< ���ڿ� unsigned long�� ��ȯ
#define toulong(pszBuf)		(unsigned long)::atol(pszBuf)
///< ���ڿ� int�� ��ȯ
#define	stoint32(s)			::atoi(cstr(s))
///< ���ڿ� unsigned int�� ��ȯ
#define stouint32(s)	(unsigned int)::_atoi64(cstr(s))
///< ���ڿ� __int64�� ��ȯ
#define stoint64(s)			::_atoi64( cstr(s) )
///< ���ڿ� unsigned __int64�� ��ȯ
#define stouint64(s)		(unsigned __int64)::_atoi64(cstr(s))
///< ���ڿ� float�� ��ȯ
#define stofloat(s)			(float)::atof(cstr(s))
///< ���ڿ� double�� ��ȯ
#define stodouble(s)		::atof(cstr(s))
///< ���ڿ� long�� ��ȯ
#define stolong(s)			(long)::atol(cstr(s))
///< ���ڿ� unsigned long�� ��ȯ
#define stoulong(s)			(unsigned long)::atol(cstr(s))

///< ���ڿ� �Լ�
#define basename(x)			util::getfilename(x).c_str()
#define dirname(x)			util::getworkpath(x).c_str()
#define null_string			""
#define bool2str(b)			b ? "true" : "false"
#define align_i(val,width)	util::rpad(util::intstr(val), width)
#define align_s(val,width)	util::lpad(val, width)
#define align_b(val,width)	util::lpad(util::upper(bool2str(val)), width)

///< �����̳� ��ȸ
#define foreach_do(type, it, container)					for (type::iterator it = container.begin(); it != container.end(); ++it )
///< const �����̳� ��ȸ
#define foreach_const_do(type, it, container)					for (type::const_iterator it = container.begin(); it != container.end(); ++it )
///< �� �˻� ���н� ����
#define notfound_map_ret(type,it,container,key,ret)		type::iterator it = container.find(key); if (it == container.end()) return ret;
///< �� �˻� ������ ����
#define found_map_ret(type,it,container,key,ret)		type::iterator it = container.find(key); if (it != container.end()) return ret;

///< ���� 1 ���� ��Ų�� 1 ���� ��Ų ���� �����Ѵ�.
#define locked_inc(dst)								InterlockedIncrement((long*)&dst)
///< ���� 1 ���� ��Ų�� 1 ���� ��Ų ���� �����Ѵ�.
#define locked_dec(dst)								InterlockedDecrement((long*)&dst)
///< ���� ��ȯ���� �񱳽����� ���� �����Ѵ�.
#define locked_exchange(dst,ex)						InterlockedExchange((long*)&dst, (long)ex)
///< ���� ������ ���� ���� �����Ѵ�.
#define locked_exchange_add(dst,add)				InterlockedExchangeAdd((long*)&dst, (long)add)
///< ���� ���� ���� ���� �����Ѵ�.
#define locked_exchange_sub(dst,sub)				InterlockedExchangeAdd((long*)&dst, -(long)sub)
///< ���� ���ؼ� ���� ��� ���� �������� ��ȯ�ϱ� ���� ���� �����Ѵ�.
#define locked_compare_exchange(dst,ex,comp)		InterlockedCompareExchange((long*)&dst, (long)ex, (long)comp)
///< ���� ���ؼ� ���� ��� ���� ��ȯ���� ��ȯ�ߴ��� ���θ� �����Ѵ�.
#define locked_cmp_exchange_ret_bool(dst,ex,comp)	(long)comp == InterlockedCompareExchange((long*)&dst, (long)ex, (long)comp)
///< ���� ������ ���� ����� �����Ѵ�.
#define locked_compare(dst,comp)					(long)comp == InterlockedCompareExchange((long*)&dst, (long)dst, (long)comp)
#if defined(WIN32)
///< ������ ��ȯ
#define locked_exchange_pointer(old_value, new_value)		InterlockedExchangePointer((PVOID*)&old_value, (PVOID)new_value)
///< �����͸� ���ؼ� ���� ��� �������� ��ȯ���� ��ȯ�ϱ� ���� ���� �����Ѵ�.
#define locked_cmp_exchange_pointer(old_value, ex, comp)	InterlockedCompareExchangePointer((PVOID*)&old_value, (PVOID)ex, (PVOID)comp)
#else
///< ������ ��ȯ
#define locked_exchange_pointer(old_value, new_value)		InterlockedExchange64((LONGLONG*)&old_value, (LONGLONG)new_value)
///< �����͸� ���ؼ� ���� ��� �������� ��ȯ���� ��ȯ�ϱ� ���� ���� �����Ѵ�.
#define locked_cmp_exchange_pointer(old_value, ex, comp)	InterlockedCompareExchange64((LONGLONG*)&old_value, (LONGLONG)ex, (LONGLONG)comp)
#endif

///< �ʱ�ȭ �ڵ� ����
#define initializer_begin			namespace { struct dumy { dumy()
///< �ʱ�ȭ �ڵ� ����
#define end_initializer				} instance_dumy; }

///< ��Ŷ ũ�⸦ 512, 1024, 2048 ������ ������
#define ResizeBytes(before, after) \
	do{ \
		uint32 iInx = (before - 1) >> 9; \
		after = (iInx + 1) << 9; \
	} while(0)

///< ��Ʈ���� �����Ǿ� �ִ��� �˻�
#define is_bit_on(flag,bit)		(((flag) & (bit)) != 0)
///< ��Ʈ���� ���������� ������ �˻�
#define is_bit_off(flag,bit)	(((flag) & (bit)) == 0)
///< ��Ʈ���� ����
#define set_bit(flag,bit)		(flag |= (bit))
///< ��Ʈ���� ����
#define clr_bit(flag,bit)		(flag &= ~(bit))

///< �����޼��� ��ȸ
#define errmsg					GetLastErrorMessage(GetLastError())

///< NetLink�� ���� �ּ�
#define net_addr(netlink)		netlink->GetAddr().c_str()