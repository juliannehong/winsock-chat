#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char *argv[]) {
	WSADATA wasData;

	int iResult;

	// *** initialize winsock

	// WSAStartup -> called to initiate use of WS2_32.dll
	// MAKEWORD(2,2) -> version2.2 winsock on the system
	iResult = WSAStartup(MAKEWORD(2, 2), &wasData);
	if (iResult != 0)
	{
		std::cout << "It failed" << std::endl;
		return 1;
	}

	// *** create addressinfo and its pointer
	addrinfo* result = nullptr;
	addrinfo* ptr = nullptr;
	addrinfo hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP 
	hints.ai_protocol = IPPROTO_TCP;

#define DEFAULT_PORT "27015"

	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	// result saves addrinfo with IP address returned by the call to getaddrinfo that matched the address family, socket type, and protocol specified in the hints parameter
	if (iResult != 0)
	{
		std::cout << "getaddrinfo failed with error code " << iResult << "." << std::endl;
		WSACleanup();
		return 1;
	}

	// *** create socket
	SOCKET ConnectSocket = INVALID_SOCKET; 
	ptr = result;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		std::cout << "Error at " << WSAGetLastError() << "." << std::endl;
		freeaddrinfo(result); // free the addrpointer result
		WSACleanup();
		return 1;
	}

	// *** connecting socket
	// *** For a client to communicate on a network, it must connect to a server.

	iResult = connect(ConnectSocket, ptr->ai_addr, (int)(ptr->ai_addrlen));
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}


	return 0;
}
