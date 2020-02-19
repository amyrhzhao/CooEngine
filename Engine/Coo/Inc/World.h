#ifndef INCLUDED_COOENGINE_WORLD_H
#define INCLUDED_COOENGINE_WORLD_H

#include "GameObjectFactory.h"

namespace Coo 
{
	class World 
	{
	public:
		void Initialize(size_t capacity);
		void Terminate();

		GameObjectHandle Create(const std::filesystem::path& templateFileName, std::string name);
		GameObjectHandle Find(const std::string& name);
		void Destroy(GameObjectHandle handle);

		void Update(float deltaTime);
		void Render();
		void DebugUI();

	private:
		void DestroyInternal(GameObject* gameObject);
		void ProcessDestroyList();

		using GameObjectList = std::vector<GameObject*>;

		std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
		std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;
		std::unique_ptr<GameObjectFactory> mGameObjectFactory;
	
		GameObjectList mUpdateList;
		GameObjectList mDestroyList;
		bool mInitialized = false;
		bool mUpdating = false;
	};
}

#endif // !INCLUDED_COOENGINE_WORLD_H
