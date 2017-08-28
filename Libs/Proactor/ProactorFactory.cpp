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
//<
#include <ThreadFactory.h>
//<
#include "ProactorImpl.h"

namespace
{
	struct ProactorEntity
	{
		bool						m_bShutdown;
		ProactorImpl*				m_pProactor;

		ProactorEntity()
			: m_pProactor(nullptr)
			, m_bShutdown(false)
		{}
		ProactorEntity(const ProactorEntity& r)
			: m_pProactor(nullptr)
			, m_bShutdown(false)
		{}
		ProactorEntity(ProactorImpl* pProactor)
			: m_pProactor(pProactor)
			, m_bShutdown(false)
		{}
		virtual ~ProactorEntity()
		{
			safe_delete( m_pProactor );
		}
	};

	class ProactorManager
	{
	private:
		// terminate
		typedef std::list<spRef<ProactorEntity> > ProactorEntityList;
		typedef std::map<int, ProactorEntityList > ProactorMap;
		ProactorMap		m_ProactorMap;
		unsigned int	m_iProactorID;

	public:
		ProactorManager()
			: m_iProactorID(1)
		{
		}
		virtual ~ProactorManager()
		{
		}

		Proactor* Create( int iPriority, unsigned short iThreadCnt, unsigned short iBindCPU )
		{
			ProactorImpl* pProactor = new ProactorImpl;
			pProactor->ID( m_iProactorID );

			ProactorEntityList* pEntityList = nullptr;
			ProactorMap::iterator it = m_ProactorMap.find( iPriority );
			if ( it != m_ProactorMap.end() )
			{
				pEntityList = &it->second;
			}
			else
			{
				m_ProactorMap.insert( ProactorMap::value_type(iPriority, ProactorEntityList()) );
				pEntityList = &m_ProactorMap[iPriority];
			}

			ProactorEntity* pEntity = new ProactorEntity(pProactor);
			pEntityList->push_back( pEntity );

			pProactor->Create( iBindCPU, iThreadCnt );
			m_iProactorID++;
			return (Proactor*)pProactor;
		}

		ProactorEntity* Get( unsigned int iProactorID )
		{
			foreach_do( ProactorMap, it, m_ProactorMap )
			{
				ProactorEntityList& lst = it->second;
				foreach_do( ProactorEntityList, ite, lst )
				{
					ProactorEntity* pEntity = *ite;
					if ( pEntity->m_pProactor->ID() == iProactorID )
					{
						return *ite;
					}
				}
			}

			return nullptr;
		}

		bool Start( unsigned int iProactorID )
		{
			if ( iProactorID != 0 )
			{
				ProactorEntity* pEntity = Get( iProactorID );
				if ( pEntity )
				{
					pEntity->m_pProactor->Resume();
					return true;
				}
			}
			else
			{
				foreach_do ( ProactorMap, it, m_ProactorMap )
				{
					ProactorEntityList& lst = it->second;
					foreach_do ( ProactorEntityList, ite, lst )
					{
						ProactorEntity* pEntity = *ite;
						pEntity->m_pProactor->Resume();
					}
				}

				return true;
			}

			return false;
		}

		bool Stop( unsigned int iProactorID )
		{
			if ( iProactorID != 0 )
			{
				ProactorEntity* pEntity = Get( iProactorID );
				if ( pEntity )
				{
					if ( pEntity->m_bShutdown )
						return true;

					if ( pEntity->m_pProactor->OnKickAll() )
					{
						pEntity->m_pProactor->Finalize();
						pEntity->m_pProactor->Wait();
						pEntity->m_bShutdown = true;
						return true;
					}
				}

				return false;
			}
			else
			{
				foreach_do ( ProactorMap, it, m_ProactorMap )
				{
					ProactorEntityList& lst = it->second;
					foreach_do ( ProactorEntityList, ite, lst )
					{
						ProactorEntity* pEntity = *ite;
						if ( pEntity->m_bShutdown )
							continue;

						if ( pEntity->m_pProactor->OnKickAll() )
						{
							pEntity->m_pProactor->Finalize();
							pEntity->m_pProactor->Wait();
							pEntity->m_bShutdown = true;
						}
						else
						{
							return false;
						}
					}
				}
				return true;
			}
		}
	};
};

//<
namespace ProactorFactory
{
	ProactorManager g_ProactorMgr;

	Proactor* Create( int iPriority, unsigned short iBindCPU, unsigned short iThreadCnt )
	{
		return g_ProactorMgr.Create( iPriority, iBindCPU, iThreadCnt );
	}

	Proactor* Get( unsigned int iProactorID )
	{
		ProactorEntity* pEntity = g_ProactorMgr.Get( iProactorID );
		return pEntity ? pEntity->m_pProactor : nullptr;
	}

	bool Start( unsigned int iProactorID )
	{
		return g_ProactorMgr.Start( iProactorID );
	}

	bool Stop( unsigned int iProactorID )
	{
		return g_ProactorMgr.Stop( iProactorID );
	}
};