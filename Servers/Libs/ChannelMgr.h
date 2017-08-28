#pragma once

#include <NetLink.h>
#include <Ptr.h>
#include "AAATypes.h"
#include "Session.h"

PTR(NetLink);
PTR(Channel);
/** 
	게이트웨이에 연결된 채널 정보를 관리.
*/
class Channel : public single_ref
{
	friend class ChannelMgr;

private:
	CID				m_iCID;
	uint32			m_iCurrentUser;
	NetLinkPtr		m_spChannelLink;
	uint32			m_iMaxClient;
	bool			m_bBlock;

public:
	Channel();
	virtual ~Channel();

	void				SetID( CID iCID );
	CID					GetID( void );
	void				SetMaxClient( uint32 iMax );
	uint32				GetMaxClient( void );
	void				SetCurrentUser( uint32 iCurrent );
	uint32				GetCurrentUser( void );
	void				SetBlock( bool bBlock );
	bool				IsBlock( void );
	void				SetLink(NetLinkPtr spLink);
	NetLinkPtr			GetLink(void);
};

/** 
	게이트웨이가 연결한 채널 정보를 관리.
*/
class ChannelMgr
{
public:
	typedef std::map<CID, ChannelPtr>	ChannelMap;

protected:
	ChannelMap		m_ChannelMap;
	uint32			m_iTotalUser;
	CID				m_iFewCID;

private:
	ChannelMgr();
	ChannelMgr(const ChannelMgr& r) {}
	virtual ~ChannelMgr();

public:
	static ChannelMgr* instance();
	ChannelMap*		GetChannelMap();
	ChannelPtr		Get( CID iCID );
	ChannelPtr		Add( CID iCID, NetLinkPtr spLink );
	bool			Remove( CID iCID );
	void			RemoveAll();
	CID				UpdateLoadBalancing();
	void			SetFewChannelID( CID iCID );
	CID				GetFewChannelID();
	int32			GetCount();
};
#define CHANNEL_MGR()		ChannelMgr::instance()