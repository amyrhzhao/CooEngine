#include "Precompiled.h"
#include "GameObjectFactory.h"

using namespace Coo;

Coo::GameObjectFactory::GameObjectFactory(GameObjectAllocator& allocator)
	: mGameObjectAllocator(allocator)
{

}

GameObject* Coo::GameObjectFactory::Create(const char * templateFileName)
{
	GameObject* gameObject = mGameObjectAllocator.New();
	if (gameObject != nullptr) 
	{

	}
	return nullptr;
}

void Coo::GameObjectFactory::Destroy(GameObject * gameObject)
{
	mGameObjectAllocator.Delete(gameObject);
}
