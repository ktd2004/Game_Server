#include "stdafx.h"
#include "GatewayMgr.h"

Gateway::Gateway()
	: m_iGID(0)
{

}

Gateway::Gateway(GID iGID)
	: m_iGID(iGID)
{

}

Gateway::~Gateway()
{
}

GID Gateway::GetID()
{
	return m_iGID;
}

void Gateway::SetID( GID iGID )
{
	m_iGID = iGID;
}

void Gateway::SetLink(NetLinkPtr spLink)
{
	m_spGatewayLink = spLink;
}

NetLinkPtr Gateway::GetLink()
{
	return m_spGatewayLink;
}

bool Gateway::Add(const Session& r)
{
	auto it = m_LoginMap.find(r.m_iUID);
	if (it != m_LoginMap.end())
	{
		it->second.SetSessionData(r);
		return true;
	}
	else
	{
		return m_LoginMap.insert(LoginSessionMap::value_type(r.m_iUID, r)).second;
	}
}

void Gateway::Remove(UID iUID)
{
	m_LoginMap.erase(iUID);
}

bool Gateway::IsExists(UID iUID)
{
	return m_LoginMap.count(iUID) > 0;
}

Session* Gateway::GetSession(UID iUID)
{
	auto it = m_LoginMap.find(iUID);
	if (it != m_LoginMap.end())
	{
		return &it->second;
	}

	return nullptr;
}

int32 Gateway::GetCount(void)
{
	return m_LoginMap.size();
}

GatewayMgr::GatewayMgr()
{

}

GatewayMgr::GatewayMgr( const GatewayMgr& r )
{

}

GatewayMgr::~GatewayMgr()
{

}

GatewayMgr* GatewayMgr::instance()
{
	static GatewayMgr sInstance;
	return &sInstance;
}

GatewayMgr::GatewayMap* GatewayMgr::GetGatewayMap()
{
	return &m_GatewayMap;
}

GatewayPtr GatewayMgr::Get( GID iGID )
{
	notfound_map_ret(GatewayMap, it, m_GatewayMap, iGID, nullptr);
	return it->second;
}

GatewayPtr GatewayMgr::Add( GID iGID, NetLinkPtr spLink )
{
	GatewayPtr spGateway;
	GatewayMap::iterator it = m_GatewayMap.find(iGID);
	if (it != m_GatewayMap.end())
	{
		spGateway = it->second;
		spGateway->SetLink(spLink);
	}
	else
	{
		spGateway = new Gateway(iGID);
		spGateway->SetLink(spLink);
		m_GatewayMap.insert(GatewayMap::value_type(iGID, spGateway));
	}

	return spGateway;
}

bool GatewayMgr::Add(GatewayPtr spGateway, NetLinkPtr spLink)
{
	isnull_ret(spGateway, false);

	GatewayMap::iterator it = m_GatewayMap.find(spGateway->GetID());
	if (it != m_GatewayMap.end())
	{
		spGateway->SetLink(spLink);
		it->second = spGateway;
	}
	else
	{
		spGateway->SetLink(spLink);
		m_GatewayMap.insert(GatewayMap::value_type(spGateway->GetID(), spGateway));
	}

	return true;
}

bool GatewayMgr::Remove( GID iGID )
{
	notfound_map_ret(GatewayMap, it, m_GatewayMap, iGID, false);
	it->second->SetLink(nullptr);
	return true;
}

int32 GatewayMgr::GetCount()
{
	return m_GatewayMap.size();
}

GatewayPtr GatewayMgr::GetByUID(UID iUID)
{
	for (auto it = m_GatewayMap.begin(); it != m_GatewayMap.end(); ++it)
	{
		if (it->second->IsExists(iUID))
		{
			return it->second;
		}
	}

	return nullptr;
}

Session* GatewayMgr::GetSession(UID iUID)
{
	for (auto it = m_GatewayMap.begin(); it != m_GatewayMap.end(); ++it)
	{
		Session* p = it->second->GetSession(iUID);
		if ( p )
		{
			return p;
		}
	}

	return nullptr;
}

NetLinkPtr GatewayMgr::GetLink(GID iGID)
{
	GatewayPtr spGateway = Get(iGID);
	if (spGateway)
		return spGateway->m_spGatewayLink;
	return nullptr;
}

int32 GatewayMgr::GetSessionCount(void)
{
	int32 iSum = 0;
	for (auto it = m_GatewayMap.begin(); it != m_GatewayMap.end(); ++it)
	{
		iSum += it->second->GetCount();
	}

	return iSum;
}
