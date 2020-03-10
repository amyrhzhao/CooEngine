#include "Precompiled.h"
#include "World.h"

using namespace Coo;

void World::Initialize(size_t capacity)
{
	ASSERT(!mInitialized, "[World] World already initialized");

	for (auto& service : mServices) 
		service->Initialize();

	mGameObjectAllocator = std::make_unique<GameObjectAllocator>(capacity);
	mGameObjectHandlePool = std::make_unique<GameObjectHandlePool>(capacity);
	mGameObjectFactory = std::make_unique<GameObjectFactory>(*mGameObjectAllocator);

	mInitialized = true;
}

void World::Terminate()
{
	if (!mInitialized) { return; }

	ASSERT(!mUpdating, "[World] Cannot terminate world during update.");
	
	// Destroy all active objects
	mUpdating = true;
	std::for_each(mUpdateList.begin(), mUpdateList.end(), [this](auto gameObject)
	{
		Destroy(gameObject->GetHandle());
	});
	mUpdating = false;
	mUpdateList.clear();

	// Destroy everything
	ProcessDestroyList();

	mGameObjectAllocator.reset();
	mGameObjectHandlePool->Flush();
	mGameObjectHandlePool.reset();
	mGameObjectFactory.reset();

	for (auto& service : mServices)
	{
		service->Terminate();
	}

	mInitialized = false;
}

GameObjectHandle Coo::World::Create(const std::filesystem::path&  templateFileName, std::string name)
{
	auto gameObject = mGameObjectFactory->Create(templateFileName.u8string().c_str());
	ASSERT(gameObject != nullptr, "[World] Failed to create game object from template %s", templateFileName.u8string().c_str());

	// Register with the handle pool
	auto handle = mGameObjectHandlePool->Register(gameObject);

	// Initialize the game object
	gameObject->mWorld = this;
	gameObject->mHandle = handle;
	gameObject->mName = std::move(name);
	gameObject->Initialize();

	// Add game object to the update list
	mUpdateList.push_back(gameObject);

	return handle;
}

GameObjectHandle Coo::World::Find(const std::string& name)
{
	auto iter = std::find_if(std::begin(mUpdateList), std::end(mUpdateList), [&name](auto gameObject) 
		{
			return gameObject->mName == name;
		});
	return (iter != end(mUpdateList)) ? (*iter)->GetHandle() : GameObjectHandle();
}

void Coo::World::Destroy(GameObjectHandle handle)
{
	// If handle is valid, nothing to do so just exit
	if (!handle.IsValid()) { return; }

	// Cache the pointer and unregister with the handle
	GameObject* gameObject = handle.Get();
	mGameObjectHandlePool->Unregister(handle);

	// Check if we can destroy it now
	if (!mUpdating)
		DestroyInternal(gameObject);
	else
		mDestroyList.push_back(gameObject);
}

void World::Update(float deltaTime)
{
	ASSERT(!mUpdating, "[World] Already updating the world.");
	
	for (auto& service : mServices)
		service->Update(deltaTime);

	// Lock the update list
	mUpdating = true;

	// Recompute size in case new objects are added to the update
	// list during iteration
	for (size_t i = 0; i < mUpdateList.size(); ++i) 
	{
		GameObject* gameObject = mUpdateList[i];
		if (gameObject->GetHandle().IsValid()) 
		{
			gameObject->Update(deltaTime);
		}
	}
	
	// Unlock the update list
	mUpdating = false;

	// Safely destroy objects
	ProcessDestroyList();
}

void World::Render()
{
	for (auto& service : mServices)
		service->Render();
	for (auto gameObject : mUpdateList)
		gameObject->Render();
}

void World::DebugUI()
{
	for (auto& service : mServices)
		service->DebugUI();
	for (auto gameObject : mUpdateList)
		gameObject->DebugUI();
}

void Coo::World::DestroyInternal(GameObject* gameObject)
{
	ASSERT(!mUpdating, "[World] Cannot destroy game objects during update");

	// If pointer is invalid, nothing to do so just exit
	if (gameObject == nullptr)
		return;

	// First remove it from the update list
	auto iter = std::find(mUpdateList.begin(), mUpdateList.end(), gameObject);
	if (iter != mUpdateList.end())
		mUpdateList.erase(iter);

	// Terminate the game object
	gameObject->Terminate();

	// Next destroy the game object
	mGameObjectFactory->Destroy(gameObject);
}

void Coo::World::ProcessDestroyList()
{
	for (auto gameObject : mDestroyList) 
		DestroyInternal(gameObject);

	mDestroyList.clear();
}
