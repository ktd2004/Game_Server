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
#include <MemoryEx.h>
#include <DBEngine.h>
#include <Management.h>
#include <getopt.h>
#include <ServerConfigManager.h>

void    ShowUsage()
{
	printf( "usage)\n" );
	printf( "  config { -d [download file] or -u [upload file] } -w [world name]\n" );
	printf( "          -s [section name,id] -h [ip,port,category,uid,passwd,charset]\n" );
	printf( "ex)\n" );
	printf( "  upload config   : config -u MstS.ini -w World1 -s MstS,10001 \n" );
	printf( "                            -h 192.168.8.9,3306,game,root,xxxx,euckr\n" );
	printf( "  download config : config -d ChnS.ini -w World1 -s ChnS,25001 \n" );
	printf( "                            -h 192.168.8.9,3306,game,root,xxxx,euckr\n" );
	printf( "                    config -d SAgent.ini -w World1 -s SAgent \n" );
	printf( "                            -h 192.168.8.9,3306,game,root,xxxx,euckr\n" );
	printf( "                    config -d PAgent.ini -w World1 -s PAgent,30 \n" );
	printf( "                            -h 192.168.8.9,3306,game,root,xxxx,euckr\n" );
	printf( "  display         : config -w World1 -s ChnS,10000 \n" );
	printf( "                            -h 192.168.8.9,3306,game,root,xxxx,euckr\n" );
	printf( "  append          : config -w World1 -s ChnS,10000 -a key=value \n" );
	printf( "                            -h 192.168.8.9,3306,game,root,xxxx,euckr\n" );
	printf( "  client to server: config -w World1 -outer \n" );
	printf( "                            -h 192.168.8.9,3306,game,root,xxxx,euckr\n" );
	printf( "  server to server: config -w World1 -inner \n" );
	printf( "                            -h 192.168.8.9,3306,game,root,xxxx,euckr\n" );
}

int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc == 1 )
	{
		ShowUsage();
		return 1;
	}

	std::string sFileName,
				sWorldName,
				sSectionName,
				sSID,
				sKey,
				sVal;

	ServerConfigManager m;
	IDBProvider::Properties DBProp;
	enum { eDisplay, eDownload, eUpload, eAppend, eClientPort, eServerPort, eDBTest };
	int32 iCase= eDisplay;

	int  opt;
	while(1)
	{
		int  option_index = 0; 
		opt  =  getopt(argc, argv, "w:s:u:d:h:a:oit"); 
		if  (opt  ==  -1)
			break; 

		switch(opt)
		{
		case 'u':
			sFileName = optarg;
			iCase = eUpload;
			break;
		case 'd':
			sFileName = optarg;
			iCase = eDownload;
			break;
		case 'w':
			sWorldName = optarg;
			break;
		case 'h':
			{
				// db Á¤º¸ : host,port,category,uid,passwd
				stringvector lst;
				util::tokenizer(optarg, lst, ',' );
				if ( lst.size() < 5)
				{
					prn_err( "dbconnectstring error" );
					return 1;
				}
				DBProp.stDatabase.sHost = lst[0];
				DBProp.stDatabase.sPort = lst[1];
				DBProp.sProviderName = lst[2];
				DBProp.stDatabase.sCategory = DBProp.sProviderName;
				DBProp.stDatabase.sUid = lst[3];
				DBProp.stDatabase.sPassword = lst[4];
				DBProp.stDatabase.bAutoReconnect = false;
				if ( lst.size() == 5 )
				{
					DBProp.stDatabase.sCharset = lst[5];
				}			
			}
			break;
		case 's':
			{
				stringvector lst;
				util::tokenizer(optarg, lst, ',' );
				if ( lst.size() == 1 )
				{
					sSectionName = lst[0];
				}
				else if ( lst.size() == 2 )
				{
					sSectionName = lst[0];
					sSID = lst[1];
				}
				else
				{
					prn_err( "section argument error" );
					return 1;
				}
			}
			break;
		case 'a' :
			{	// -a key=value
				sKey = util::pairkey( optarg );
				sVal = util::pairval( optarg );
				iCase = eAppend;
			}
			break;
		case 'o' :
			iCase = eClientPort;
			break;
		case 'i' :
			iCase = eServerPort;
			break;
		case 't' :
			iCase = eDBTest;
			break;
		case ':':
			prn_err( "-%c requires an argument\n", optarg );
			return 1;
		default:
			prn_err( "argument parse error" );
			return 1;
		}
	}

	if (iCase != eDBTest)
	{
		if (sWorldName.empty() ||
			(iCase == eUpload && sFileName.empty()) ||
			(iCase == eDownload && sFileName.empty()) ||
			(iCase == eAppend && (sSectionName.empty() || sKey.empty() || sVal.empty())))
		{
			prn_err("argument error");
			ShowUsage();
			return 1;
		}
	}

	if ( !m.Connect(DBProp) )
	{
		return 1;
	}

	if (iCase == eDBTest)
	{
		prn_inf("connect success");
		return 0;
	}
	
	switch(iCase)
	{
	case eDisplay :
		m.DisplayConfig(sWorldName, sSID, sSectionName);
		break;
	case eClientPort :
		m.DisplayConfig(sWorldName, "", "ClientPort");
		break;
	case eServerPort :
		m.DisplayConfig(sWorldName, "", "ServerPort");
		break;
	case eUpload :
		m.SaveDB( sFileName, sWorldName, false );
		break;
	case eDownload :
		m.SaveFile( sFileName, sWorldName, sSID, sSectionName );
		break;
	case eAppend :
		m.SaveDB( sWorldName, sSectionName, sSID, sKey, sVal );
		break;
	}

	return 0;
}

