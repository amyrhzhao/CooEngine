#include "Precompiled.h"
#include "MetaType.h"
#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaPointer.h"

using namespace Coo::Core::Meta;

Coo::Core::Meta::MetaType::MetaType(const char * name, Category category, size_t size, CreateFunc create, DestroyFunc destroy, SerializeFunc serialize, DeserializeFunc deserialize)
	: mName(name), mCategory(category), mSize(size), mCreate(std::move(create)), mDestory(std::move(destroy)), mSerialize(std::move(serialize)), mDeserialize(std::move(deserialize))
	{
	}


void* Coo::Core::Meta::MetaType::Create() const
{
	ASSERT(mCreate, "no creation callable for '%s'.", GetName());
	return mCreate();
}

void Coo::Core::Meta::MetaType::Destroy(void* data) const
{
	ASSERT(mDestory, "no destroy callable for '%s'.", GetName());
	return mDestory(data);
}

void Coo::Core::Meta::MetaType::Serialize(const void* instance, rapidjson::Value & jsonValue) const
{
	ASSERT(mSerialize, "no serialize callable for '%s'.", GetName());
	return mSerialize(instance, jsonValue);
}

void Coo::Core::Meta::MetaType::Deserialize(void* instance, const rapidjson::Value & jsonValue) const
{
	ASSERT(mDeserialize, "no deserialize callable for '%s'.", GetName());
	return mDeserialize(instance, jsonValue);
}

//const MetaArray* Coo::Core::Meta::MetaType::GetMetaArray() const
//{
//	ASSERT(mCategory == Category::Array, "[MetaType] Invalid type cast.");
//	return static_cast<const MetaArray*>(this);
//}
//
//const MetaClass* Coo::Core::Meta::MetaType::GetMetaClass() const
//{
//	ASSERT(mCategory == Category::Class, "[MetaType] Invalid type cast.");
//	return static_cast<const MetaClass*>(this);
//}
//
//const MetaPointer* Coo::Core::Meta::MetaType::GetMetaPointer() const
//{
//	ASSERT(mCategory == Category::Pointer, "[MetaType] Invalid type cast.");
//	return static_cast<const MetaPointer*>(this);
//}

