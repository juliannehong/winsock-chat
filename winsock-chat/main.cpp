#include "Socket.h"
#include <memory>

bool StartWSA();

int main()
{
	if (!StartWSA())
		return 1;

	std::unique_ptr<ClientSocket> client_ptr;
	client_ptr->SetSocketAddress("127.0.0.1", 43);
	ULONG ad = client_ptr->GetHostAddress();

	std::cout << ad << std::endl;

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