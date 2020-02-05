#ifndef INCLUDED_COOENGINE_CORE_METATYPE_H
#define INCLUDED_COOENGINE_CORE_METATYPE_H

#include "Debug.h"

namespace Coo::Core::Meta
{
	class MetaType
	{
	public:
		using CreateFunc = std::function<void*()>;
		using DestroyFunc = std::function<void(void*)>;

		enum class Category
		{
			Primitive,
			Class,
			Array,
			Pointer
		};

		MetaType(const char* name, Category category, size_t size, CreateFunc create, DestroyFunc destroy);

		const char* GetName() const { return mName.c_str(); }
		Category GetCategory() const { return mCategory; }
		size_t GetSize() const { return mSize; }
		
		void* Create() const 
		{
			ASSERT(mCreate, "no creation callable for '%s'.", GetName());
			return mCreate(); 
		}

		void Destroy(void* data) const 
		{
			ASSERT(mDestory, "no destroy callable for '%s'.", GetName());
			return mDestory(data); 
		}

	private:
		const std::string mName;
		const Category mCategory;
		const size_t mSize;
		CreateFunc mCreate;
		DestroyFunc mDestory;
	};

} // namespace Coo::Core::Meta

#endif // !INCLUDED_COOENGINE_CORE_METATYPE_H
