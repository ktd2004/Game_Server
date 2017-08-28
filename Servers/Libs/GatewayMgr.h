#pragma once

#include <NetLink.h>
#include <Buffer.h>
#include <MacroFunc.h>
#include "AAATypes.h"
#include "Session.h"

PTR(Gateway);
/** 
	����Ʈ���� ����
*/
class Gateway : public single_ref
{
	friend class GatewayMgr;
public:
	typedef std::map<UID, Session>	LoginSessionMap;

private:
	GID					m_iGID;
	NetLinkPtr			m_spGatewayLink;
	LoginSessionMap		m_LoginMap;

public:
	Gateway();
	Gateway(GID iGID);
	virtual ~Gateway();

	GID					GetID( void );
	void				SetID( GID iGID );

	void				SetLink( NetLinkPtr spLink );
	NetLinkPtr			GetLink( void );
	bool				Add(const Session& r);
	void				Remove(UID iUID);
	bool				IsExists(UID iUID);
	Session*			GetSession(UID iUID);
	int32				GetCount( void );
};

/** 
	�����Ϳ��� ����� ����Ʈ���̸� �����ϱ� ���ؼ� ���.
*/
class GatewayMgr
{
public:
	typedef std::map<GID, GatewayPtr>	GatewayMap;

private:
	GatewayMap		m_GatewayMap;

	GatewayMgr();
	GatewayMgr(const GatewayMgr& r);
	virtual ~GatewayMgr();

public:
	static GatewayMgr* instance();
	GatewayMap*		GetGatewayMap();
	GatewayPtr		Get( GID iGID );
	NetLinkPtr		GetLink( GID iGID );
	GatewayPtr		Add( GID iGID, NetLinkPtr spLink );
	bool			Add( GatewayPtr spGateway, NetLinkPtr spLink );
	bool			Remove( GID iAID );
	int32			GetCount( void );
	GatewayPtr		GetByUID( UID iUID );
	Session*		GetSession( UID iUID );
	int32			GetSessionCount( void );
};

#define GATEWAY_MGR()		GatewayMgr::instance()
