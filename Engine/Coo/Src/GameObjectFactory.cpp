#include "Precompiled.h"
#include "GameObjectFactory.h"
#include <RapidJSON/document.h>
#include <RapidJSON/filereadstream.h>

using namespace Coo;
using namespace rapidjson;

namespace 
{
	void Deserialize(void* instance, const Core::Meta::MetaArray* metaClass, const Value& jsonValue) 
	{

	}

	void Deserialize(void* instance, const Core::Meta::MetaPointer* metaClass, const Value& jsonValue)
	{

	}

	void Deserialize(void* instance, const Core::Meta::MetaType* metaClass, const Value& jsonValue) 
	{

	}

	void Deserialize(void* instance, const Core::Meta::MetaClass* metaClass, const Value& jsonValue) 
	{
		for (auto& member : jsonValue.GetObjectW())
		{
			auto metaField = metaClass->FindField(member.name.GetString());
			auto metaType = metaField->GetMetaType();

			void* instanceField = static_cast<uint8_t*>(instance) + metaField->GetOffset();
			switch (metaType->GetCategory())
			{
			case Core::Meta::MetaType::Category::Primitive:
				Deserialize(instanceField, metaType, member.value);
				break;
			case Core::Meta::MetaType::Category::Class: 
				Deserialize(instanceField, metaType->GetMetaClass(), member.value);
				break;
			case Core::Meta::MetaType::Category::Array: 
				Deserialize(instanceField, metaType->GetMetaArray(), member.value);
				break;
			case Core::Meta::MetaType::Category::Pointer: 
				Deserialize(instanceField, metaType->GetMetaPointer(), member.value);
				break;
			}
			metaField->GetName();
			metaField->GetMetaType();
		}
	}
}

Coo::GameObjectFactory::GameObjectFactory(GameObjectAllocator& allocator)
	: mGameObjectAllocator(allocator)
{

}

GameObject* Coo::GameObjectFactory::Create(const char * templateFileName)
{
	GameObject* gameObject = mGameObjectAllocator.New();
	if (gameObject != nullptr) 
	{
		FILE* file = nullptr;
		fopen_s(&file, templateFileName, "r");

		char readBuffer[65536];
		FileReadStream is(file, readBuffer, sizeof(readBuffer));

		Document document;
		document.ParseStream(is);

		if (document.HasMember("GameObject") && document["GameObject"].IsObject())
		{
			auto jsonObject = document["GameObject"].GetObjectW();
			if (jsonObject.HasMember("Components") && jsonObject["Components"].IsObject()) 
			{
				auto components = jsonObject["Components"].GetObjectW();
				for (auto& component : components) 
				{
					auto metaClass = Core::Meta::FindMetaClass(component.name.GetString());
					//const char* name = metaClass->GetName();
					auto newComponent = gameObject->AddComponent(metaClass);
					ASSERT(newComponent, "[GameObjectFactory] Failed to create component %s.", component.name.GetString());
					Deserialize(newComponent, metaClass, component.value);
				}
			}
		}

		fclose(file);
	}
	return nullptr;
}

void Coo::GameObjectFactory::Destroy(GameObject * gameObject)
{
	mGameObjectAllocator.Delete(gameObject);
}
