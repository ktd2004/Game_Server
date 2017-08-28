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

/** 
	서버구성 파일을 로딩하여 관리하는 클래스.
*/
class INILoader
{
public:
	typedef std::map<std::string, std::string>	KeyMap;
	typedef std::map<std::string, KeyMap>		SectionMap;

private:
	SectionMap		m_Sections;			///< 섹션정보를 관리
	bool			m_bLoaded;			///< 서버구성 파일을 로딩했는지 여부
	std::string		m_sPath;			///< 서버구성 파일의 경로

public:
	INILoader();
	INILoader( const INILoader& copy );
	virtual ~INILoader(void);

public:
	static INILoader* instance();

	/**
		서버구성파일을 로딩한다.
		@param sPath 서버구성파일의 경로
	*/
	bool				Load( const std::string& sPath );
	/**
		서버구성파일이 로딩됬는지 여부.
	*/
	bool				IsLoaded( void );
	/**
		섹션 정보를 담고 있는 컨테이너를 참조한다.
		@param sSection 섹션명
	*/
	KeyMap&				GetKeyMap( const std::string& sSection );
	/**
		모든 섹션 정보를 담고 있는 컨테이너를 참조한다.
	*/
	SectionMap&			GetSectionMap( void );
	/**
		섹션의 키값을 조회한다.
		@param Section 섹션명
		@param Key 키값
		@param Default 정의된 키값이 없을때 반환할 값
	*/
	const std::string&	GetString( const std::string& Section, const std::string& Key, const std::string& Default );
	/**
		섹션의 키값을 조회한다.
		@param Section 섹션명
		@param Key 키값
		@param Default 정의된 키값이 없을때 반환할 값
	*/
	int32				GetInt( const std::string& Section, const std::string& Key, int32 Default );
	/**
		섹션의 키값을 조회한다.
		@param Section 섹션명
		@param Key 키값
		@param Default 정의된 키값이 없을때 반환할 값
	*/
	uint64				GetUint64( const std::string& Section, const std::string& Key, uint64 Default );
	/**
		섹션의 키값을 조회한다.
		@param Section 섹션명
		@param Key 키값
		@param Default 정의된 키값이 없을때 반환할 값
	*/
	double				GetDouble( const std::string& Section, const std::string& Key, const double Default );
	/**
		섹션의 키값을 조회한다.
		@param Section 섹션명
		@param Key 키값
		@param Default 정의된 키값이 없을때 반환할 값
	*/
	bool				GetBool( const std::string& Section, const std::string& Key, const bool Default );
	/**
		섹션의 키값을 설정한다.
		@param Section 섹션명
		@param Key 키값
		@param Value 저장할 값
	*/
	bool				SetString( const std::string& Section, const std::string& Key, const std::string& Value );
	/**
		섹션의 키값을 설정한다.
		@param Section 섹션명
		@param Key 키값
		@param Value 저장할 값
	*/
	bool				SetInt( const std::string& Section, const std::string& Key, const int32 iValue );
	/**
		섹션의 키값을 설정한다.
		@param Section 섹션명
		@param Key 키값
		@param Value 저장할 값
	*/
	bool				SetDouble( const std::string& Section, const std::string& Key, const double dValue );
	/**
		섹션의 키값을 설정한다.
		@param Section 섹션명
		@param Key 키값
		@param Value 저장할 값
	*/
	bool				SetBool( const std::string& Section, const std::string& Key, const bool bValue );
	/**
		모든 데이타를 파일로 저장한다.
		@param sFileName 파일명
	*/
	bool				Save( const std::string& sFileName );
	/**
		섹션에 키값이 존재하는 조사한다.
		@param Section 섹션명
		@param Key 키값
	*/
	bool				IsExists( const std::string& Section, const std::string& Key );
	/**
		모든 섹션정보를 출력한다.
	*/
	void				Dump(void);
};

#define Env_i	INILoader::instance()->GetInt
#define Env_s	INILoader::instance()->GetString
#define Env_b	INILoader::instance()->GetBool
#define Env_i64	INILoader::instance()->GetUint64