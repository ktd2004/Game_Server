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

//<
namespace WSA
{
	bool		Startup();
	void		Cleanup();
	long		RefCount();
};

namespace IOCP
{
	HANDLE		New();
	void		Delete( HANDLE& hIOCP );
	bool		Associate( HANDLE hIOCP, SOCKET hSocket, ULONG_PTR key );
	bool		InheritAcceptContext( SOCKET hListen, SOCKET hSocket );
	bool		UpdateConnectContext( SOCKET hSocket );
};

namespace Socket
{
	enum eType
	{
		eTCP,
		eUDP,
	};


	bool		SockAddr( sockaddr_in& addr, const std::string& rHost, unsigned short iPort );
	SOCKET		New( eType e );
	bool		Listen( SOCKET& hSocket, const std::string& rHost, unsigned short iPort );
	bool		Connect( SOCKET& hSocket, unsigned short iPort, const std::string& rHost );
	void		Delete( SOCKET& hSocket, eType e );
	bool		AsyncSend( SOCKET hSocket, WSABUF& wsaBuf, WSAOVERLAPPED& ov );
	bool		AsyncSend( SOCKET hSocket, LPWSABUF wsaBuf, DWORD iBufCnt, WSAOVERLAPPED& ov );
	bool		SyncSend( SOCKET hSocket, WSABUF& wsaBuf, WSAOVERLAPPED& ov );
	bool		Bind( SOCKET& hSocket, const std::string& rHost, unsigned short iPort );
	bool		AsyncSendTo( SOCKET hSocket, WSABUF& wsaBuf, WSAOVERLAPPED& ov, SOCKADDR* to );
	bool		AsyncSendTo( SOCKET hSocket, LPWSABUF wsaBuf, DWORD iBufCnt, WSAOVERLAPPED& ov, SOCKADDR* to );
	bool		Receive( SOCKET hSocket, WSABUF& wsaBuf, WSAOVERLAPPED& ov );
	bool		ReceiveFrom( SOCKET hSocket, WSABUF& wsaBuf, WSAOVERLAPPED& ov,SOCKADDR* from );
	BOOL		AcceptEx( SOCKET hListen,
						  SOCKET hSocket,
						  PVOID lpOutputBuffer,
						  DWORD dwReceiveDataLength,
						  DWORD dwLocalAddressLength,
						  DWORD dwRemoteAddressLength,
						  LPDWORD lpdwBytesReceived,
						  LPOVERLAPPED lpOverlapped );
	BOOL		DisconnectEx( SOCKET hSocket, LPOVERLAPPED lpOverlapped );
	BOOL		ConnectEx( SOCKET hSocket,
						  LPOVERLAPPED lpOverlapped,
						  unsigned short iPort,
						  const std::string& rHost );

};