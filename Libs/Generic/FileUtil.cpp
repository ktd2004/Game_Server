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
#include "stdafx.h"
#include <MacroFunc.h>
#include <StringUtil.h>
#include <FileUtil.h>


namespace util
{

	file::file() 
		: sFileName("")
	{
		memset( &stat_buf, 0x00, sizeof(stat_buf) );
	}

	file::file( const file& r )
	{
		sFileName = r.sFileName;
		memcpy( &stat_buf, &r.stat_buf, sizeof(stat_buf) );
	}

	file& file::operator=( const file& r )
	{
		sFileName = r.sFileName;
		memcpy( &stat_buf, &r.stat_buf, sizeof(stat_buf) );
		return *this;
	}


	folders::folders()
		: m_sFolderName("")
		, m_sFilter("*.*")
		, m_pPrev(nullptr)
	{

	}

	folders::folders( const folders& r )
	{
		m_sFolderName = r.m_sFolderName;
		m_Files = r.m_Files;
		m_sFilter = r.m_sFilter;
		m_pPrev = r.m_pPrev;
	}

	folders::~folders()
	{
		clear();
	}

	folders& folders::operator=( const folders& r )
	{
		m_sFolderName = r.m_sFolderName;
		m_Files = r.m_Files;
		m_Folders = r.m_Folders;
		m_sHome = r.m_sHome;
		m_sFilter = r.m_sFilter;
		return *this;
	}

	folders* folders::root( folders& Sub )
	{
		folders* t = &Sub;
		while ( t->m_pPrev )
		{
			t = t->m_pPrev;
		}
		return t;
	}

	void folders::home( const std::string& sPath, const std::string& sFilter )
	{
		m_sHome = sPath;
		if ( sFilter.size() )
			m_sFilter = sFilter;
		else
			m_sFilter = "*.*";
	}

	bool folders::push_file( const std::string& sFileName, struct _stat& stat_buf )
	{
		filemap::iterator it = m_Files.find( sFileName );
		if ( it != m_Files.end() )
			return false;
		file f;
		f.sFileName = sFileName;
		memcpy( &f.stat_buf, &stat_buf, sizeof(f.stat_buf) );
		m_Files[sFileName] = f;
		return true;
	}

	bool folders::pop_file( const std::string& sFileName )
	{
		filemap::iterator it = m_Files.find( sFileName );
		if ( it != m_Files.end() )
		{
			m_Files.erase(it);
			return true;
		}
		return false;
	}

	folders& folders::push_folder( const std::string& sFolderName )
	{
		foldermap::iterator it = m_Folders.find( sFolderName );
		if ( it != m_Folders.end() )
		{
			return it->second;
		}
		folders f;
		f.m_sFolderName = m_sFolderName.size() ? m_sFolderName + "\\" + sFolderName : sFolderName;
		f.m_sHome = m_sHome;
		f.m_pPrev = this;
		m_Folders[sFolderName] = f;
		return m_Folders[sFolderName];
	}

	void folders::clear()
	{
		m_Files.clear();
		m_Folders.clear();
	}

	size_t folders::size_files()
	{
		return m_Files.size();
	}

	size_t folders::size_folders()
	{
		return m_Folders.size();
	}

	folders* folders::find_folder( folders& Sub, const std::string& sFolderName )
	{
		folders* fld = nullptr;
		size_t homepos = sFolderName.find( m_sHome );
		if ( homepos != std::string::npos )
		{
			fld = &Sub;
			size_t start = homepos + m_sHome.size() + 1;
			size_t pos = sFolderName.find_first_of( "\\/", start );
			while ( pos != std::string::npos )
			{
//				printf( "%s\n", sFolderName.substr(start, pos-start).c_str() );
				fld = find_subfolders( fld, sFolderName.substr(start, pos-start).c_str() );
				if ( fld )
				{
					size_t t = sFolderName.find_first_of( "\\/", pos + 1 );
					if ( t == std::string::npos && pos < sFolderName.size() )
					{
//						printf( "%s\n", sFolderName.substr(start).c_str());
						fld = find_subfolders( fld, sFolderName.substr(start).c_str() );
						break;
					}
					else
					{ pos = t; }
				}
				else
				{ break; }
			}

		}
		return fld;
	}

