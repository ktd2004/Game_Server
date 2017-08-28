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
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param procid �񵿱� ���ν����� ���� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_V0(1, 1000, this, &ItemManager::Func1);
		}
		virtual ~ItemManager() {}

		bool Func1( interface_ref* pCaller )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_V0(tschid, procid, inst, func) \
	do { \
		procedure_v0* proc = new procedure_v0; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, procid, proc ); \
	} while(0)

/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_NM_V0(1, this, &ItemManager::Func1);
		}
		virtual ~ItemManager() {}

		bool Func1( interface_ref* pCaller )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_NM_V0(tschid, inst, func) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		procedure_v0* proc = new procedure_v0; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, hashval, proc ); \
	} while(0)

/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param procid �񵿱� ���ν����� ���� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_V1(1, 1000, this, &ItemManager::Func2, int);
		}
		virtual ~ItemManager() {}

		bool Func2( interface_ref* pCaller, int iVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_V1(tschid, procid, inst, func, arg1) \
	do { \
		procedure_v1<arg1>* proc = new procedure_v1<arg1>; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, procid, proc ); \
	} while(0)

/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_NM_V1(1, this, &ItemManager::Func2, int);
		}
		virtual ~ItemManager() {}

		bool Func2( interface_ref* pCaller, int iVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_NM_V1(tschid, inst, func, arg1) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		procedure_v1<arg1>* proc = new procedure_v1<arg1>; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, hashval, proc ); \
	} while(0)
/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param procid �񵿱� ���ν����� ���� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_V2(1, 1000, this, &ItemManager::Func3, int, const std::string& );
		}
		virtual ~ItemManager() {}

		bool Func3( interface_ref* pCaller, int iVal, const std::string& sVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_V2(tschid, procid, inst, func, arg1, arg2) \
	do { \
		procedure_v2<arg1,arg2>* proc = new procedure_v2<arg1,arg2>; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, procid, proc ); \
	} while(0)

/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_NM_V2(1, this, &ItemManager::Func3, int, const std::string& );
		}
		virtual ~ItemManager() {}

		bool Func3( interface_ref* pCaller, int iVal, const std::string& sVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_NM_V2(tschid, inst, func, arg1, arg2) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		procedure_v2<arg1,arg2>* proc = new procedure_v2<arg1,arg2>; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, hashval, proc ); \
	} while(0)

/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param procid �񵿱� ���ν����� ���� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_V3(1, 1000, this, &ItemManager::Func4, int, const std::string&, long );
		}
		virtual ~ItemManager() {}

		bool Func4( interface_ref* pCaller, int iVal, const std::string& sVal, long lVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_V3(tschid, procid, inst, func, arg1, arg2, arg3) \
	do { \
		procedure_v3<arg1,arg2,arg3>* proc = new procedure_v3<arg1,arg2,arg3>; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, procid, proc ); \
	} while(0)

/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_NM_V3(1, this, &ItemManager::Func4, int, const std::string&, long );
		}
		virtual ~ItemManager() {}

		bool Func4( interface_ref* pCaller, int iVal, const std::string& sVal, long lVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_NM_V3(tschid, inst, func, arg1, arg2, arg3) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		procedure_v3<arg1,arg2,arg3>* proc = new procedure_v3<arg1,arg2,arg3>; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, hashval, proc ); \
	} while(0)

/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param procid �񵿱� ���ν����� ���� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 �񵿱� ���ν����� ������ �Ű�����4�� �ڷ���

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_V4(1, 1000, this, &ItemManager::Func5, int, const std::string&, long, float );
		}
		virtual ~ItemManager() {}

		bool Func5( interface_ref* pCaller, int iVal, const std::string& sVal, long lVal, float fVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_V4(tschid, procid, inst, func, arg1, arg2, arg3, arg4) \
	do { \
		procedure_v4<arg1,arg2,arg3,arg4>* proc = new procedure_v4<arg1,arg2,arg3,arg4>; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, procid, proc ); \
	} while(0)

/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 �񵿱� ���ν����� ������ �Ű�����4�� �ڷ���

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_NM_V4(1, this, &ItemManager::Func5, int, const std::string&, long, float );
		}
		virtual ~ItemManager() {}

		bool Func5( interface_ref* pCaller, int iVal, const std::string& sVal, long lVal, float fVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_NM_V4(tschid, inst, func, arg1, arg2, arg3, arg4) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		procedure_v4<arg1,arg2,arg3,arg4>* proc = new procedure_v4<arg1,arg2,arg3,arg4>; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, hashval, proc ); \
	} while(0)

