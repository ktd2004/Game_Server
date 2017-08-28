#pragma once

#include <AAATypes.h>
#include <NetLink.h>
#include "Player/PlayerSession.h"


class WholeChat
{
	typedef std::list<PlayerSession*>	SessionList;

public:
	virtual ~WholeChat();
	static WholeChat* Instance();

	bool	Add( PlayerSession* pSession );
	bool	Remove( PlayerSession* pSession );

private:
	WholeChat() {}
	WholeChat( const WholeChat& r ) {}

private:
	SessionList		m_SessionList;
};
#define WHOLE_CHAT()	WholeChat::Instance()
