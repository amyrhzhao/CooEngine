#include "Precompiled.h"
#include "SocketAddress.h"

using namespace Coo::Network;

SocketAddress::SocketAddress()
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIP4() = INADDR_ANY;
	GetPort() = 0;
}

SocketAddress::SocketAddress(const char* host, uint16_t port)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetPort() = htons(port);
	inet_pton(AF_INET, host, &GetIP4());
}

SocketAddress::SocketAddress(uint16_t port)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIP4() = INADDR_ANY;
	GetPort() = htons(port);
}

const char * SocketAddress::GetIpAddr() const
{
	return inet_ntoa(reinterpret_cast<const sockaddr_in*>(&mSockAddr)->sin_addr);
}
