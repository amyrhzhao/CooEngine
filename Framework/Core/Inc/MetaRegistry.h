#ifndef INCLUDED_COOENGINE_CORE_METAREGISTRY_H
#define INCLUDED_COOENGINE_CORE_METAREGISTRY_H

namespace Coo::Core::Meta
{
	class MetaClass;

	void Register(const MetaClass* metaClass);

	const MetaClass* FindMetaClass(const std::string& className);

} // namespace Coo::Core::Meta 

#define META_REGISTER(Class)\
	Coo::Core::Meta::Register(Class::StaticGetMetaClass());

#endif // !INCLUDED_COOENGINE_CORE_METAREGISTRY_H
