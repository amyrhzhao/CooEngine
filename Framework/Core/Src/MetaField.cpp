#include "Precompiled.h"
#include "MetaField.h"

using namespace Coo::Core::Meta;

MetaField::MetaField(const char* name, const MetaType* type, size_t offset)
	:mType(type), mName(name), mOffset(offset)
{
}
