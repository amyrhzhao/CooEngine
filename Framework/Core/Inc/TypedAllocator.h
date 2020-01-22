#ifndef INCLUDED_COOENGINE_CORE_TYPEDALLOCATOR_H
#define INCLUDED_COOENGINE_CORE_TYPEDALLOCATOR_H

#include "BlockAllocator.h"

namespace Coo::Core 
{
	template<class DataType>
	class TypedAllocator : BlockAllocator
	{
		TypedAllocator(size_t capacity) :
			BlockAllocator(sizeof(DataType), capacity) {}

		template<class... Args>
		DataType* New(Args&&... args);
		
		void Delete(DataType* ptr);
	};
	
	template<class DataType>
	template<class ...Args>
	inline DataType * TypedAllocator<DataType>::New(Args&& ...args)
	{
		DataType* mem = static_cast<DataType>(Allocate());
		if (mem != nullptr)
		{
			new (mem) DataType(std::forward<Args>(args)...);
		}
		return mem;
	}
	
	template<class DataType>
	inline void TypedAllocator<DataType>::Delete(DataType* ptr)
	{
		if (ptr == nullptr) { return; }

		ptr->~DataType();
		Free(ptr);
	}
} // namespace Coo::Core

#endif // !INCLUDED_COOENGINE_CORE_TYPEDALLOCATOR_H