/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param procid �񵿱� ���ν����� ���� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 �񵿱� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg5 �񵿱� ���ν����� ������ �Ű�����5�� �ڷ���

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_V5(1, 1000, this, &ItemManager::Func6, int, const std::string&, long, float, double );
		}
		virtual ~ItemManager() {}

		bool Func6( interface_ref* pCaller, int iVal, const std::string& sVal, long lVal, float fVal, double dVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_V5(tschid, procid, inst, func, arg1, arg2, arg3, arg4, arg5) \
	do { \
		procedure_v5<arg1,arg2,arg3,arg4,arg5>* proc = new procedure_v5<arg1,arg2,arg3,arg4,arg5>; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, procid, proc ); \
	} while(0)

/**
	@def Ŭ������ �ɹ��Լ��� �񵿱� ���ν����� ����ϴ� ��ũ��
	@param tschid �񵿱� ���ν����� ����� �½�ũ�����ٷ��� ID
	@param procid �񵿱� ���ν����� ���� ID
	@param inst �񵿱� ���ν����� ����� �ɹ��Լ��� ������ �ִ� ��ü�� �ּҰ�
	@param func �񵿱� ���ν����� ����� �ɹ��Լ��� �ּҰ�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 �񵿱� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg5 �񵿱� ���ν����� ������ �Ű�����5�� �ڷ���

	@verbatim
	class ItemManager
	{
	public:
		ItemManager()
		{
			EXPORT_PROC_NM_V5(1, this, &ItemManager::Func6, int, const std::string&, long, float, double );
		}
		virtual ~ItemManager() {}

		bool Func6( interface_ref* pCaller, int iVal, const std::string& sVal, long lVal, float fVal, double dVal )
		{

			return true;
		}
	};
	@endverbatim
*/
#define EXPORT_PROC_NM_V5(tschid, inst, func, arg1, arg2, arg3, arg4, arg5) \
	do { \
		unsigned int hashval = Md5Hash::ToHash( util::pairval(#func, ":") ); \
		procedure_v5<arg1,arg2,arg3,arg4,arg5>* proc = new procedure_v5<arg1,arg2,arg3,arg4,arg5>; \
		proc->m_fn.assign(inst, func); \
		TaskSchedulerFactory::RegProcedure( tschid, hashval, proc ); \
	} while(0)

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid �񵿱����ν��� ����ID
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_V0( 1000, Func1, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_V0(procid, func, arg_size) \
	inline bool proc_##func(interface_ref* pCaller) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		return TaskSchedulerFactory::CallProcedure( procid, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_V0( Func1, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_NM_V0(func, arg_size) \
	inline bool proc_##func(interface_ref* pCaller) \
	{ \
		static unsigned int hashval = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		return TaskSchedulerFactory::CallProcedure( hashval, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid �񵿱����ν��� ����ID
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_V1( 1000, Func2, int, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_V1(procid, func, type1, arg_size) \
	inline bool proc_##func(interface_ref* pCaller, type1 arg1) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		*pBuf << arg1; \
		return TaskSchedulerFactory::CallProcedure( procid, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_NM_V1( Func2, int, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_NM_V1(func, type1, arg_size) \
	inline bool proc_##func(interface_ref* pCaller, type1 arg1) \
	{ \
		static unsigned int hashval = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		*pBuf << arg1; \
		return TaskSchedulerFactory::CallProcedure( hashval, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid �񵿱����ν��� ����ID
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_V2( 1000, Func3, int, const std::string&, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_V2(procid, func, type1, type2, arg_size) \
	inline bool proc_##func(interface_ref* pCaller, type1 arg1, type2 arg2) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		*pBuf << arg1 << arg2; \
		return TaskSchedulerFactory::CallProcedure( procid, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_NM_V2( Func3, int, const std::string&, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_NM_V2(func, type1, type2, arg_size) \
	inline bool proc_##func(interface_ref* pCaller, type1 arg1, type2 arg2) \
	{ \
		static unsigned int hashval = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		*pBuf << arg1 << arg2; \
		return TaskSchedulerFactory::CallProcedure( hashval, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid �񵿱����ν��� ����ID
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_V3( 1000, Func4, int, const std::string&, long, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_V3(procid, func, type1, type2, type3, arg_size) \
	inline bool proc_##func(interface_ref* pCaller, type1 arg1, type2 arg2, type3 arg3) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		*pBuf << arg1 << arg2 << arg3; \
		return TaskSchedulerFactory::CallProcedure( procid, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_NM_V3( Func4, int, const std::string&, long, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_NM_V3(func, type1, type2, type3, arg_size) \
	inline bool proc_##func(interface_ref* pCaller, type1 arg1, type2 arg2, type3 arg3) \
	{ \
		static unsigned int hashval = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		*pBuf << arg1 << arg2 << arg3; \
		return TaskSchedulerFactory::CallProcedure( hashval, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid �񵿱����ν��� ����ID
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 �񵿱� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_V4( 1000, Func5, int, const std::string&, long, float, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_V4(procid, func, type1, type2, type3, type4, arg_size) \
	inline bool proc_##func(interface_ref* pCaler, type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		*pBuf << arg1 << arg2 << arg3 << arg4; \
		return TaskSchedulerFactory::CallProcedure( procid, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 �񵿱� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_NM_V4( Func5, int, const std::string&, long, float, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_NM_V4(func, type1, type2, type3, type4, arg_size) \
	inline bool proc_##func(interface_ref* pCaler, type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
	{ \
		static unsigned int hashval = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		*pBuf << arg1 << arg2 << arg3 << arg4; \
		return TaskSchedulerFactory::CallProcedure( hashval, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param procid �񵿱����ν��� ����ID
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 �񵿱� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg5 �񵿱� ���ν����� ������ �Ű�����5�� �ڷ���
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_V5( 1000, Func6, int, const std::string&, long, float, double, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_V5(procid, func, type1, type2, type3, type4, type5, arg_size) \
	inline bool proc_##func(interface_ref* pCaller, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5) \
	{ \
		Buffer* pBuf = new Buffer(arg_size); \
		*pBuf << arg1 << arg2 << arg3 << arg4 << arg5; \
		return TaskSchedulerFactory::CallProcedure( procid, pCaller, pBuf ); \
	}

/**
	@def �񵿱� ���ν����� ��ϵ� �Լ��� Ŭ������ �ɹ��Լ��� �����ϴ� ��ũ��
	@param func �񵿱� ���ν����� ȣ���ϴ� �ɹ��Լ��� �̸�
	@param arg1 �񵿱� ���ν����� ������ �Ű�����1�� �ڷ���
	@param arg2 �񵿱� ���ν����� ������ �Ű�����2�� �ڷ���
	@param arg3 �񵿱� ���ν����� ������ �Ű�����3�� �ڷ���
	@param arg4 �񵿱� ���ν����� ������ �Ű�����4�� �ڷ���
	@param arg5 �񵿱� ���ν����� ������ �Ű�����5�� �ڷ���
	@param arg_size �񵿱����ν����� ���޵� �Ű������� ������ ������ ũ��

	@verbatim
	class PlayerManager
	{
	public:
		PlayerManager()
		{
			
		}
		virtual ~PlayerManager() {}

		IMPORT_PROC_NM_V5( Func6, int, const std::string&, long, float, double, 1024 );
	};
	@endverbatim
*/
#define IMPORT_PROC_NM_V5(func, type1, type2, type3, type4, type5, arg_size) \
	inline bool proc_##func(interface_ref* pCaller, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5) \
	{ \
		static unsigned int hashval = Md5Hash::ToHash( ":"#func ); \
		Buffer* pBuf = new Buffer(arg_size); \
		*pBuf << arg1 << arg2 << arg3 << arg4 << arg5; \
		return TaskSchedulerFactory::CallProcedure( hashval, pCaller, pBuf ); \
	}

#define IMPORT_COMMAND_V0( handler ) \
	IMPORT_PROC_NM_V0( handler, 1024 ); \
	bool handler##(interface_ref* pCaller )
#define IMPORT_COMMAND_V1( handler ) \
	IMPORT_PROC_NM_V1( handler, const std::string&, 1024 ); \
	bool handler##(interface_ref* pCaller, const std::string& )
#define IMPORT_COMMAND_V2( handler ) \
	IMPORT_PROC_NM_V2( handler, const std::string&, const std::string&, 1024 ); \
	bool handler##(interface_ref* pCaller, const std::string&, const std::string& )
#define IMPORT_COMMAND_V3( handler ) \
	IMPORT_PROC_NM_V3( handler, const std::string&, const std::string&, const std::string&, 1024 ); \
	bool handler##(interface_ref* pCaller, const std::string&, const std::string&, const std::string& )
#define IMPORT_COMMAND_V4( handler ) \
	IMPORT_PROC_NM_V4( handler, const std::string&, const std::string&, const std::string&, const std::string&, 1024 ); \
	bool handler##(interface_ref* pCaller, const std::string&, const std::string&, const std::string&, const std::string& )
#define IMPORT_COMMAND_V5( handler ) \
	IMPORT_PROC_NM_V5( handler, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, 1024 ); \
	bool handler##(interface_ref* pCaller, const std::string&, const std::string&, const std::string&, const std::string&, const std::string& )
