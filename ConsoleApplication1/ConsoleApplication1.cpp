#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define BUFFSIZE 512

bool StartWSA()
{
	WSADATA wasData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wasData);
	if (iResult != 0)
	{
		std::cout << "WSA Startup failed." << std::endl;
		return false;
	}
	else
	{
		std::cout << "WAS successfully started." << std::endl;
	}

	return true;
}

int main()
{
	if (!StartWSA())
		return 1;

	SOCKET connectSocket = INVALID_SOCKET;
	char* message = "this is a test";
	int err = 0;
	char recvbuff[BUFFSIZE];
	err = getaddrinfo("127.0.0.1", "27015", )

}