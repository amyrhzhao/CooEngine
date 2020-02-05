#ifndef INCLUDED_COOENGINE_CORE_META_H
#define INCLUDED_COOENGINE_CORE_META_H

#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "MetaPointer.h"
#include "MetaRegistry.h"
#include "MetaType.h"
#include "MetaUtil.h"

namespace Coo::Core
{
#define META_TYPE_DECLARE(Type)\
	template<> const Meta::MetaType* Meta::DeduceType<Type>();

#define META_TYPE_DEFINE(Type, Name)\
	template<> const Meta::MetaType* Meta::DeduceType<Type>()\
	{\
		static const Meta::MetaType sMetaType(\
			#Name, Meta::MetaType::Category::Primitive, sizeof(Type),\
			[] (void) {return new Type;},\
			[] (void* data) {delete static_cast<Type*>(data);});\
		return &sMetaType;\
	}

	// Primitive Type Declarations
	META_TYPE_DECLARE(int)
	META_TYPE_DECLARE(float)
	META_TYPE_DECLARE(bool)
	META_TYPE_DECLARE(std::string)

} // namespace Coo::Core::Meta

#endif // !INCLUDED_COOENGINE_CORE_META_H
