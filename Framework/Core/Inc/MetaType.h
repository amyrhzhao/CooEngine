#ifndef INCLUDED_COOENGINE_CORE_METATYPE_H
#define INCLUDED_COOENGINE_CORE_METATYPE_H

namespace Coo::Core::Meta
{
	class MetaType
	{
	public:
		enum class Category
		{
			Primitive,
			Class,
			Array,
			Pointer
		};

		MetaType(const char* name, Category category, size_t size);

		const char* GetName() const { return mName; }
		Category GetCategory() const { return mCategory; }
		size_t GetSize() const { return mSize; }

	private:
		const char* mName;
		Category mCategory;
		size_t mSize;
	};

} // namespace Coo::Core::Meta

#endif // !INCLUDED_COOENGINE_CORE_METATYPE_H
