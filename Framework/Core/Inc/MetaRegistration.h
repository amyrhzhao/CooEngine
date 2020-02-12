#ifndef INCLUDED_COOENGINE_CORE_METAREGISTRATION_H
#define INCLUDED_COOENGINE_CORE_METAREGISTRATION_H

#include "Meta.h"

// Primitive Type Declarations
META_TYPE_DECLARE(int)
META_TYPE_DECLARE(float)
META_TYPE_DECLARE(bool)
META_TYPE_DECLARE(std::string)

namespace Coo::Core 
{
	void StaticRegister();

} // namespace Coo::Core

#endif //!INCLUDED_COOENGINE_CORE_METAREGISTRATION_H