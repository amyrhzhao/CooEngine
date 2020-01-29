#ifndef INCLUDED_COOENGINE_CORE_METACLASS_H
#define INCLUDED_COOENGINE_CORE_METACLASS_H

#include "MetaType.h"

namespace Coo::Core::Meta
{
	class MetaField;

	class MetaClass : public MetaType 
	{
	public:
		MetaClass(const char* name, size_t size);
	};

} // namespace Coo::Core::Meta

#endif // !INCLUDED_COOENGINE_CORE_METACLASS_H
