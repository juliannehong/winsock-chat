#include "Socket.h"

void SocketBase::SetSocketAddress(const char* ip_string, u_short host)
{
	m_sockaddr.sin_addr.S_un.S_addr = inet_addr(ip_string);
	m_sockaddr.sin_port = htons(host);
	m_sockaddr.sin_family = AF_INET; // IPv4
}

sockaddr_in SocketBase::GetSocketAddress()
{
	return m_sockaddr;
}

ULONG SocketBase::GetHostAddress()
{
	return m_sockaddr.sin_addr.S_un.S_addr;
}

u_short SocketBase::GetPortNumber()
{
	return m_sockaddr.sin_port;
}