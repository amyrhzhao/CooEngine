#ifndef INCLUDED_COOENGINE_NETWORK_LINKINGCONTEXT_H
#define INCLUDED_COOENGINE_NETWORK_LINKINGCONTEXT_H

namespace Coo::Network
{
	// A helper class that pairs pointers with ids so
	// we can refer to object instances across a network.
	class LinkingContext
	{
	public:
		static constexpr int InvalidId = -1;

		int GetNetworkId(void* instance) const;
		void* GetInstance(int networkId) const;
		void SetOffset(int clientId) { mNextNetworkId = clientId * 1000; }

		// Register an instance to the next network id
		int Register(void* instance);

		// Register an instance to a specific network id
		void Register(void* instance, int networkId);

		// Unregister an instance
		void Unregister(void* instance);

	private:
		std::unordered_map<int, void*> mNetworkIdToInstanceMap;
		std::unordered_map<void*, int> mInstanceToNetworkIdMap;
		int mNextNetworkId{ 0 };
	};

} // namespace Coo::Network

#endif // !INCLUDED_COOENGINE_NETWORK_LINKINGCONTEXT_H
