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
#include <Ptr.h>
#include "NetUtil.h"
#include <MSWSock.h>
#include <MSTcpIP.h> //WSAIoctl 을 하기위한(keepAlive) header
#include <MacroFunc.h>
#include <Logger.h>

initializer_begin
{
	WSADATA WsaData;
	if ( WSAStartup(WINSOCK_VERSION, &WsaData) != 0 )
	{
		prn_err( "WSAStartup error(%d)", GetLastError() );
	}
}
end_initializer;

namespace IOCP
{
	HANDLE New()
	{
		HANDLE h = NULL;
		h = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
		return h;
	}

	void Delete( HANDLE& hIOCP )
	{
		istrue_ret( hIOCP == NULL, );
		HANDLE hTemp = hIOCP;
		hIOCP = NULL;
		CloseHandle( hTemp );
	}

	bool Associate( HANDLE hIOCP, SOCKET hSocket, ULONG_PTR key )
	{
		HANDLE hTemp = CreateIoCompletionPort(reinterpret_cast<HANDLE>(hSocket), hIOCP, (ULONG_PTR)key, 0);
		istrue_ret( hTemp != hIOCP, false );
		return true;
	}

	bool InheritAcceptContext( SOCKET hListen, SOCKET hSocket )
	{
		istrue_ret( 
			SOCKET_ERROR == setsockopt(hSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&hListen, sizeof(hListen)),
			false );

		return true;
	}

	bool UpdateConnectContext( SOCKET hSocket )
	{
		istrue_ret( 
			SOCKET_ERROR == setsockopt(hSocket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0),
			false );

		return true;
	}
};


namespace Socket
{
	static LPFN_ACCEPTEX		lpfnAcceptEx = NULL;
	static GUID					guidAcceptEx = WSAID_ACCEPTEX;
	static LPFN_DISCONNECTEX	lpfnDisconnectEx = NULL;
	static GUID					guidDisconnectEx = WSAID_DISCONNECTEX;
	static LPFN_CONNECTEX		lpfnConnectEx = NULL;
	static GUID					guidConnectEx = WSAID_CONNECTEX;

	bool LoadSocketExAPI( SOCKET hSocket )
	{
		istrue_ret( lpfnAcceptEx && lpfnDisconnectEx && lpfnConnectEx, true );

		DWORD dwBytes;
		istrue_ret(
			SOCKET_ERROR == WSAIoctl(
			hSocket,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidAcceptEx,
			sizeof(guidAcceptEx),
			&lpfnAcceptEx,
			sizeof(lpfnAcceptEx),
			&dwBytes,
			NULL,
			NULL),
			false );
		istrue_ret(
			SOCKET_ERROR == WSAIoctl(
			hSocket,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidDisconnectEx,
			sizeof(guidDisconnectEx),
			&lpfnDisconnectEx,
			sizeof(lpfnDisconnectEx),
			&dwBytes,
			NULL,
			NULL),
			false );
		istrue_ret(
			SOCKET_ERROR == WSAIoctl(
			hSocket,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidConnectEx,
			sizeof(guidConnectEx),
			&lpfnConnectEx,
			sizeof(lpfnConnectEx),
			&dwBytes,
			NULL,
			NULL),
			false );
		return true;
	}

	bool SockAddr( sockaddr_in& addr, const std::string& rHost, unsigned short iPort )
	{
		addr.sin_family	= AF_INET; addr.sin_port = htons(iPort);
		if ( rHost.empty() )
		{
			addr.sin_addr.s_addr	= htonl(INADDR_ANY);
		}
		else
		{
			addr.sin_addr.s_addr	= inet_addr(rHost.c_str());
			if(addr.sin_addr.s_addr == INADDR_NONE)
			{
				LPHOSTENT lpHosten = gethostbyname(rHost.c_str());
				isnull_ret(lpHosten, false);
				addr.sin_addr.s_addr  = ((LPIN_ADDR)lpHosten->h_addr)->s_addr;
			}
		}

		return true;
	}

