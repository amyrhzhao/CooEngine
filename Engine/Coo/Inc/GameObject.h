#ifndef INCLUDED_COOENGINE_GAMEOBJECT_H
#define INCLUDED_COOENGINE_GAMEOBJECT_H

#include "Component.h"

namespace Coo 
{
	class Component;
	class World;
	class GameObject;

	using GameObjectAllocator = Core::TypedAllocator<GameObject>;
	using GameObjectHandle = Core::Handle<GameObject>;
	using GameObjectHandlePool = Core::HandlePool<GameObject>;

	class GameObject final 
	{
	public:
		META_CLASS_DECLARE;

		void Initialize();
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		void SetName(const char* name) { mName = name; }
		const char* GetName() const { return mName.c_str(); }

		World& GetWorld() { return *mWorld; }
		const World& GetWorld() const { return *mWorld; }

		GameObjectHandle GetHandle() { return mHandle; }

		template<class T, class = std::void_t<std::is_base_of<Component,T>>>
		T* AddComponent()
		{
			auto& newComponent = mComponents.emplace_back(std::make_unique<T>());
			newComponent->mOwner = this;
			return static_cast<T*>(newComponent.get());
		}

		//Component* AddComponent(const char* componentTypeName);
		Component* AddComponent(const Core::Meta::MetaClass* metaClass);
		
		template <class T>
		const T* GetComponent() const 
		{
			for (auto& component : mComponents) 
			{
				if (component->GetMetaClass() == T::StaticGetMetaClass())
					return static_cast<T*>(component.get());
			}
			return nullptr;
		}

		template <class T>
		T* GetComponent() 
		{
			const auto* constMe = this;
			return const_cast<T*>(constMe->GetComponent<T>());
		}

	private:
		friend class World;
		friend class Editor;
		using Components = std::vector<std::unique_ptr<Component>>;

		std::string mName = "NoName";
		World* mWorld = nullptr;
		Components mComponents;
		GameObjectHandle mHandle;
	};

} // namespace Coo

#endif // !INCLUDED_COOENGINE_GAMEOBJECT_H
