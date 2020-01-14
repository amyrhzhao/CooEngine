#include "GrassManager.h"

using namespace Coo::Math;

namespace
{
	std::unique_ptr<GrassManager> sManager;
}

void GrassManager::StaticInitialize()
{
	sManager = std::make_unique<GrassManager>();
}

void GrassManager::StaticTerminate()
{
	if (sManager)
	{
		sManager->Terminate();
		sManager.reset();
	}
}

GrassManager* GrassManager::Get()
{
	return sManager.get();
}

void GrassManager::Initialize(Coo::AI::AIWorld & world)
{
	auto graphicSystem = Coo::Graphics::GraphicsSystem::Get();
	float screenWidth = static_cast<float>(graphicSystem->GetBackBufferWidth());
	float screenHeight = static_cast<float>(graphicSystem->GetBackBufferHeight());
	grasses.reserve(grassCount);
	for (int i = 0; i < grassCount; ++i) 
	{
		auto& grass = grasses.emplace_back(std::make_unique<Grass>(world));
		grass->Initialize({ RandomFloat(0.0f, screenWidth),RandomFloat(0.0f, screenHeight) });
	}
}

void GrassManager::Terminate()
{
	for (auto& grass : grasses) 
	{
		grass.reset();
	}
}

void GrassManager::Update()
{
	auto graphicSystem = Coo::Graphics::GraphicsSystem::Get();
	float screenWidth = static_cast<float>(graphicSystem->GetBackBufferWidth());
	float screenHeight = static_cast<float>(graphicSystem->GetBackBufferHeight());
	for (auto& grass : grasses)
	{
		if (!grass->IsActive())
		{
			grass->Initialize({ RandomFloat(0.0f, screenWidth),RandomFloat(0.0f, screenHeight) });
		}
	}
}

void GrassManager::Render() const
{
	for (auto& grass : grasses)
	{
		grass->Render();
	}
}
