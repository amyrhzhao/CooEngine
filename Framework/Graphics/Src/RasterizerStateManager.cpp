#include "Precompiled.h"
#include "RasterizerStateManager.h"

using namespace Coo::Graphics;
namespace
{
	std::unique_ptr<RasterizerStateManager> sRasterizerStateManager = nullptr;
}


void Coo::Graphics::RasterizerStateManager::StaticInitialize()
{
	ASSERT(sRasterizerStateManager == nullptr, "[RasterizerStateManager] System already initialized!");
	sRasterizerStateManager = std::make_unique<RasterizerStateManager>();
	sRasterizerStateManager->Initialize();
}

void Coo::Graphics::RasterizerStateManager::StaticTerminate()
{
	if (sRasterizerStateManager != nullptr)
	{
		sRasterizerStateManager->Terminate();
		sRasterizerStateManager.reset();
	}
}

RasterizerStateManager* Coo::Graphics::RasterizerStateManager::Get()
{
	ASSERT(sRasterizerStateManager != nullptr, "[RasterizerStateManager] No Instance registed.");
	return sRasterizerStateManager.get();
}

Coo::Graphics::RasterizerStateManager::~RasterizerStateManager()
{
}

void Coo::Graphics::RasterizerStateManager::Initialize()
{
	AddRasterizerState("Wireframe", RasterizerState::CullMode::Back, RasterizerState::FillMode::Wireframe);
	AddRasterizerState("CullFrontSolid", RasterizerState::CullMode::Front, RasterizerState::FillMode::Solid);
	AddRasterizerState("CullFrontWireframe", RasterizerState::CullMode::Front, RasterizerState::FillMode::Wireframe);
	AddRasterizerState("CullNoneSolid", RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	AddRasterizerState("CullNoneWireframe", RasterizerState::CullMode::None, RasterizerState::FillMode::Wireframe);
}

void Coo::Graphics::RasterizerStateManager::Terminate()
{
	for (auto&[key, value] : mInventory)
	{
		value->Terminate();
	}
}

bool Coo::Graphics::RasterizerStateManager::AddRasterizerState(std::string name, RasterizerState::CullMode cullMode, RasterizerState::FillMode fillMode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto blendState = std::make_unique<RasterizerState>();
		blendState->Initialize(cullMode, fillMode);
		iter->second = std::move(blendState);
	}
	return success;
}

RasterizerState* Coo::Graphics::RasterizerStateManager::GetRasterizerState(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}
