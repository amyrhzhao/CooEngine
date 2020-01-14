#include "Precompiled.h"
#include "LinkingContext.h"

using namespace Coo::Network;

int LinkingContext::GetNetworkId(void* instance) const
{
	auto got = mInstanceToNetworkIdMap.find(instance);
	int ret = InvalidId;
	if (got != mInstanceToNetworkIdMap.end())
	{
		ret = got->second;
	}
	return ret;
}

void* LinkingContext::GetInstance(int networkId) const
{
	auto got = mNetworkIdToInstanceMap.find(networkId);
	void* ret = nullptr;
	if (got != mNetworkIdToInstanceMap.end())
	{
		ret = got->second;
	}
	return ret;
}

int LinkingContext::Register(void* instance)
{
	auto id = GetNetworkId(instance);
	if (id == InvalidId)
	{
		id = mNextNetworkId++;
	}
	Register(instance, id);
	return id;
}

void LinkingContext::Register(void* instance, int networkId)
{
	ASSERT(instance != nullptr, "Can not register a nullptr.");
	auto id = GetNetworkId(instance);
	if (id == InvalidId)
	{
		Unregister(GetInstance(id));
	}
	mInstanceToNetworkIdMap.emplace(instance, networkId);
	mNetworkIdToInstanceMap.emplace(networkId, instance);
}

void LinkingContext::Unregister(void* instance)
{
	int id = GetNetworkId(instance);
	if (id != InvalidId)
	{
		mNetworkIdToInstanceMap.erase(id);
		mInstanceToNetworkIdMap.erase(instance);
	}
}