	bool ClientOption( SOCKET hSocket, eType e )
	{
		int iVal = 0;
		istrue_ret(
			SOCKET_ERROR == setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&iVal, sizeof(iVal)),
			false);
		iVal = 0;
		istrue_ret(
			SOCKET_ERROR == setsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&iVal, sizeof(iVal)),
			false);
		iVal = 1;
		istrue_ret(
			SOCKET_ERROR == setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&iVal, sizeof(iVal)),
			false);

		if ( e == eTCP )
		{
			iVal = 1;
			istrue_ret(
				SOCKET_ERROR == setsockopt(hSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&iVal, sizeof(iVal)),
				false);
			BOOL bVal = TRUE;
			istrue_ret(
				SOCKET_ERROR == setsockopt(hSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&bVal, sizeof(bVal)),
				false);

			// 20초에 한 번씩, 재전송 주기는 1초에 한 번씩으로 맞춘다.
			tcp_keepalive keepAlive = { TRUE, 10000, 2000 }; DWORD tmp;
			istrue_ret(
				SOCKET_ERROR == WSAIoctl(hSocket, SIO_KEEPALIVE_VALS, &keepAlive, sizeof(keepAlive), 0, 0, &tmp, NULL, NULL),
				false);

			LINGER _SO_LINGER = { 1, 0 };
			istrue_ret(
				SOCKET_ERROR == setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (char *)&_SO_LINGER, sizeof(_SO_LINGER)),
				false);
		}

		return true;
	}

	bool ServerOption( SOCKET hSocket )
	{
		BOOL bVal = TRUE;
		istrue_ret(
			SOCKET_ERROR == setsockopt(hSocket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&bVal, sizeof(bVal)),
			false);

		return true;
	}

	SOCKET	New( eType e )
	{
		SOCKET hSocket	= INVALID_SOCKET;
		int iType;
		int iProto;

		switch ( e )
		{
		case eUDP : 
			iType = SOCK_DGRAM;
			iProto = IPPROTO_UDP;
			break;
		case eTCP :
			iType = SOCK_STREAM;
			iProto = IPPROTO_TCP;
			break;
		}

		hSocket = WSASocket( 
					AF_INET,
					iType,
					iProto,
					NULL, NULL,
					WSA_FLAG_OVERLAPPED );
		
		istrue_do_ret(
			false == ClientOption(hSocket, e),
			Socket::Delete(hSocket, e), hSocket );

		return hSocket;
	}

	bool Listen( SOCKET& hSocket, const std::string& rHost, unsigned short iPort )
	{
		istrue_goto(
			false == ServerOption(hSocket),
			Socket_Listen_Error );

		struct sockaddr_in sa;
		istrue_goto(
			false == SockAddr(sa, rHost, iPort),
			Socket_Listen_Error );
		istrue_goto(
			SOCKET_ERROR == ::bind(hSocket, (struct sockaddr *)&sa, sizeof(sa)),
			Socket_Listen_Error );
		istrue_goto(
			SOCKET_ERROR == ::listen(hSocket, 128),
			Socket_Listen_Error );

		return true;

Socket_Listen_Error :
		Socket::Delete(hSocket, eTCP);
		return false;
	}

	bool Connect( SOCKET& hSocket, unsigned short iPort, const std::string& rHost )
	{
		istrue_goto(
			false == ClientOption(hSocket, eTCP),
			Socket_Connect_Error );

		struct sockaddr_in sa;
		istrue_goto(
			false == SockAddr(sa, rHost, iPort),
			Socket_Connect_Error );
		
		istrue_goto(
			SOCKET_ERROR == ::connect(hSocket, (struct sockaddr*)&sa, sizeof(sa)),
			Socket_Connect_Error );

		return true;

Socket_Connect_Error :
		Socket::Delete( hSocket, eTCP );
		return false;
	}

	void Delete( SOCKET& hSocket, eType e )
	{
		istrue_ret( hSocket == INVALID_SOCKET, );
//		SOCKET hTemp = InterlockedExchange((LONG *)&hSocket, INVALID_SOCKET);
		SOCKET hTemp = (SOCKET)locked_exchange_pointer(hSocket, INVALID_SOCKET);
		if ( e == eTCP )
		{
			LINGER linger = { 1, 0 };
			setsockopt( hTemp, SOL_SOCKET, SO_LINGER,	(char *)&linger, sizeof(linger) );
		}
		else if ( e == eUDP )
		{
			shutdown( hTemp , SD_SEND | SD_RECEIVE );
		}
		CancelIo((HANDLE) hTemp);
		closesocket( hTemp );
	}

	bool AsyncSend( SOCKET hSocket, WSABUF& wsaBuf, WSAOVERLAPPED& ov )
	{
		DWORD dwSent = 0;
		if ( SOCKET_ERROR == WSASend(hSocket, &wsaBuf, 1, &dwSent, 0, &ov,	NULL) )
		{
			istrue_ret( ERROR_IO_PENDING != WSAGetLastError(), false );
		}
		return true;
	}

	bool AsyncSend( SOCKET hSocket, LPWSABUF wsaBuf, DWORD iBufCnt, WSAOVERLAPPED& ov )
	{
		DWORD dwSent = 0;
		if ( SOCKET_ERROR == WSASend(hSocket, wsaBuf, iBufCnt, &dwSent, 0, &ov,	NULL) )
		{
			istrue_ret( ERROR_IO_PENDING != WSAGetLastError(), false );
		}
		return true;
	}


	bool SyncSend( SOCKET hSocket, WSABUF& wsaBuf, WSAOVERLAPPED& ov )
	{
		DWORD		dwSent = 0;
		DWORD		dwFlag = 0;

		WSAEVENT	evtSend = WSACreateEvent();
		ov.hEvent = evtSend;
		if ( SOCKET_ERROR == WSASend(hSocket, &wsaBuf, 1, &dwSent, 0, &ov,	NULL) )
		{
			if ( ERROR_IO_PENDING != WSAGetLastError() )
			{
				WSACloseEvent(evtSend);
				return false;
			}
		}

		WSAWaitForMultipleEvents(1, &evtSend, TRUE, WSA_INFINITE, FALSE);
		if ( FALSE == WSAGetOverlappedResult(hSocket, &ov, &dwSent, FALSE, &dwFlag) )
		{
			prn_err( "send data failed, error code(%u)", WSAGetLastError() );
		}
		WSACloseEvent(evtSend);

		return true;
	}

	bool Receive( SOCKET hSocket, WSABUF& wsaBuf, WSAOVERLAPPED& ov )
	{
		DWORD dwRecved	= 0, dwFlags	= 0;
		if ( SOCKET_ERROR == WSARecv(hSocket, &wsaBuf, 1, &dwRecved, &dwFlags, &ov, NULL))
		{
			istrue_ret( ERROR_IO_PENDING != WSAGetLastError(), false );
		}
		return true;
	}

	bool Bind( SOCKET& hSocket, const std::string& rHost, unsigned short iPort )
	{
		struct sockaddr_in sa;
		istrue_goto(
			false == SockAddr(sa, rHost, iPort),
			Socket_Bind_Error );

		istrue_goto(
			SOCKET_ERROR == ::bind(hSocket, (struct sockaddr *)&sa, sizeof(sa)),
			Socket_Bind_Error );

		return true;

Socket_Bind_Error :
		Socket::Delete(hSocket, eUDP);
		return false;
	}

	bool AsyncSendTo( SOCKET hSocket, WSABUF& wsaBuf, WSAOVERLAPPED& ov, SOCKADDR* to )
	{
		static const INT iAddrSize = sizeof(SOCKADDR_IN);
		DWORD dwSent = 0;

		if ( SOCKET_ERROR == WSASendTo(hSocket,	&wsaBuf, 1,	&dwSent, 0,	to,	iAddrSize, &ov,	NULL) )
		{
			istrue_ret( (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK),
				false );
		}

		return true;
	}

	bool AsyncSendTo( SOCKET hSocket, LPWSABUF wsaBuf, DWORD iBufCnt, WSAOVERLAPPED& ov, SOCKADDR* to )
	{
		static const INT iAddrSize = sizeof(SOCKADDR_IN);
		DWORD dwSent = 0;

		if ( SOCKET_ERROR == WSASendTo(hSocket,	wsaBuf, iBufCnt, &dwSent, 0, to, iAddrSize, &ov, NULL) )
		{
			istrue_ret( (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK),
				false );
		}

		return true;
	}

	bool ReceiveFrom( SOCKET hSocket, WSABUF& wsaBuf, WSAOVERLAPPED& ov, SOCKADDR* from )
	{
		INT iAddrSize = sizeof(SOCKADDR_IN);
		DWORD dwRecved			= 0;
		DWORD dwFlags			= 0;

ERROR_RAISE:
		if ( SOCKET_ERROR == WSARecvFrom(hSocket, &wsaBuf, 1, &dwRecved, &dwFlags, from, &iAddrSize, &ov,NULL) )
		{
			DWORD dwError = WSAGetLastError();
			if ( dwError != WSA_IO_PENDING && dwError != WSAEWOULDBLOCK)
			{
				if ( dwError == ERROR_PORT_UNREACHABLE || dwError == WSAECONNRESET )
				{
					goto ERROR_RAISE;
				}
				return false;
			}
		}

		return true;
	}

	BOOL AcceptEx( SOCKET hListen, SOCKET hSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped )
	{
		isfalse_ret( LoadSocketExAPI(hListen), FALSE );

		return lpfnAcceptEx(
					hListen,				// Listen Socket
					hSocket,				// Accept Socket
					lpOutputBuffer,			// Buffer for Accept( server, remote address )
					dwReceiveDataLength,	// default 0
					dwLocalAddressLength,	// local address length
					dwRemoteAddressLength,	// remote address length
					lpdwBytesReceived,		// received bytes
					lpOverlapped );			// overlapped
	}

	BOOL DisconnectEx( SOCKET hSocket, LPOVERLAPPED lpOverlapped )
	{
		isfalse_ret( LoadSocketExAPI(hSocket), FALSE );

		return lpfnDisconnectEx( hSocket, lpOverlapped, TF_REUSE_SOCKET, 0 );
	}

	BOOL ConnectEx( SOCKET hSocket, LPOVERLAPPED lpOverlapped, unsigned short iPort, const std::string& rHost )
	{
		isfalse_ret( LoadSocketExAPI(hSocket), FALSE );

		istrue_ret(
			false == ClientOption(hSocket, eTCP),
			FALSE );

		struct sockaddr_in RemoteAddr;
		istrue_ret(
			false == SockAddr(RemoteAddr, rHost, iPort),
			FALSE );

		struct sockaddr_in LocalAddr;
		int iLocalAddrLen = sizeof(LocalAddr);
		ZeroMemory( &LocalAddr, sizeof(LocalAddr) );
		LocalAddr.sin_family = AF_INET;
		istrue_ret(
			SOCKET_ERROR == ::bind(hSocket, (struct sockaddr*)&LocalAddr, iLocalAddrLen),
			FALSE );
		
		return lpfnConnectEx(
					hSocket,						// Socket
					(struct sockaddr*)&RemoteAddr,	// remote address
					sizeof(RemoteAddr),				// remote address length 
					NULL,							// send buffer
					0,								// send buffer length
					NULL,							// sent bytes
					lpOverlapped);
	}
};