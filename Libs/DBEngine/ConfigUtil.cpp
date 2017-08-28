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
#include <DataTypes.h>
#include <StringUtil.h>
#include <INILoader.h>
#include <Logger.h>
#include <ConfigUtil.h>


namespace util
{
	void load_dbcfg(const std::string& sDBSection, IDBProvider::Properties& config)
	{
		config.sProviderName				= Env_s( sDBSection, "provider_name", sDBSection );
		config.stDatabase.sHost				= Env_s( sDBSection, "host", "localhost" );
		config.stDatabase.sPort				= Env_s( sDBSection, "port", "3306" );
		config.stDatabase.sCategory			= Env_s( sDBSection, "category", "" );
		config.stDatabase.sUid				= Env_s( sDBSection, "uid", "root" );
		config.stDatabase.sPassword			= Env_s( sDBSection, "passwd", "" );
		config.stDatabase.sCharset			= Env_s( sDBSection, "charset", "euckr" );
		config.stDatabase.bAutoReconnect	= Env_b( sDBSection, "auto_reconnect", true );
		config.stDatabase.iConnectionCount	= Env_i( sDBSection, "database_connection_count", 4 );
		config.iCompressOverSize			= Env_i( sDBSection, "compress_over_size", 0 );
	}

	void load_remotedbcfg(const std::string& sDBSection, IDBProvider::Properties& config)
	{
		config.sProviderName				= Env_s( sDBSection, "provider_name", sDBSection );
		config.stRemote.bReconnect			= Env_b( sDBSection, "auto_reconnect", true );
		config.stRemote.sHost				= Env_s( sDBSection, "remote_addr", "localhost" );
		config.stRemote.sPort				= Env_s( sDBSection, "remote_port", "6001" );
		config.stRemote.iThreadCount		= Env_i( sDBSection, "network_threads_count", 4 );
		config.iCompressOverSize			= Env_i( sDBSection, "compress_over_size", 0 );
		config.stRemote.iMaxBufSize			= Env_i( sDBSection, "max_buf_size", 4096 );
	}

	void show_dbcfg(IDBProvider::Properties& config)
	{
		prn_inf( "provider name %s", cstr(config.sProviderName) );
		prn_inf( "++ db addr,port (%s,%s)", cstr(config.stDatabase.sHost), cstr(config.stDatabase.sPort) );
		prn_inf( "++ category (%s)", cstr(config.stDatabase.sCategory) );
		prn_inf( "++ uid/password (%s/%s)", cstr(config.stDatabase.sUid), cstr(config.stDatabase.sPassword) );
		prn_inf( "++ character set (%s)", cstr(config.stDatabase.sCharset) );
		prn_inf( "++ auto reconnect (%s)", config.stDatabase.bAutoReconnect ? "true" : "false" );
		prn_inf( "++ db connections (%d)", config.stDatabase.iConnectionCount );
	}


	void show_remotedbcfg(IDBProvider::Properties& config)
	{
		prn_inf( "provider name %s", cstr(config.sProviderName) );
		prn_inf( "++ auto reconnect (%s)", bool2str(config.stDatabase.bAutoReconnect) );
		prn_inf( "++ remote addr,port (%s,%s)", cstr(config.stRemote.sHost), cstr(config.stRemote.sPort) );
		prn_inf( "++ remote thread count(%d)", config.stRemote.iThreadCount );
		prn_inf( "++ compress over size(%d)", config.iCompressOverSize );
	}

}
