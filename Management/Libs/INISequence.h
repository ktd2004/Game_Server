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
#pragma once

#include <DataTypes.h>

// 파일에 저장된 레코드의 순서를 보장하기 위해서 로딩된 순서대로 저장되어 있다.
class INISequence
{
public:
	struct Record
	{
		std::string sSID;
		std::string sSection;
		std::string sKey;
		std::string sValue;
		std::string sComment;
		Record(
			const std::string& _sSID,
			const std::string& _sSection,
			const std::string& _sKey,
			const std::string& _sValue,
			const std::string& _sComment)
			: sSID(_sSID)
			, sSection(_sSection)
			, sKey(_sKey)
			, sValue(_sValue)
			, sComment(_sComment)
		{
		}
		Record(const Record& r)
			: sSID(r.sSID)
			, sSection(r.sSection)
			, sKey(r.sKey)
			, sValue(r.sValue)
			, sComment(r.sComment)
		{
		}
	};

	typedef std::map<int32, Record>	INIMap;
	INIMap		m_INIMap;

public:
	INISequence();
	virtual ~INISequence();
	bool	Load( const std::string& sFileName, bool bDebug = false );
};