	util::file folders::find_file( folders& Sub, const std::string& sFilePath )
	{
		const std::string sPath =  util::getworkpath( sFilePath );
		const std::string sFileName = util::getfilename( sFilePath );
		folders* fld = find_folder( Sub, sPath );
		if ( fld )
		{
			filemap::iterator it = fld->m_Files.find( sFileName );
			if ( it != fld->m_Files.end() )
			{
				return it->second;
			}
		}
		return file();
	}

	void folders::extract_filelist( filelist& flst, folders& fldr )
	{
//		printf( "%sD)%s\n", sp.c_str(), f.m_sFolderName.c_str() );
		for ( filemap::iterator itfile = fldr.m_Files.begin(); itfile != fldr.m_Files.end(); itfile++ )
		{
			file fd = itfile->second;
			if ( fldr.m_sFolderName.size() != 0 )
			{ fd.sFileName = fldr.m_sFolderName + "\\" + itfile->second.sFileName; }
			else
			{ fd.sFileName = itfile->second.sFileName; }

			flst.push_back( fd );
		}		
		for ( foldermap::iterator itfolder = fldr.m_Folders.begin(); itfolder != fldr.m_Folders.end(); itfolder++ )
		{
			extract_filelist(flst, itfolder->second );
		}
	}

	void folders::extract_folderlist( folderlist& fldlst, folders& fldr )
	{
		if ( fldr.m_sFolderName.size() != 0 )
		{ fldlst.push_back( fldr.m_sFolderName.c_str() ); }

		for ( foldermap::iterator itfolder = fldr.m_Folders.begin(); itfolder != fldr.m_Folders.end(); itfolder++ )
		{
			extract_folderlist( fldlst, itfolder->second );
		}
	}

	void folders::dump( folders& fldr, int iDepth /*= 0 */ )
	{
		std::string sp(iDepth, ' ');
		printf( "%sD)%s\n", sp.c_str(), fldr.m_sFolderName.c_str() );
		for ( filemap::iterator itfile = fldr.m_Files.begin(); itfile != fldr.m_Files.end(); itfile++ )
		{
			printf( "%s%sF)%s\n", sp.c_str(), sp.c_str(), itfile->first.c_str() );
		}		
		for ( foldermap::iterator itfolder = fldr.m_Folders.begin(); itfolder != fldr.m_Folders.end(); itfolder++ )
		{
			dump(itfolder->second, ++iDepth);
		}
	}

	folders* folders::find_subfolders( folders* Sub, const std::string& sFolderName )
	{
		if ( m_sFolderName == sFolderName )
		{ return this; }
		foldermap::iterator it = Sub->m_Folders.find(sFolderName);
		if ( it != m_Folders.end() )
		{ return &it->second; }
		return nullptr;
	}

	std::string folders::absolutepath( const std::string& sFileName )
	{
		if ( sFileName.size() != 0 && m_sFolderName.size() != 0 )
		{
			return m_sHome + "\\" + m_sFolderName + "\\" + sFileName;
		}
		else if ( sFileName.size() == 0 && m_sFolderName.size() != 0 )
		{
			return m_sHome + "\\" + m_sFolderName;
		}
		else if ( sFileName.size() )
		{
			return m_sHome + "\\" + sFileName;
		}
	
		return m_sHome;
	}

	bool isdotdir( const char *s )
	{
		return (s[0] == '.' &&
			((s[1] == 0) || (s[1] == '.' && s[2] == 0)));
	}

