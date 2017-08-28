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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param rpcid ���� ���ν����� ���� ID
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param rpcid ���� ���ν����� ���� ID
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param rpcid ���� ���ν����� ���� ID
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param rpcid ���� ���ν����� ���� ID
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param rpcid ���� ���ν����� ���� ID
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 ���� ���ν����� ������ �Ű�����4�� �ڷ���

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 ���� ���ν����� ������ �Ű�����4�� �ڷ���

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param rpcid ���� ���ν����� ���� ID
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 ���� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg5 ���� ���ν����� ������ �Ű�����5�� �ڷ���

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
	@def Ŭ������ �ɹ��Լ��� ���� ���ν����� ����ϴ� ��ũ��
	@param inst ���� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func ���� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 ���� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg5 ���� ���ν����� ������ �Ű�����5�� �ڷ���

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid ���� ����ID
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid ���� ����ID
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid ���� ����ID
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid ���� ����ID
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid ���� ����ID
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid ���� ����ID
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 ���� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 ���� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid ���� ����ID
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 ���� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg5 ���� ���ν����� ������ �Ű�����5�� �ڷ���
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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
	@def ���� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func ���� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 ���� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 ���� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 ���� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 ���� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg5 ���� ���ν����� ������ �Ű�����5�� �ڷ���
	@param arg_size ���� ���ν����� ���޵� �Ű������� ������ ������ ũ��

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