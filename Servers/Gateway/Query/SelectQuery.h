#pragma once

#include "AAATypes.h"
#include "QueryAbstract.h"
#include "ClientHandler.h"

class SelectAccount : public QueryAbstract_v3<ClientHandler, SelectAccount>
{
public:
	struct AccInfo
	{
		std::string		m_sNickname;
		uint16			m_iGender;
		std::string		m_sBirth;
		std::string		m_sPhoto;

		uint8			m_iPlatform;
		uint32			m_iMarketID;
		std::string		m_sPushID;
		uint16			m_iPushOpt;

		std::string		m_sRecommendCode;
		std::string		m_sToken;
		time_t			m_tExpireTime;

		uint16			m_iLogined;
		MID				m_iMID;
		CID				m_iCID;
		uint16			m_iStatus;
		
		time_t			m_tStart;
		time_t			m_tEnd;

		AccInfo()
			: m_sNickname("")
			, m_iGender(0)
			, m_sBirth("0000-00-00")
			, m_sPhoto("")
			, m_sPushID("")
			, m_iPushOpt(1)
			, m_sRecommendCode("")
			, m_sToken("")
			, m_tExpireTime(0)
			, m_iLogined(0)
			, m_iMID(null_id)
			, m_iCID(null_id)
			, m_iStatus(0)
			, m_tStart(0)
			, m_tEnd(0) {}
	};

public:
	SelectAccount( SessionPtr spSession )
		: QueryAbstract_v3(spSession->GetLink())
		, m_spSession(spSession)
		, m_pResult(nullptr)
		, m_bExistAccount(false)
	{
		m_Sql = "SP_SelectAccount( ? )";
	}

	virtual ~SelectAccount()
	{
// 		safe_delete( m_pResult );
// 		m_spSession = nullptr;
	}

	virtual void OnSetParam( IDBCommand* pCmd )
	{
		if( pCmd )
		{
			pCmd->AddParam( m_spSession->GetUID() );
		}
	}

	virtual void OnQuery( IDBResult* pResult )
	{
		m_pResult = pResult;
		if (m_pResult)
		{
			IDBRecordset* pRst = m_pResult->FetchRecordset();
			if (pRst)
			{
				IDBRow* pRow = nullptr;
				while (pRow = pRst->FetchRow())
				{
					QueryResultHelper data(pRow);
					m_bExistAccount = true;

					m_AccInfo.m_sNickname = data[0];
					m_AccInfo.m_iGender = stouint32(data[1]);
					m_AccInfo.m_sBirth = data[2];
					m_AccInfo.m_sPhoto = data[3];
					m_AccInfo.m_iPlatform = stouint32(data[4]);
					m_AccInfo.m_iMarketID = stoint32(data[5]);
					m_AccInfo.m_sPushID = data[6];
					m_AccInfo.m_iPushOpt = stouint32(data[7]);
					m_AccInfo.m_sRecommendCode = data[8];
					m_AccInfo.m_sToken = data[9];
					m_AccInfo.m_tExpireTime = stouint32(data[10]);
					m_AccInfo.m_iLogined = stoint32(data[11]);
					m_AccInfo.m_iMID = stoint32(data[13]);
					m_AccInfo.m_iCID = stoint32(data[14]);
					m_AccInfo.m_iStatus = stouint32(data[15]);
					m_AccInfo.m_tStart = stouint32(data[16]);
					m_AccInfo.m_tEnd = stouint32(data[17]);
				}
			}
		}
	}

	UID			GetUID() { return m_spSession->GetUID(); }
	AccInfo*	GetInfo() { return &m_AccInfo; }
	bool		ExistAccount() { return m_bExistAccount; }

	SessionPtr	GetSession() { return m_spSession; }

private:
	SessionPtr	m_spSession;
	IDBResult*	m_pResult;
	AccInfo		m_AccInfo;
	bool		m_bExistAccount;
};
