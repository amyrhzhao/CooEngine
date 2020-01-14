#include "Precompiled.h"
#include "TcpSocket.h"

using namespace Coo::Network;

bool TcpSocket::Open()
{
	ASSERT(mSocket == INVALID_SOCKET, "Socket already opened!");
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		LOG("Failed to open socket. Error type:%d", WSAGetLastError());
		return false;
	}
	return true;
}

void TcpSocket::Close()
{
	if (mSocket != INVALID_SOCKET)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}
}

bool TcpSocket::SetNoDelay(bool noDelay)
{
	ASSERT(mSocket != INVALID_SOCKET, "Socket not opened!");
	int delayArg = noDelay ? 1 : 0;
	int iResult = setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&delayArg, sizeof(int));
	if (iResult == SOCKET_ERROR)
	{
		LOG("set no delay failed. Error type:%d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

bool TcpSocket::SetNonBlocking(bool nonBlocking)
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

bool TcpSocket::Connect(const SocketAddress & address)
{
	if (mSocket == INVALID_SOCKET && !Open())
	{
		return false;
	}
	int iResult = connect(mSocket, (LPSOCKADDR)&address.mSockAddr, static_cast<int>(address.GetSize()));
	if (iResult == SOCKET_ERROR)
	{
		int lastError = WSAGetLastError();
		// Only used in non blocking socket
		// check the error if it is because of non blocking
		if (lastError == WSAEWOULDBLOCK)
		{
			fd_set write, err;
			FD_ZERO(&write);
			FD_ZERO(&err);
			FD_SET(mSocket, &write);
			FD_SET(mSocket, &err);

			TIMEVAL timeout = { 10,0 }; // timeout after 10 seconds
			int result = select(0, NULL, &write, &err, &timeout);
			if (result == 0)
			{
				// Nothing happened in 10 seconds
				LOG("Connect timed out.");
				return false;
			}
			else
			{
				// socket is ready
				if (FD_ISSET(mSocket, &write))
				{
					return true;
				}

				if (FD_ISSET(mSocket, &err))
				{
					LOG("Select error.");
					return false;
				}
			}
		}
		else
		{
			LOG("Connect failed. Error type:%d", lastError);
			Close();
			return false;

		}
	}
	return true;
}

bool TcpSocket::Bind(const SocketAddress & address)
{
	if (mSocket == INVALID_SOCKET && !Open())
	{
		return false;
	}
	int iResult = bind(mSocket, (LPSOCKADDR)&address.mSockAddr, static_cast<int>(address.GetSize()));
	if (iResult == SOCKET_ERROR)
	{
		LOG("bind failed. Error type:%d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

bool TcpSocket::Listen(int backLog)
{
	int iResult = listen(mSocket, backLog);
	if (iResult == SOCKET_ERROR)
	{
		LOG("listen failed. Error type:%d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

std::unique_ptr<TcpSocket> TcpSocket::Accept(SocketAddress & fromAddress)
{
	int addrlen = static_cast<int>(fromAddress.GetSize());
	SOCKET client = accept(mSocket, (LPSOCKADDR)&fromAddress.mSockAddr, &addrlen);
	if (client == INVALID_SOCKET)
	{
		int lastError = WSAGetLastError();
		if (lastError != WSAEWOULDBLOCK)
		{
			LOG("accept failed. Error type:%d", WSAGetLastError());
			Close();
		}
		return nullptr;
	}

	std::unique_ptr<TcpSocket> newSocket = std::make_unique<TcpSocket>();
	newSocket->mSocket = client;
	return newSocket;
}

int TcpSocket::Send(const uint8_t * buffer, size_t length)
{
	int sent = send(mSocket, reinterpret_cast<const char*>(buffer), static_cast<int>(length), 0);
	if (sent == SOCKET_ERROR)
	{
		LOG("send failed. Error type:%d", WSAGetLastError());
	}
	return sent;
}

int TcpSocket::Receive(uint8_t * buffer, size_t length)
{
	int bytesReceived = recv(mSocket, reinterpret_cast<char*>(buffer), static_cast<int>(length), 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		int lastError = WSAGetLastError();
		if (lastError == WSAEWOULDBLOCK)
		{
			// If we have SOCKET_ERROR because we are non blocking
			// just pretend we have 0 zero
			bytesReceived = 0;
		}
		else
		{
			LOG("received failed. Error type:%d", WSAGetLastError());
			Close();
			return SOCKET_ERROR;
		}
	}
	return bytesReceived;
}