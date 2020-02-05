#ifndef INCLUDED_COOENGINE_CORE_METAUTIL_H
#define INCLUDED_COOENGINE_CORE_METAUTIL_H

#include "MetaArray.h"
#include "MetaPointer.h"

namespace Coo::Core::Meta 
{
	class MetaType;

	template <class DataType>
	const MetaType* DeduceType();

	namespace Detail
	{
		template<class DataType>
		inline const MetaType* DeduceTypeImpl(DataType*) 
		{
			// This is for Unexpected Type
			static_assert(sizeof(DataType) == -1, "Cannot deduce type");
		}

		template<class DataType>
		inline const MetaType* DeduceTypeImpl(DataType**) 
		{
			static MetaPointer sMetaPointer(DeduceType<DataType>());
			return &sMetaPointer;
		}

		template<class DataType>
		inline const MetaType* DeduceTypeImpl(std::vector<DataType>*)
		{
			static const auto getElement = [](void* instance, size_t index)->void* 
			{
				auto vector = reinterpret_cast<std::vector<DataType>*>(instance);
				vector->resize(index + 1);
				return vector->data() + index;
			};
			
			static const auto getCount = [](void* instance)-> size_t 
			{
				auto vector = reinterpret_cast<std::vector<DataType>*>(instance);
				return vector->size();
			};
			
			static MetaArray sMetaArray(DeduceType<DataType>(), getElement, getCount);
			return &sMetaArray;
		}

	} // namespace Detail

	template<class DataType>
	const MetaType* DeduceType()
	{
		return Detail::DeduceTypeImpl(static_cast<DataType*>(nullptr));
	}

} // namespace Coo::Core::Meta


#endif // !INCLUDED_COOENGINE_CORE_METAUTIL_H
