#ifndef INCLUDED_COOENGINE_GAMEOBJECTFACTORY_H
#define INCLUDED_COOENGINE_GAMEOBJECTFACTORY_H

#include "GameObject.h"

namespace Coo 
{
	class GameObjectFactory 
	{
	public:
		GameObjectFactory(GameObjectAllocator& allocator);
		GameObject* Create(const char* templateFileName);
		void Destroy(GameObject* gameObject);

	private:
		GameObjectAllocator& mGameObjectAllocator;
	};
}

#endif // !INCLUDED_COOENGINE_GAMEOBJECTFACTORY_H