	int recursive_scandir( folders& fld, int iDepth, int iRecursiveLevel )
	{
		if ( iDepth != 0 && iDepth <= iRecursiveLevel )
		{
			return eError;
		}

		struct _finddata_t dir;
		intptr_t	hFile;
		if ( (hFile = _findfirst(cstr(fld.absolutepath(fld.m_sFilter)), &dir)) == -1 )
		{
			return eError;
		}

		do
		{
			if ( isdotdir(dir.name) )
				continue;

			if ( dir.attrib != _A_SUBDIR )
			{
				struct _stat fs;
				::_stat( (const char*)fld.absolutepath(dir.name).c_str(), &fs );
				fld.push_file( dir.name, fs );
			}
			else
			{
				recursive_scandir( fld.push_folder(dir.name), iDepth, iRecursiveLevel );
			}
		} while ( _findnext( hFile, &dir) == 0 );

		_findclose( hFile );

		return eSuccess;
	}

	int scandir( folders& fldr, int iDepth )
	{
		return recursive_scandir( fldr, iDepth, 0 );
	}

	int isfolder( const std::string& sPath, struct _stat* pStat /*= nullptr */ )
	{
		int rst = eError;
		struct _stat stat_buf;
		if ( ::_stat( (const char*)sPath.c_str(), &stat_buf) == -1 )
		{
			return rst;
		}

		if ( pStat )
		{
			memcpy( pStat, &stat_buf, sizeof(stat_buf) );
		}

		switch( stat_buf.st_mode & S_IFMT )
		{
		case S_IFREG :
			rst = eFile;
			break;
		case S_IFDIR :
			rst = eFolder;
			break;
		default :
			rst = eFile;
			break;
		}

		return rst;
	}

	int makedir( const std::string& sDir )
	{
		std::stringstream ss;
		ss << sDir;

		std::string level;
		std::string s;
		while ( std::getline(ss, s, '\\') )
		{
			if ( s.find_first_of(":") != std::string::npos )
			{
				level += s + "\\";
				continue;
			}

			level += s;
			struct _stat isExist;
			if( ::_stat(s.c_str(), &isExist) != 0 )
			{
//				printf( "%s\n", level.c_str() );
				if( ::mkdir(level.c_str()) != 0 )
				{
					return eError;
				}
			}
			level += "\\";
		}

		return eSuccess;
	}

	int recursive_removedir( const std::string& rPath )
	{
		int rst = eSuccess;
		::chdir(rPath.c_str());

		std::string sFilter = rPath + "\\*.*";
		struct _finddata_t dir;
		intptr_t hFile;
		if ( (hFile = _findfirst(cstr(sFilter), &dir)) == -1 )
		{
			return eError;
		}
		else
		{
			do
			{
				if ( isdotdir(dir.name) == false )
					continue;

				struct _stat stat_buf;
				if ( ::_stat(dir.name, &stat_buf) == -1 )
				{
					_findclose( hFile );
					return rst;
				}

				switch( stat_buf.st_mode & S_IFMT )
				{
				case S_IFREG :
					::unlink( dir.name );
//					printf( "unlink path = %s\\%s\n", rPath.c_str(), dir.name );
					break;
				case S_IFDIR :
					removedir(  rPath + "\\" + dir.name );
					break;
				default :
					// ÆÄÀÏ
					::unlink( dir.name );
//					printf( "unlink path = %s\\%s\n", rPath.c_str(), dir.name );
					break;
				}

			} while ( _findnext( hFile, &dir) == 0 );

			_findclose( hFile );
		}

		::chdir( ".." );
		::rmdir( rPath.c_str() );
//		printf( "rmdir %s\n", rPath.c_str() );

		return rst;
	}

	int removedir( const std::string& rPath )
	{
		char szDir[_MAX_PATH];
		::GetCurrentDirectory( sizeof(szDir), szDir );
		int ret = recursive_removedir(rPath);
		::chdir( szDir );
		return ret;
	}
};


