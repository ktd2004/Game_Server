#pragma once

#include <Proactor.h>
#include "AAAConst.h"
#include "AAATypes.h"

PTR(Master);
/** 
	게이트웨이에서 마스터 정보를 관리.
*/
class Master : public single_ref
{
	friend class MasterMgr;
private:
	MID				m_iMID;
	bool			m_bBlock;
	NetLinkPtr		m_spMasterLink;

public:
	Master();
	virtual ~Master();
	MID			GetID( void );
	void		SetID( MID iMID );
	void		SetBlock( bool bBlock );
	bool		IsBlock( bool bBlock );
	void		SetLink( NetLinkPtr spLink );
	NetLinkPtr	GetLink( void );
};

/** 
	인증서버에서 월드별로 마스터 정보를 관리.
*/
class MasterMgr
{
public:
	typedef	std::map<MID, MasterPtr>	MasterMap;

private:
	MasterMap	m_MasterMap;

private:
	MasterMgr();
	MasterMgr(const MasterMgr& r) {}
	virtual ~MasterMgr();

public:
	static MasterMgr* instance( void );
	MasterMap*	GetMasterMap( void );
	int32		GetMasterCount( void );

	MasterPtr	Add( MID iMID, NetLinkPtr spLink );
	MasterPtr	Get( MID iMID );

	bool		Remove( MID iMID );
	void		RemoveAll( void );
};
#define MASTER_MGR()		MasterMgr::instance()
