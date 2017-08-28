///< ServerApp를 상속받은 클래스에 포함되는 버전번호 파일

#ifndef __VersionNum_h__
#define __VersionNum_h__

#pragma once

#include <StringUtil.h>

// Version 양식
// major.middle.minor
// major : 중요 시스템 및 대규모 컨텐츠 업데이트
// middle : 컨텐츠 업데이트
// minor : 버그 패치

class Version
{
public:
	Version()
		: m_iMajor(0), m_iMiddle(0), m_iMinor(0) {}

	Version( const std::string& sVersion )
	{
		Parse( sVersion, m_iMajor, m_iMiddle, m_iMinor );
	}

	Version( const Version& version )
	{
		m_iMajor = version.m_iMajor;
		m_iMiddle = version.m_iMiddle;
		m_iMinor = version.m_iMinor;
	}
	
	virtual ~Version() {}

	bool operator==( const Version& version )
	{
		if( (m_iMajor == version.m_iMajor) &&
			(m_iMiddle == version.m_iMiddle) &&
			(m_iMinor == version.m_iMinor) )
		{
			return true;
		}
		
		return false;
	}

	bool operator==( const std::string& sVersion )
	{
		uint16 iMajor, iMiddle, iMinor;
		Parse( sVersion, iMajor, iMiddle, iMinor );
		
		if( (m_iMajor == iMajor) &&
			(m_iMiddle == iMiddle) &&
			(m_iMinor == iMinor) )
		{
			return true;
		}
		
		return false;
	}

	bool operator!=( const Version& version )
	{
		if( (m_iMajor != version.m_iMajor) ||
			(m_iMiddle != version.m_iMiddle) ||
			(m_iMinor != version.m_iMinor) )
		{
			return true;
		}

		return false;
	}

	bool operator!=( const std::string& sVersion )
	{
		uint16 iMajor, iMiddle, iMinor;
		Parse( sVersion, iMajor, iMiddle, iMinor );
		
		if( (m_iMajor != iMajor) ||
			(m_iMiddle != iMiddle) ||
			(m_iMinor != iMinor) )
		{
			return true;
		}
		
		return false;
	}
	
	bool operator<( const std::string& sVersion )
	{
		uint16 iMajor, iMiddle, iMinor;
		Parse( sVersion, iMajor, iMiddle, iMinor );

		if( m_iMajor == iMajor )
		{
			if( m_iMiddle == iMiddle )
			{
				if( m_iMinor < iMinor )
				{
					return true;
				}	
			}
			else
			{
				if( m_iMiddle < iMiddle )
				{
					return true;
				}
			}
			
			return false;
		}
		else
		{
			if( m_iMajor < iMajor )
			{
				return true;
			}

			return false;
		}
	}
	
	bool operator<( Version& version )
	{
		return *this < version.ToString();
	}
	
	bool operator<=( const std::string& sVersion )
	{
		if( *this == sVersion )
		{
			return true;
		}
		
		return *this < sVersion;
	}
	
	bool operator<=( Version& version )
	{
		return *this <= version.ToString();
	}
	
	bool operator>( const std::string& sVersion )
	{
		uint16 iMajor, iMiddle, iMinor;
		Parse( sVersion, iMajor, iMiddle, iMinor );
		
		if( m_iMajor == iMajor )
		{
			if( m_iMiddle == iMiddle )
			{
				if( m_iMinor > iMinor )
				{
					return true;
				}	
			}
			else
			{
				if( m_iMiddle > iMiddle )
				{
					return true;
				}
			}
			
			return false;
		}
		else
		{
			if( m_iMajor > iMajor )
			{
				return true;
			}
			
			return false;
		}
	}
	
	bool operator>( Version& version )
	{
		return *this > version.ToString();
	}
	
	bool operator>=( const std::string& sVersion )
	{
		if( *this == sVersion )
		{
			return true;
		}
		
		return *this > sVersion;
	}

	bool operator>=( Version& version )
	{
		return *this >= version;
	}
	
	void operator=(const std::string& sVersion)
	{
		Set(sVersion);
	}

	std::string	ToString()
	{
		char buf[16] = {0};
		sprintf_s( buf, "%u.%u.%u", m_iMajor, m_iMiddle, m_iMinor );
		return buf;
	}

	inline void Set( const std::string& sVersion )
	{
		Parse( sVersion, m_iMajor, m_iMiddle, m_iMinor );
	}

private:
	inline void Parse( const std::string& str, uint16& iMajor, uint16& iMiddle, uint16& iMinor )
	{
		stringlist slist;
		util::tokenizer( str, slist, '.' );

		if( slist.size() != 3 )
		{
			// error
			iMajor = iMiddle = iMinor = 0;
		}
		else
		{
			iMajor = stouint32( slist.front() );
			slist.pop_front();

			iMiddle = stouint32( slist.front() );
			slist.pop_front();

			iMinor = stouint32( slist.front() );
			slist.clear();
		}
	}

private:
	uint16		m_iMajor;
	uint16		m_iMiddle;
	uint16		m_iMinor;
};

#endif
