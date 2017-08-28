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
	유틸리티 모음을 위한 네임스페이스.
*/
namespace util
{
	/** 
		파일정보 구조체.
	*/
	struct file
	{
		std::string sFileName;		///< 파일명
		struct _stat stat_buf;		///< 파일정보
		file();
		file(const file& r);
		file& operator = (const file& r);
	};

	/**
		파일명으로 색인되는 파일정보를 관리하는 맵 정의
	*/
	typedef std::map<std::string, file> filemap;
	/**
		파일정보를 관리하는 리스트를 정의
	*/
	typedef std::list<file> filelist;

	class folders;
	/**
		폴더명으로 색인되는 폴더정보를 관리하는 맵 정의
	*/
	typedef std::map<std::string, folders> foldermap;
	/**
		폴더정보를 관리하는 리스트를 정의
	*/
	typedef std::list<std::string> folderlist;
	/**
		폴더명를 존재여부를 확인하는 맵 정의
	*/
	typedef std::map<std::string, bool> foldernamemap;
	/**
		폴더 정보 클래스.
	*/
	class folders
	{
	public:
		filemap		m_Files;			///< 폴더 내부에 저장된 파일정보를 관리하는 맵
		std::string m_sHome;			///< 폴더의 경로
		std::string m_sFilter;			///< 파일명 필터
		std::string m_sFolderName;		///< 폴더명
		folders*	m_pPrev;			///< 상위 폴더
		foldermap	m_Folders;			///< 하위 폴더를 관리하는 맵
	public:
		folders();
		folders( const folders& r );
		virtual ~folders();

		folders& operator = ( const folders& r );
		/**
			최상위 폴더를 조회한다
			@param Sub 폴더
		*/
		folders*	root( folders& Sub );
		/**
			폴더의 경로를 설정한다.
			@param sPath 폴더의 경로
			@param sFilter 검색할 파일 필터
		*/
		void		home( const std::string& sPath, const std::string& sFilter = "" );
		/**
			폴더에 파일정보를 저장한다.
			@param sFileName 저장할 파일이름
			@param stat_buf 파일정보
		*/
		bool		push_file( const std::string& sFileName, struct _stat& stat_buf );
		/**
			폴더에 파일정보를 꺼내온다.
			@param sFileName 꺼내올 파일이름
		*/
		bool		pop_file( const std::string& sFileName );
		/**
			폴더에 폴더정보를 저장한다.
			@param sFolderName 저장할 폴더명
		*/
		folders&	push_folder( const std::string& sFolderName );
		/**
			폴더의 모든 정보를 삭제한다.
		*/
		void		clear( void );
		/**
			폴더에 저장된 파일정보의 개수를 조회한다.
		*/
		size_t		size_files( void );
		/**
			폴더에 저장된 폴더정보의 개수를 조회한다.
		*/
		size_t		size_folders( void );
		/**
			폴더명으로 내부에 저장된 폴더정보를 조회한다.
			@param Sub 검색할 폴더
			@param sFolderName 검색할 폴더명
		*/
		folders*	find_folder( folders& Sub, const std::string& sFolderName );
		/**
			파일명으로 내부에 저장된 파일정보를 조회한다.
			@param Sub 검색할 폴더
			@param sFileName 검색할 파일명
		*/
		file		find_file( folders& Sub, const std::string& sFilePath);
		/**
			폴더에 저장된 모든 파일정보를 얻어온다.
			@param flst 폴더에 저장된 모든 파일정보를 얻어올 파일리스트
			@param fldr 폴더정보 구조체를 가리키는 주소값
		*/
		void		extract_filelist( filelist& flst, folders& fldr );
		/**
			폴더에 저장된 모든 폴더정보를 얻어온다.
			@param fldlst 폴더에 저장된 모든 폴더정보를 얻어올 폴더리스트
			@param fldr 폴더정보 구조체를 가리키는 주소값
		*/
		void		extract_folderlist( folderlist& fldlst, folders& fldr );
		/**
			폴더에 저장된 모든 정보를 화면에 출력한다.
			@param fldr 폴더 정보 구조체
			@param iLevel 출력할 깊이
		*/
		static void dump(folders& fldr, int iDepth = 0 );
		/**
			폴더를 기준으로 절대 경로를 얻어온다.
			@param sFileName 파일명
		*/
		std::string absolutepath( const std::string& sFileName = "" );

	private:
		/**
			폴더에 저장된 폴더를 검색한다.
			@param Sub 폴더 구조체
			@param sFolderName 검색할 폴더명
		*/
		folders*	find_subfolders( folders* Sub, const std::string& sFolderName );
	};

	/**
		파일 관련 에러코드 정의
	*/
	enum
	{
		eError		= -1,		///< 에러
		eSuccess	=  0,		///< 성공
		eFile		=  1,		///< 파일
		eFolder		=  2,		///< 폴더
	};
	/**
		폴더인지 조사한다.
		@param sPath 대상경로
		@param pStat 대상경로의 정보
	*/
	int isfolder( const std::string& sPath, struct _stat* pStat = nullptr );
	/**
		폴더를 생성한다.
		@param sPath 대상경로
	*/
	int makedir( const std::string& sDir );
	/**
		폴더를 삭제한다.
		@param sPath 대상경로
	*/
	int removedir( const std::string& rPath );
	/**
		폴더를 검색하여 폴더정보 구조체를 생성한다.
		@param fldr 폴더정보
		@param iDepth 검색할 깊이
	*/
	int scandir( folders& fldr, int iDepth );

};