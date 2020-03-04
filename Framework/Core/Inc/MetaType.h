#ifndef INCLUDED_COOENGINE_CORE_METATYPE_H
#define INCLUDED_COOENGINE_CORE_METATYPE_H

#include "Debug.h"

namespace Coo::Core::Meta
{
	class MetaArray;
	class MetaPointer;
	class MetaClass;

	class MetaType
	{
	public:
		using CreateFunc = std::function<void*()>;
		using DestroyFunc = std::function<void(void*)>;
		using SerializeFunc = std::function<void(const void* instance, rapidjson::Value& jsonValue)>;
		using DeserializeFunc = std::function<void(void* instance, const rapidjson::Value& jsonValue)>;

		enum class Category
		{
			Primitive,
			Class,
			Array,
			Pointer
		};

		MetaType(const char* name, Category category, size_t size, CreateFunc create, DestroyFunc destroy, SerializeFunc serialize = nullptr, DeserializeFunc deserialize = nullptr);

		const char* GetName() const { return mName.c_str(); }
		Category GetCategory() const { return mCategory; }
		size_t GetSize() const { return mSize; }
		
		void* Create() const;
		void Destroy(void* data) const;
		virtual void Serialize(const void* instance, rapidjson::Value& jsonValue) const;
		virtual void Deserialize(void* instance, const rapidjson::Value& jsonValue) const;

		//const MetaArray* GetMetaArray() const;
		//const MetaClass* GetMetaClass() const;
		//const MetaPointer* GetMetaPointer() const;

	private:
		const std::string mName;
		const Category mCategory;
		const size_t mSize;
		const CreateFunc mCreate;
		const DestroyFunc mDestory;
		const SerializeFunc mSerialize;
		const DeserializeFunc mDeserialize;
	};

} // namespace Coo::Core::Meta

#endif // !INCLUDED_COOENGINE_CORE_METATYPE_H
