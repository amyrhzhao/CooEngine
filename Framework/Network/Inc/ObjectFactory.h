#ifndef INCLUDED_COOENGINE_NETWORK_OBJECTFACTORY_H
#define INCLUDED_COOENGINE_NETWORK_OBJECTFACTORY_H

#include "Common.h"

// Macro to add Runtime-Type-Information
#define RTTI(ClassName)\
static std::string GetTypeName() { return #ClassName; }\
static std::size_t GetTypeId() { return std::hash<std::string>{}(GetTypeName()); }\
static void* CreateInstance() { return new ClassName{}; }

namespace Coo::Network
{

	class ObjectFactory
	{
	public:
		template <class T>
		void Register()
		{
			mCreators[T::GetTypeId()] = T::CreateInstance;
		}

		template <class T>
		T* CreateInstance()
		{
			return reinterpret_cast<T*>(CreateInstance(T::GetTypeId()));
		}

		void* CreateInstance(std::size_t typeId)
		{
			ASSERT(mCreators.find(typeId) != mCreators.end(),
				"Type id %d not registered.", static_cast<int>(typeId));
			return mCreators[typeId]();
		}

	private:
		std::unordered_map<std::size_t, std::function<void*()>> mCreators;
	};

} // namespace Network


#endif // !INCLUDED_COOENGINE_NETWORK_OBJECTFACTORY_H
