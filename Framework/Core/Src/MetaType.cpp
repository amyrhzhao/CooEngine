#include "Precompiled.h"
#include "MetaType.h"
#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaPointer.h"

using namespace Coo::Core::Meta;

MetaType::MetaType(const char * name, Category category, size_t size, CreateFunc create, DestroyFunc destroy)
	: mName(name), mCategory(category), mSize(size), mCreate(std::move(create)), mDestory(std::move(destroy))
{
}

const MetaArray* Coo::Core::Meta::MetaType::GetMetaArray() const
{
	ASSERT(mCategory == Category::Array, "[MetaType] Invalid type cast.");
	return static_cast<const MetaArray*>(this);
}

const MetaClass* Coo::Core::Meta::MetaType::GetMetaClass() const
{
	ASSERT(mCategory == Category::Class, "[MetaType] Invalid type cast.");
	return static_cast<const MetaClass*>(this);
}

const MetaPointer* Coo::Core::Meta::MetaType::GetMetaPointer() const
{
	ASSERT(mCategory == Category::Pointer, "[MetaType] Invalid type cast.");
	return static_cast<const MetaPointer*>(this);
}
