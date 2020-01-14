#ifndef INCLUDED_COOENGINE_NETWORK_TCPSOCKET_H
#define INCLUDED_COOENGINE_NETWORK_TCPSOCKET_H

#include "SocketAddress.h"

namespace Coo::Network 
{
	class TcpSocket
	{
	public:
		TcpSocket() = default;
		bool Open();
		void Close();
		bool SetNoDelay(bool noDelay);
		bool SetNonBlocking(bool nonBlocking);

		bool Connect(const SocketAddress& address);
		bool Bind(const SocketAddress& address);
		bool Listen(int backLog = 10);
		std::unique_ptr<TcpSocket> Accept(SocketAddress& fromAddress);

		int Send(const uint8_t* buffer, size_t length);
		int Receive(uint8_t* buffer, size_t length);

	protected:
		TcpSocket(const TcpSocket&) = delete;
		TcpSocket& operator=(const TcpSocket&) = delete;

		SOCKET mSocket{ INVALID_SOCKET };
	};

} // namespace Coo::Network

#endif // !INCLUDED_COOENGINE_NETWORK_TCPSOCKET_H
