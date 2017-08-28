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
#include <NetLink.h>
#include <Packet.h>

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param rpcid 원격 프로시져의 고유 ID
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_V0(1000, this, &ItemManager::Func1 );
		}
		virtual ~ItemManager() {}

		bool Func1( NetLinkPtr pCaller )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_V0(rpcid, inst, func) \
	do { \
		rpc_v0* proc = new rpc_v0; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( rpcid, proc ); \
	} while(0)

#define _EXPORT_RPC_V0(net, rpcid, inst, func) \
	do { \
		rpc_v0* proc = new rpc_v0; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( rpcid, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_NM_V0(this, &ItemManager::Func1 );
		}
		virtual ~ItemManager() {}

		bool Func1( NetLinkPtr pCaller )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_NM_V0(inst, func) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v0* proc = new rpc_v0; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( hashval, proc ); \
	} while(0)

#define _EXPORT_RPC_NM_V0(net, rpcid, inst, func) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v0* proc = new rpc_v0; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( hashval, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param rpcid 원격 프로시져의 고유 ID
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_V1(1000, this, &ItemManager::Func2, int );
		}
		virtual ~ItemManager() {}

		bool Func2( NetLinkPtr pCaller, int iVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_V1(rpcid, inst, func, arg1) \
	do { \
		rpc_v1<arg1>* proc = new rpc_v1<arg1>; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( rpcid, proc ); \
	} while(0)

#define _EXPORT_RPC_V1(net, rpcid, inst, func, arg1) \
	do { \
		rpc_v1<arg1>* proc = new rpc_v1<arg1>; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( rpcid, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_NM_V1(this, &ItemManager::Func2, int );
		}
		virtual ~ItemManager() {}

		bool Func2( NetLinkPtr pCaller, int iVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_NM_V1(inst, func, arg1) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v1<arg1>* proc = new rpc_v1<arg1>; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( hashval, proc ); \
	} while(0)

#define _EXPORT_RPC_NM_V1(net, inst, func, arg1) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v1<arg1>* proc = new rpc_v1<arg1>; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( hashval, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param rpcid 원격 프로시져의 고유 ID
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_V2(1000, this, &ItemManager::Func3, int, const std::string& );
		}
		virtual ~ItemManager() {}

		bool Func3( NetLinkPtr pCaller, int iVal, const std::string& sVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_V2(rpcid, inst, func, arg1, arg2) \
	do { \
		rpc_v2<arg1,arg2>* proc = new rpc_v2<arg1,arg2>; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( rpcid, proc ); \
	} while(0)

#define _EXPORT_RPC_V2(net, rpcid, inst, func, arg1, arg2) \
	do { \
		rpc_v2<arg1,arg2>* proc = new rpc_v2<arg1,arg2>; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( rpcid, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_NM_V2(this, &ItemManager::Func3, int, const std::string& );
		}
		virtual ~ItemManager() {}

		bool Func3( NetLinkPtr pCaller, int iVal, const std::string& sVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_NM_V2(inst, func, arg1, arg2) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v2<arg1,arg2>* proc = new rpc_v2<arg1,arg2>; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( hashval, proc ); \
	} while(0)

#define _EXPORT_RPC_NM_V2(net, inst, func, arg1, arg2) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v2<arg1,arg2>* proc = new rpc_v2<arg1,arg2>; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( hashval, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param rpcid 원격 프로시져의 고유 ID
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_V3(1000, this, &ItemManager::Func4, int, const std::string&, long );
		}
		virtual ~ItemManager() {}

		bool Func4( NetLinkPtr pCaller, int iVal, const std::string& sVal, long lVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_V3(rpcid, inst, func, arg1, arg2, arg3) \
	do { \
		rpc_v3<arg1,arg2,arg3>* proc = new rpc_v3<arg1,arg2,arg3>; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( rpcid, proc ); \
	} while(0)

#define _EXPORT_RPC_V3(net, rpcid, inst, func, arg1, arg2, arg3) \
	do { \
		rpc_v3<arg1,arg2,arg3>* proc = new rpc_v3<arg1,arg2,arg3>; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( rpcid, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_NM_V3(this, &ItemManager::Func4, int, const std::string&, long );
		}
		virtual ~ItemManager() {}

		bool Func4( NetLinkPtr pCaller, int iVal, const std::string& sVal, long lVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_NM_V3(inst, func, arg1, arg2, arg3) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v3<arg1,arg2,arg3>* proc = new rpc_v3<arg1,arg2,arg3>; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( hashval, proc ); \
	} while(0)

#define _EXPORT_RPC_NM_V3(net, inst, func, arg1, arg2, arg3) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v3<arg1,arg2,arg3>* proc = new rpc_v3<arg1,arg2,arg3>; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( hashval, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param rpcid 원격 프로시져의 고유 ID
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형
	@param arg4 원격 프로시져에 전달할 매개변수4의 자료형

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_V4(1000, this, &ItemManager::Func5, int, const std::string&, long, float );
		}
		virtual ~ItemManager() {}

		bool Func5( NetLinkPtr pCaller, int iVal, const std::string& sVal, long lVal, float fVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_V4(rpcid, inst, func, arg1, arg2, arg3, arg4) \
	do { \
		rpc_v4<arg1,arg2,arg3,arg4>* proc = new rpc_v4<arg1,arg2,arg3,arg4>; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( rpcid, proc ); \
	} while(0)

#define _EXPORT_RPC_V4(net, rpcid, inst, func, arg1, arg2, arg3, arg4) \
	do { \
		rpc_v4<arg1,arg2,arg3,arg4>* proc = new rpc_v4<arg1,arg2,arg3,arg4>; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( rpcid, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형
	@param arg4 원격 프로시져에 전달할 매개변수4의 자료형

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_NM_V4(this, &ItemManager::Func5, int, const std::string&, long, float );
		}
		virtual ~ItemManager() {}

		bool Func5( NetLinkPtr pCaller, int iVal, const std::string& sVal, long lVal, float fVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_NM_V4(inst, func, arg1, arg2, arg3, arg4) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v4<arg1,arg2,arg3,arg4>* proc = new rpc_v4<arg1,arg2,arg3,arg4>; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( hashval, proc ); \
	} while(0)

#define _EXPORT_RPC_NM_V4(net, inst, func, arg1, arg2, arg3, arg4) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v4<arg1,arg2,arg3,arg4>* proc = new rpc_v4<arg1,arg2,arg3,arg4>; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( hashval, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param rpcid 원격 프로시져의 고유 ID
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형
	@param arg4 원격 프로시져에 전달할 매개변수4의 자료형
	@param arg5 원격 프로시져에 전달할 매개변수5의 자료형

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_V5(1000, this, &ItemManager::Func6, int, const std::string&, long, float, double );
		}
		virtual ~ItemManager() {}

		bool Func6( NetLinkPtr pCaller, int iVal, const std::string& sVal, long lVal, float fVal, double dVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_V5(rpcid, inst, func, arg1, arg2, arg3, arg4, arg5) \
	do { \
		rpc_v5<arg1,arg2,arg3,arg4,arg5>* proc = new rpc_v5<arg1,arg2,arg3,arg4,arg5>; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( rpcid, proc ); \
	} while(0)

#define _EXPORT_RPC_V5(net, rpcid, inst, func, arg1, arg2, arg3, arg4, arg5) \
	do { \
		rpc_v5<arg1,arg2,arg3,arg4,arg5>* proc = new rpc_v5<arg1,arg2,arg3,arg4,arg5>; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( rpcid, proc ); \
	} while(0)

/**
	@def 클래스의 맴버함수를 원격 프로시져로 등록하는 매크로
	@param inst 원격 프로시져로 등록할 맴버함수를 가지고 있는 객체의 주소값
	@param func 원격 프로시져로 등록할 맴버함수의 주소값
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형
	@param arg4 원격 프로시져에 전달할 매개변수4의 자료형
	@param arg5 원격 프로시져에 전달할 매개변수5의 자료형

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_RPC_NM_V5(this, &ItemManager::Func6, int, const std::string&, long, float, double );
		}
		virtual ~ItemManager() {}

		bool Func6( NetLinkPtr pCaller, int iVal, const std::string& sVal, long lVal, float fVal, double dVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_RPC_NM_V5(inst, func, arg1, arg2, arg3, arg4, arg5) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v5<arg1,arg2,arg3,arg4,arg5>* proc = new rpc_v5<arg1,arg2,arg3,arg4,arg5>; \
		proc->m_fn.assign(inst, func); \
		NetEventHandler::RegRemoteProcedure( hashval, proc ); \
	} while(0)

#define _EXPORT_RPC_NM_V5(net, inst, func, arg1, arg2, arg3, arg4, arg5) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		rpc_v5<arg1,arg2,arg3,arg4,arg5>* proc = new rpc_v5<arg1,arg2,arg3,arg4,arg5>; \
		proc->m_fn.assign(inst, func); \
		net->RegRemoteProcedure( hashval, proc ); \
	} while(0)


/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param procid 원격 고유ID
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_V0( 1000, Func1, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_V0(rpcid, func, arg_size) \
	inline Buffer* get_##func( void ) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink) \
	{ \
		Buffer* pBuf = get_##func(); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}

/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param procid 원격 고유ID
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_NM_V0( Func1, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_NM_V0(func, arg_size) \
	inline Buffer* get_##func( void ) \
	{ \
		static unsigned int rpcid = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink) \
	{ \
		Buffer* pBuf = get_##func(); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}

/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param procid 원격 고유ID
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_V1( 1000, Func2, int, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_V1(rpcid, func, type1, arg_size) \
	inline Buffer* get_##func( type1 arg1 ) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		pk << arg1; \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink, type1 arg1) \
	{ \
		Buffer* pBuf = get_##func( arg1 ); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}

/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_NM_V1( Func2, int, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_NM_V1(func, type1, arg_size) \
	inline Buffer* get_##func( type1 arg1 ) \
	{ \
		static unsigned int rpcid = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		pk << arg1; \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink, type1 arg1) \
	{ \
		Buffer* pBuf = get_##func( arg1 ); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}

/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param procid 원격 고유ID
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_V2( 1000, Func3, int, const std::string&, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_V2(rpcid, func, type1, type2, arg_size) \
	inline Buffer* get_##func( type1 arg1, type2 arg2 ) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		pk << arg1 << arg2; \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink, type1 arg1, type2 arg2) \
	{ \
		Buffer* pBuf = get_##func( arg1, arg2 ); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}


/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_NM_V2( Func3, int, const std::string&, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_NM_V2(func, type1, type2, arg_size) \
	inline Buffer* get_##func( type1 arg1, type2 arg2 ) \
	{ \
		static unsigned int rpcid = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		pk << arg1 << arg2; \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink, type1 arg1, type2 arg2) \
	{ \
		Buffer* pBuf = get_##func( arg1, arg2 ); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}

/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param procid 원격 고유ID
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_V3( 1000, Func4, int, const std::string&, long, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_V3(rpcid, func, type1, type2, type3, arg_size) \
	inline Buffer* get_##func( type1 arg1, type2 arg2, type3 arg3 ) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		pk << arg1 << arg2 << arg3; \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink, type1 arg1, type2 arg2, type3 arg3) \
	{ \
		Buffer* pBuf = get_##func( arg1, arg2, arg3 ); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}

/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_NM_V3( Func4, int, const std::string&, long, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_NM_V3(func, type1, type2, type3, arg_size) \
	inline Buffer* get_##func( type1 arg1, type2 arg2, type3 arg3 ) \
	{ \
		static unsigned int rpcid = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		pk << arg1 << arg2 << arg3; \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink, type1 arg1, type2 arg2, type3 arg3) \
	{ \
		Buffer* pBuf = get_##func( arg1, arg2, arg3 ); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}


/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param procid 원격 고유ID
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형
	@param arg4 원격 프로시져에 전달할 매개변수4의 자료형
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_V4( 1000, Func5, int, const std::string&, long, float, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_V4(rpcid, func, type1, type2, type3, type4, arg_size) \
	inline Buffer* get_##func( type1 arg1, type2 arg2, type3 arg3, type4 arg4 ) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		pk << arg1 << arg2 << arg3 << arg4; \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink, type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
	{ \
		Buffer* pBuf = get_##func( arg1, arg2, arg3, arg4 ); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}

/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형
	@param arg4 원격 프로시져에 전달할 매개변수4의 자료형
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_NM_V4( Func5, int, const std::string&, long, float, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_NM_V4(func, type1, type2, type3, type4, arg_size) \
	inline Buffer* get_##func( type1 arg1, type2 arg2, type3 arg3, type4 arg4 ) \
	{ \
		static unsigned int rpcid = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		pk << arg1 << arg2 << arg3 << arg4; \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink, type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
	{ \
		Buffer* pBuf = get_##func( arg1, arg2, arg3, arg4 ); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}


/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param procid 원격 고유ID
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형
	@param arg4 원격 프로시져에 전달할 매개변수4의 자료형
	@param arg5 원격 프로시져에 전달할 매개변수5의 자료형
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_V5( 1000, Func6, int, const std::string&, long, float, double, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_V5(rpcid, func, type1, type2, type3, type4, type5, arg_size) \
	inline Buffer* get_##func( type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5 ) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		pk << arg1 << arg2 << arg3 << arg4 << arg5; \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5) \
	{ \
		Buffer* pBuf = get_##func( arg1, arg2, arg3, arg4, arg5 ); \
		if ( spLink ) \
		{ \
			return spLink->Send( pBuf ); \
		} \
		pBuf->release(); \
		return false; \
	}

/**
	@def 원격 프로시져로 등록된 함수를 클래스의 맴버함수로 정의하는 매크로
	@param func 원격 프로시져를 호출하는 맴버함수의 이름
	@param arg1 원격 프로시져에 전달할 매개변수1의 자료형
	@param arg2 원격 프로시져에 전달할 매개변수2의 자료형
	@param arg3 원격 프로시져에 전달할 매개변수3의 자료형
	@param arg4 원격 프로시져에 전달할 매개변수4의 자료형
	@param arg5 원격 프로시져에 전달할 매개변수5의 자료형
	@param arg_size 원격 프로시져에 전달될 매개변수를 저장할 버퍼의 크기

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_RPC_NM_V5( Func6, int, const std::string&, long, float, double, 1024 );
	};
	@endverbatim
*/
#define IMPORT_RPC_NM_V5(func, type1, type2, type3, type4, type5, arg_size) \
	inline Buffer* get_##func( type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5 ) \
	{ \
		static unsigned int rpcid = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		Packet pk(pBuf); \
		pk.id(rpcid); \
		pk << arg1 << arg2 << arg3 << arg4 << arg5; \
		return pk.get(); \
	} \
	inline bool rpc_##func(NetLinkPtr& spLink, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5) \
	{ \
		Buffer* pBuf = get_##func( arg1, arg2, arg3, arg4, arg5 ); \
		if ( spLink ) \
		{ \
			return spLink->Send( pk.get() ); \
		} \
		pBuf->release(); \
		return false; \
	}