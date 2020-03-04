#ifndef INCLUDED_COOENGINE_CORE_META_H
#define INCLUDED_COOENGINE_CORE_META_H

#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "MetaPointer.h"
#include "MetaRegistry.h"
#include "MetaType.h"
#include "MetaUtil.h"

#define META_TYPE_DECLARE(Type)\
	template<> const Coo::Core::Meta::MetaType* Coo::Core::Meta::DeduceType<Type>();

#define META_TYPE_DEFINE(Type, Name)\
	template<> const Coo::Core::Meta::MetaType* Coo::Core::Meta::DeduceType<Type>()\
	{\
		static const Coo::Core::Meta::MetaType sMetaType(\
			#Name, Coo::Core::Meta::MetaType::Category::Primitive, sizeof(Type),\
			[] (void) {return new Type;},\
			[] (void* data) {delete static_cast<Type*>(data);},\
			nullptr,\
			Coo::Core::Meta::Deserialize<Type>);\
		return &sMetaType;\
	}

#define META_CLASS_DECLARE\
	static const Coo::Core::Meta::MetaClass* StaticGetMetaClass();\
	virtual const Coo::Core::Meta::MetaClass* GetMetaClass() const { return StaticGetMetaClass(); }

#define META_DERIVED_BEGIN(ClassType, ParentType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const Coo::Core::Meta::MetaClass* parentMetaClass = ParentType::StaticGetMetaClass();

#define META_CLASS_BEGIN(ClassType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const Coo::Core::Meta::MetaClass* parentMetaClass = nullptr;

#define META_CLASS_BEGIN_INTERNAL(ClassType)\
	template <> const Coo::Core::Meta::MetaType* Coo::Core::Meta::DeduceType<ClassType>()\
	{\
		return ClassType::StaticGetMetaClass();\
	}\
	const Coo::Core::Meta::MetaClass* ClassType::StaticGetMetaClass()\
	{\
		using Class = ClassType;\
		const char* className = #ClassType;

#define META_FIELD_BEGIN\
		static const std::initializer_list<Coo::Core::Meta::MetaField> fields {

#define META_FIELD(Var, Name)\
			{ Name, Coo::Core::Meta::DeduceMemberType(&Class::Var), Coo::Core::Meta::GetFieldOffset(&Class::Var) },

#define META_FIELD_END\
		};

#define META_NO_FIELD\
		static const std::initializer_list<Coo::Core::Meta::MetaField> fields;

#define META_CLASS_END\
		static const Coo::Core::Meta::MetaClass sMetaClass(\
			className, sizeof(Class),\
			[] { return new Class(); },\
			[](void* data) { delete static_cast<Class*>(data); },\
			parentMetaClass, fields);\
		return &sMetaClass;\
	}

#endif // !INCLUDED_COOENGINE_CORE_META_H
