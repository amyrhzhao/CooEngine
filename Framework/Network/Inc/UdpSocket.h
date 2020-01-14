#ifndef INCLUDED_COOENGINE_NETWORK_UDPSOCKET_H
#define INCLUDED_COOENGINE_NETWORK_UDPSOCKET_H

#include "SocketAddress.h"

namespace Coo::Network
{
	class UdpSocket
	{
	public:
		UdpSocket() = default;
		bool Open();
		void Close();

		bool Bind(const SocketAddress& address);
		bool SetBroadcast(bool server);
		bool SetNonblocking(bool nonBlocking);

		int SendTo(const uint8_t* buffer, size_t length, const SocketAddress& toAddress);
		int RecvFrom(uint8_t* buffer, size_t length, const SocketAddress& fromAddress);

	private:
		UdpSocket(const UdpSocket&) = delete;
		UdpSocket& operator=(const UdpSocket&) = delete;

		SOCKET mSocket{ INVALID_SOCKET };
	};

} // namespace Coo::Network


#endif // !INCLUDED_COOENGINE_NETWORK_UDPSOCKET_H
