#include "Precompiled.h"
#include "MetaPointer.h"

using namespace Coo::Core::Meta;

MetaPointer::MetaPointer(const MetaType* pointerType)
	: MetaType("Pointer", MetaType::Category::Pointer, sizeof(nullptr), nullptr, nullptr)
	, mPointerType(pointerType)
{
}
