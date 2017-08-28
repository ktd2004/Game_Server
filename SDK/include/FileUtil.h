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

#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>  
#include <io.h>
#include <DataTypes.h>

/** 
	��ƿ��Ƽ ������ ���� ���ӽ����̽�.
*/
namespace util
{
	/** 
		�������� ����ü.
	*/
	struct file
	{
		std::string sFileName;		///< ���ϸ�
		struct _stat stat_buf;		///< ��������
		file();
		file(const file& r);
		file& operator = (const file& r);
	};

	/**
		���ϸ����� ���εǴ� ���������� �����ϴ� �� ����
	*/
	typedef std::map<std::string, file> filemap;
	/**
		���������� �����ϴ� ����Ʈ�� ����
	*/
	typedef std::list<file> filelist;

	class folders;
	/**
		���������� ���εǴ� ���������� �����ϴ� �� ����
	*/
	typedef std::map<std::string, folders> foldermap;
	/**
		���������� �����ϴ� ����Ʈ�� ����
	*/
	typedef std::list<std::string> folderlist;
	/**
		������ ���翩�θ� Ȯ���ϴ� �� ����
	*/
	typedef std::map<std::string, bool> foldernamemap;
	/**
		���� ���� Ŭ����.
	*/
	class folders
	{
	public:
		filemap		m_Files;			///< ���� ���ο� ����� ���������� �����ϴ� ��
		std::string m_sHome;			///< ������ ���
		std::string m_sFilter;			///< ���ϸ� ����
		std::string m_sFolderName;		///< ������
		folders*	m_pPrev;			///< ���� ����
		foldermap	m_Folders;			///< ���� ������ �����ϴ� ��
	public:
		folders();
		folders( const folders& r );
		virtual ~folders();

		folders& operator = ( const folders& r );
		/**
			�ֻ��� ������ ��ȸ�Ѵ�
			@param Sub ����
		*/
		folders*	root( folders& Sub );
		/**
			������ ��θ� �����Ѵ�.
			@param sPath ������ ���
			@param sFilter �˻��� ���� ����
		*/
		void		home( const std::string& sPath, const std::string& sFilter = "" );
		/**
			������ ���������� �����Ѵ�.
			@param sFileName ������ �����̸�
			@param stat_buf ��������
		*/
		bool		push_file( const std::string& sFileName, struct _stat& stat_buf );
		/**
			������ ���������� �����´�.
			@param sFileName ������ �����̸�
		*/
		bool		pop_file( const std::string& sFileName );
		/**
			������ ���������� �����Ѵ�.
			@param sFolderName ������ ������
		*/
		folders&	push_folder( const std::string& sFolderName );
		/**
			������ ��� ������ �����Ѵ�.
		*/
		void		clear( void );
		/**
			������ ����� ���������� ������ ��ȸ�Ѵ�.
		*/
		size_t		size_files( void );
		/**
			������ ����� ���������� ������ ��ȸ�Ѵ�.
		*/
		size_t		size_folders( void );
		/**
			���������� ���ο� ����� ���������� ��ȸ�Ѵ�.
			@param Sub �˻��� ����
			@param sFolderName �˻��� ������
		*/
		folders*	find_folder( folders& Sub, const std::string& sFolderName );
		/**
			���ϸ����� ���ο� ����� ���������� ��ȸ�Ѵ�.
			@param Sub �˻��� ����
			@param sFileName �˻��� ���ϸ�
		*/
		file		find_file( folders& Sub, const std::string& sFilePath);
		/**
			������ ����� ��� ���������� ���´�.
			@param flst ������ ����� ��� ���������� ���� ���ϸ���Ʈ
			@param fldr �������� ����ü�� ����Ű�� �ּҰ�
		*/
		void		extract_filelist( filelist& flst, folders& fldr );
		/**
			������ ����� ��� ���������� ���´�.
			@param fldlst ������ ����� ��� ���������� ���� ��������Ʈ
			@param fldr �������� ����ü�� ����Ű�� �ּҰ�
		*/
		void		extract_folderlist( folderlist& fldlst, folders& fldr );
		/**
			������ ����� ��� ������ ȭ�鿡 ����Ѵ�.
			@param fldr ���� ���� ����ü
			@param iLevel ����� ����
		*/
		static void dump(folders& fldr, int iDepth = 0 );
		/**
			������ �������� ���� ��θ� ���´�.
			@param sFileName ���ϸ�
		*/
		std::string absolutepath( const std::string& sFileName = "" );

	private:
		/**
			������ ����� ������ �˻��Ѵ�.
			@param Sub ���� ����ü
			@param sFolderName �˻��� ������
		*/
		folders*	find_subfolders( folders* Sub, const std::string& sFolderName );
	};

	/**
		���� ���� �����ڵ� ����
	*/
	enum
	{
		eError		= -1,		///< ����
		eSuccess	=  0,		///< ����
		eFile		=  1,		///< ����
		eFolder		=  2,		///< ����
	};
	/**
		�������� �����Ѵ�.
		@param sPath �����
		@param pStat ������� ����
	*/
	int isfolder( const std::string& sPath, struct _stat* pStat = nullptr );
	/**
		������ �����Ѵ�.
		@param sPath �����
	*/
	int makedir( const std::string& sDir );
	/**
		������ �����Ѵ�.
		@param sPath �����
	*/
	int removedir( const std::string& rPath );
	/**
		������ �˻��Ͽ� �������� ����ü�� �����Ѵ�.
		@param fldr ��������
		@param iDepth �˻��� ����
	*/
	int scandir( folders& fldr, int iDepth );

};