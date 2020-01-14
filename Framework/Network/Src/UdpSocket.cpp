#include "Precompiled.h"
#include "UdpSocket.h"

using namespace Coo::Network;

bool UdpSocket::Open()
{
	ASSERT(mSocket == INVALID_SOCKET, "Socket already opened!");
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == INVALID_SOCKET)
	{
		LOG("Failed to open socket. Error type:%d", WSAGetLastError());
		return false;
	}
	return true;
}

void UdpSocket::Close()
{
	if (mSocket != INVALID_SOCKET)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}
}

bool UdpSocket::Bind(const SocketAddress & address)
{
	if (mSocket == INVALID_SOCKET && !Open())
	{
		return false;
	}
	if (bind(mSocket, &address.mSockAddr, static_cast<int>(address.GetSize())) == SOCKET_ERROR)
	{
		LOG("Failed to bind. Error type:%d", WSAGetLastError());
		return false;
	}
	return true;
}

bool UdpSocket::SetBroadcast(bool server)
{
	if (mSocket == INVALID_SOCKET && !Open())
	{
		return false;
	}
	char broadcast = '1';
	auto option = server ? SO_BROADCAST : SO_REUSEADDR;
	if (setsockopt(mSocket, SOL_SOCKET, option, &broadcast, sizeof(broadcast)) < 0)
	{
		LOG("Failed to set broadcast option. Error type:%d", WSAGetLastError());
		return false;
	}
	return true;
}

bool UdpSocket::SetNonblocking(bool nonBlocking)
{
	ASSERT(mSocket != INVALID_SOCKET, "Socket not opened!");
	u_long arg = nonBlocking ? 1 : 0;
	int iResult = ioctlsocket(mSocket, FIONBIO, &arg);
	if (iResult == SOCKET_ERROR)
	{
		LOG("set non blocking failed. Error type:%d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

int UdpSocket::SendTo(const uint8_t * buffer, size_t length, const SocketAddress& toAddress)
{
	int sent = sendto(mSocket, reinterpret_cast<const char*>(buffer), static_cast<int>(length), 0, &toAddress.mSockAddr, static_cast<int>(toAddress.GetSize()));
	if (sent < 0)
	{
		LOG("sendto failed. Error type:%d", WSAGetLastError());
	}
	return sent;
}

int UdpSocket::RecvFrom(uint8_t * buffer, size_t length, const SocketAddress& fromAddress)
{
	int addrlen = static_cast<int>(fromAddress.GetSize());
	int bytesReceived = recvfrom(mSocket, reinterpret_cast<char*>(buffer), static_cast<int>(length), 0, (LPSOCKADDR)&fromAddress.mSockAddr, &addrlen);

	if (bytesReceived >= 0)
	{
		return bytesReceived;
	}

	int error = WSAGetLastError();
	if (error == WSAEWOULDBLOCK)
	{
		return 0;
	}

	if (error == WSAECONNRESET)
	{
		// this can happen if a client closed and we haven't DC yet
		// this is the ICMP message being sent back saying the port on that computer is closed
		LOG("Connection reset");
		Close();
		return -WSAECONNRESET;
	}

	LOG("recvfrom failed. Error type:%d", error);
	Close();
	return -error;
}
