#pragma once
// SocketBase.h

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")


class SocketBase
{
	
public:
	SocketBase() {};
	~SocketBase() {};

	void SetSocketAddress(const char* ip_string, u_short port);
	ULONG GetHostAddress();
	u_short GetPortNumber();
	sockaddr_in GetSocketAddress();

};

class ClientSocket : public SocketBase
{
public:
	ClientSocket() {};
	~ClientSocket() {};
private:
	sockaddr_in client_addr;
};