#include "SheepManager.h"

using namespace Coo::Math;

namespace 
{
	std::unique_ptr<SheepManager> sManager;
}

void SheepManager::StaticInitialize()
{
	sManager = std::make_unique<SheepManager>();
}

void SheepManager::StaticTerminate()
{
	if(sManager)
	{
		sManager->Terminate();
		sManager.reset();
	}
}

SheepManager * SheepManager::Get()
{
	return sManager.get();
}

void SheepManager::Initialize(Coo::AI::AIWorld& world)
{
	auto graphicSystem = Coo::Graphics::GraphicsSystem::Get();
	float screenWidth = static_cast<float>(graphicSystem->GetBackBufferWidth());
	float screenHeight = static_cast<float>(graphicSystem->GetBackBufferHeight());
	sheeps.reserve(sheepCount);
	for (int i = 0; i < sheepCount; ++i) 
	{
		auto& sheep = sheeps.emplace_back(std::make_unique<Sheep>(world));
		sheep->Initialize({ RandomFloat(0.0f, screenWidth),RandomFloat(0.0f, screenHeight) });
	}
}

void SheepManager::Terminate()
{
	for(auto& sheep : sheeps)
	{
		sheep->Terminate();
		sheep.reset();
	}
}

void SheepManager::Render() const
{
	for (auto& sheep : sheeps)
	{
		sheep->Render();
	}
}

void SheepManager::Update(float deltaTime)
{
	for (auto& sheep : sheeps)
	{
		sheep->Update(deltaTime);
	}
}

