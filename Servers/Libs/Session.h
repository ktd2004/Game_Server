#pragma once

#include <Ptr.h>
#include "AAATypes.h"
#include <TimeUtil.h>
#include <NetLink.h>
#include <Buffer.h>

class Buffer;
PTR(NetLink);
PTR(Session)
class Session : public single_ref
{
private:
	friend Buffer& operator<<( Buffer& pk, const Session& r );
	friend Buffer& operator>>( Buffer& pk, Session& r );
	friend class SessionManager;

	interface_ref*		m_pUserData;

public:
	enum eSessionState
	{
		eAUTHENTICATE,	// ���� ó�� ���� ��
		eVERIFICATION,	// ���� Ȯ�� �Ϸ�
	};
	
public:
	Session();
	Session( UID iUID );
	Session( const Session& r );
	Session( const SessionPtr& r );
	virtual ~Session();

	void				SetSessionData( const Session& r );
	UID					GetUID( void );
	void				SetLink( NetLinkPtr spLink );
	NetLinkPtr			GetLink( void );
	void				SetAuthKey( uint32 iAuthKey );
	uint32				GetAuthKey( void );
	void				SetAccount( const std::string& sAccount );
	const std::string&	GetAccount( void );
	void				SetAuthTime( void );
	time_t				GetAuthTime( void );
	void				SetLoginedInfo( GID iAID, MID iMID, CID iCID );
	GID					GetGID();
	MID					GetMasterID( void );
	CID					GetChannelID( void );
	void				SetState( int32 iState );
	int32				GetState( void );
	void				SetLastTick( void );
	time_t				GetLastTick( void );
	void				SetLastSave( void );
	time_t				GetLastSave( void );
	time_t				GetLoginTime( void );
	bool				IsAuthencation( void );
	void				UserData( interface_ref* pUserData );
	interface_ref*		UserData( void );

	// override playersession
	virtual bool		IsUpdated( void );
	virtual bool		Save( bool bClosed=false );

public:
	NetLinkPtr	m_spLink;
	UID			m_iUID;
	uint32		m_iAuthKey;
	std::string m_sAccount;
	time_t		m_tAuthTime;	// ������ ������ �ð�
	int32		m_iState;		// ���� ���� ����
	time_t		m_tLastTick;	// heartbit
	time_t		m_tLastSave;	// ���� �ð�

	// ���� �������� �����Ǵ� �����ؾ� �� �� �������� ID
	GID			m_iGID;
	MID			m_iMasterID;
	CID			m_iChannelID;
};

Buffer& operator<<( Buffer& pk, const Session& r );
Buffer& operator>>( Buffer& pk, Session& r );
