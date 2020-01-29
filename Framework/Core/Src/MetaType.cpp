#include "Precompiled.h"
#include "MetaType.h"

using namespace Coo::Core::Meta;

MetaType::MetaType(const char * name, Category category, size_t size)
	: mName(name), mCategory(category), mSize(size)
{
}
