#include "Precompiled.h"
#include "MetaRegistry.h"

#include "Debug.h"
#include "MetaClass.h"

using namespace Coo::Core;

namespace
{

	inline auto& GetRegistry()
	{
		using MetaClassMap = std::map<std::string, const Meta::MetaClass*>;
		static MetaClassMap sMetaClassMap;
		return sMetaClassMap;
	}
}

void Meta::Register(const Meta::MetaClass* metaClass)
{
	GetRegistry().emplace(metaClass->GetName(), metaClass);
}

const Meta::MetaClass* Meta::FindMetaClass(const std::string& className)
{
	auto iter = GetRegistry().find(className);
	ASSERT(iter != GetRegistry().end(), "[MetaRegistry] Meta class for %s not found.", className.c_str());
	return iter->second;
}