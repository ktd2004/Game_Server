#pragma once

enum LogProtocol
{
	eLOG		= 1000, 
	eLOG_MAX	= 2000, 
};

// gateway -> client
enum GtwsProtocol
{
	eGTWS_CLIENT_LOGIN_ERROR = 2001, 
	eGTWS_CLIENT_LOGIN_SUCCESS, 
	eGTWS_MAX = 3000, 
};

// client -> gtws
enum ClientProtocol
{
	eCLIENT_GTWS_LOGIN = 3001, 
	eCLIENT_GTWS_LOGIN_RESULT, 
};
