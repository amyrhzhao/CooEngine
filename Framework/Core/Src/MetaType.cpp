#include "Precompiled.h"
#include "MetaType.h"

using namespace Coo::Core::Meta;

MetaType::MetaType(const char * name, Category category, size_t size, CreateFunc create, DestroyFunc destroy)
	: mName(name), mCategory(category), mSize(size), mCreate(std::move(create)), mDestory(std::move(destroy))
{
}
