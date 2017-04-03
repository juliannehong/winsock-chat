#include "Socket.h"
#include <memory>

bool StartWSA();

int main()
{
	if (!StartWSA())
		return 1;

	int errcode(0);

	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;

	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_port = 27015;

	errcode = bind(listenSocket, (SOCKADDR*) &addr, sizeof(addr));
	if (errcode == SOCKET_ERROR)
		std::cout << "first fail" << std::endl;

	errcode = listen(listenSocket, SOMAXCONN);
	if (errcode == SOCKET_ERROR)
		std::cout << "second fail" << std::endl;

	clientSocket = accept(listenSocket, nullptr, nullptr);
	if (clientSocket == INVALID_SOCKET)
		std::cout << "Client connected" << std::endl;


	closesocket(listenSocket); // no longer need listen
	/*std::unique_ptr<ClientSocket> client_ptr;
	client_ptr->SetSocketAddress("127.0.0.1", 43);
	ULONG ad = client_ptr->GetHostAddress();

	std::cout << ad << std::endl;
*/
	system("pause");
	
	return 0;
}

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