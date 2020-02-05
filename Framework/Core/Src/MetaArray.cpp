#include "Precompiled.h"
#include "MetaArray.h"

using namespace Coo::Core::Meta;

MetaArray::MetaArray(const MetaType * elementType, GetElementFunc getElement, GetCountFunc getCount)
	: MetaType("Array", MetaType::Category::Array, sizeof(std::vector<int>), nullptr, nullptr)
	, mElementType(elementType)
	, mGetElement(std::move(getElement))
	, mGetCount(std::move(getCount))
{
}
