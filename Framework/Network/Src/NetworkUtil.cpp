#include "Precompiled.h"
#include "NetworkUtil.h"

using namespace Coo::Network;

bool Initialize()
{
	// Initialize Winsock version 2.2
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		LOG("Network -- failed to initialize Winsock.");
		return false;
	}
	return true;
}

bool Terminate()
{
	// Shutdown Winsock
	int result = WSACleanup();
	return result == 0;
}
