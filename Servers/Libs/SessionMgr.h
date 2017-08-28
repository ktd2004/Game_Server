#pragma once

#include "Session.h"
#include "AuthKey.h"


class SessionManager
{
public:
	typedef std::map<UID, SessionPtr>	SessionMap;

	struct KickReasonInfo
	{
		time_t				m_tTime;	// kick 요청 시간
		NetLinkPtr			m_spLink;
		int32				m_iReason;
		const std::string	m_sFunc;

		KickReasonInfo( time_t tTime, NetLinkPtr spLink, int32 iReason, const std::string& sFunc )
			: m_tTime(tTime)
			, m_spLink(spLink)
			, m_iReason(iReason)
			, m_sFunc(sFunc) {}

		KickReasonInfo( const KickReasonInfo& r )
			: m_tTime(r.m_tTime)
			, m_spLink(r.m_spLink)
			, m_iReason(r.m_iReason)
			, m_sFunc(r.m_sFunc) {}
	};
	typedef std::list<KickReasonInfo>	KickList;

private:
	SessionMap		m_SessionMap;
	SessionMap		m_AuthMap;
	KickList		m_KickList;
	int32			m_iTaskSchedulerID;

protected:
	SessionManager();
	SessionManager( const SessionManager& r );
	virtual ~SessionManager();

public:
	static SessionManager* instance();

	// 세션 맵
	SessionPtr	Reg( UID iUID, NetLinkPtr spLink, uint32 iAuthKey=0 );
	SessionPtr	Reg( SessionPtr spSession );
	SessionPtr	Reg( const Session& r, NetLinkPtr spLink );
	bool		Unreg( UID iUID );
	SessionPtr	Get( UID iUID );
	void		SetLink( UID iUID, NetLinkPtr spLink );
	int32		GetCount();
	
	// 인증 맵
	SessionPtr	Reg3A( UID iUID, NetLinkPtr spLink, uint32 iAuthKey = 0 );
	SessionPtr	Reg3A( SessionPtr spSession );
	bool		Unreg3A( UID iUID );
	SessionPtr	Get3A( UID iUID );
	int32		GetCount3A();

	// 세션 맵으로 이동
	SessionPtr	Authentication( UID iUID );
	// 인증 맵으로 이동
	SessionPtr	Unauthentication( UID iUID );
	// 세션 맵 및 인증 맵에서 동시 삭제
	bool		UnregAll( UID iUID );
	// 세션키 비교후 세션 맵 및 인증 맵에서 동시 삭제
	bool		CompareAuthKeyUnregAll(UID iUID, uint32 iAuthKey, int32 iReason, const char* pFunc);

	SessionPtr	IsExists( UID iUID );
	void		Kick( NetLinkPtr spLink, int32 iReason, const std::string& sFunc );
	void		Kick( UID iUID, int32 iReason, const std::string& sFunc );
	void		StartKickTimer( int32 iTaskSchedulerID, uint32 iMsec = 5000 );
	bool		OnKickTimer( void* pInst );
	size_t		Broadcast( Buffer* pBuffer );
	SessionManager::SessionMap& GetSessionMap();
	SessionManager::SessionMap& GetAuthMap();
};
#define SESSION_MGR()	SessionManager::instance()
