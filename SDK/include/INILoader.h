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
	�������� ������ �ε��Ͽ� �����ϴ� Ŭ����.
*/
class INILoader
{
public:
	typedef std::map<std::string, std::string>	KeyMap;
	typedef std::map<std::string, KeyMap>		SectionMap;

private:
	SectionMap		m_Sections;			///< ���������� ����
	bool			m_bLoaded;			///< �������� ������ �ε��ߴ��� ����
	std::string		m_sPath;			///< �������� ������ ���

public:
	INILoader();
	INILoader( const INILoader& copy );
	virtual ~INILoader(void);

public:
	static INILoader* instance();

	/**
		�������������� �ε��Ѵ�.
		@param sPath �������������� ���
	*/
	bool				Load( const std::string& sPath );
	/**
		�������������� �ε������ ����.
	*/
	bool				IsLoaded( void );
	/**
		���� ������ ��� �ִ� �����̳ʸ� �����Ѵ�.
		@param sSection ���Ǹ�
	*/
	KeyMap&				GetKeyMap( const std::string& sSection );
	/**
		��� ���� ������ ��� �ִ� �����̳ʸ� �����Ѵ�.
	*/
	SectionMap&			GetSectionMap( void );
	/**
		������ Ű���� ��ȸ�Ѵ�.
		@param Section ���Ǹ�
		@param Key Ű��
		@param Default ���ǵ� Ű���� ������ ��ȯ�� ��
	*/
	const std::string&	GetString( const std::string& Section, const std::string& Key, const std::string& Default );
	/**
		������ Ű���� ��ȸ�Ѵ�.
		@param Section ���Ǹ�
		@param Key Ű��
		@param Default ���ǵ� Ű���� ������ ��ȯ�� ��
	*/
	int32				GetInt( const std::string& Section, const std::string& Key, int32 Default );
	/**
		������ Ű���� ��ȸ�Ѵ�.
		@param Section ���Ǹ�
		@param Key Ű��
		@param Default ���ǵ� Ű���� ������ ��ȯ�� ��
	*/
	uint64				GetUint64( const std::string& Section, const std::string& Key, uint64 Default );
	/**
		������ Ű���� ��ȸ�Ѵ�.
		@param Section ���Ǹ�
		@param Key Ű��
		@param Default ���ǵ� Ű���� ������ ��ȯ�� ��
	*/
	double				GetDouble( const std::string& Section, const std::string& Key, const double Default );
	/**
		������ Ű���� ��ȸ�Ѵ�.
		@param Section ���Ǹ�
		@param Key Ű��
		@param Default ���ǵ� Ű���� ������ ��ȯ�� ��
	*/
	bool				GetBool( const std::string& Section, const std::string& Key, const bool Default );
	/**
		������ Ű���� �����Ѵ�.
		@param Section ���Ǹ�
		@param Key Ű��
		@param Value ������ ��
	*/
	bool				SetString( const std::string& Section, const std::string& Key, const std::string& Value );
	/**
		������ Ű���� �����Ѵ�.
		@param Section ���Ǹ�
		@param Key Ű��
		@param Value ������ ��
	*/
	bool				SetInt( const std::string& Section, const std::string& Key, const int32 iValue );
	/**
		������ Ű���� �����Ѵ�.
		@param Section ���Ǹ�
		@param Key Ű��
		@param Value ������ ��
	*/
	bool				SetDouble( const std::string& Section, const std::string& Key, const double dValue );
	/**
		������ Ű���� �����Ѵ�.
		@param Section ���Ǹ�
		@param Key Ű��
		@param Value ������ ��
	*/
	bool				SetBool( const std::string& Section, const std::string& Key, const bool bValue );
	/**
		��� ����Ÿ�� ���Ϸ� �����Ѵ�.
		@param sFileName ���ϸ�
	*/
	bool				Save( const std::string& sFileName );
	/**
		���ǿ� Ű���� �����ϴ� �����Ѵ�.
		@param Section ���Ǹ�
		@param Key Ű��
	*/
	bool				IsExists( const std::string& Section, const std::string& Key );
	/**
		��� ���������� ����Ѵ�.
	*/
	void				Dump(void);
};

#define Env_i	INILoader::instance()->GetInt
#define Env_s	INILoader::instance()->GetString
#define Env_b	INILoader::instance()->GetBool
#define Env_i64	INILoader::instance()->GetUint64