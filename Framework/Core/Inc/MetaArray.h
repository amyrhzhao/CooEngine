#ifndef INCLUDED_COOENGINE_CORE_METAARRAY_H
#define INCLUDED_COOENGINE_CORE_METAARRAY_H

#include "MetaType.h"

namespace Coo::Core::Meta 
{
	class MetaArray : public MetaType 
	{
	public:
		using GetElementFunc = std::function<void*(void*, size_t)>;
		using GetCountFunc = std::function<size_t(void*)>;
		
		MetaArray(const MetaType* elementType, GetElementFunc getElement, GetCountFunc getCount);

		const MetaType* GetElementType() const { return mElementType; }
		size_t GetElementCount(void* instance) const { return mGetCount(instance); }
		void* GetElement(void* instance, size_t index) const { return mGetElement(instance, index); }

	private:
		const MetaType* mElementType;
		GetElementFunc mGetElement;
		GetCountFunc mGetCount;
	};

} // namespace Coo::Core::Meta

#endif // !INCLUDED_COOENGINE_CORE_METAARRAY_H

