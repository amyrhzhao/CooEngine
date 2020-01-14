#include "Precompiled.h"
#include "BlendStateManager.h"

using namespace Coo::Graphics;

namespace
{
	std::unique_ptr<BlendStateManager> sBlendStateManager = nullptr;
}

void BlendStateManager::StaticInitialize()
{
	ASSERT(sBlendStateManager == nullptr, "[BlendStateManager] System already initialized!");
	sBlendStateManager = std::make_unique<BlendStateManager>();
	sBlendStateManager->Initialize();
}

void BlendStateManager::StaticTerminate()
{
	if (sBlendStateManager != nullptr)
	{
		sBlendStateManager->Terminate();
		sBlendStateManager.reset();
	}
}

BlendStateManager* BlendStateManager::Get()
{
	ASSERT(sBlendStateManager != nullptr, "[BlendStateManager] No Instance registed.");
	return sBlendStateManager.get();
}

BlendStateManager::~BlendStateManager()
{
}

void BlendStateManager::Initialize()
{
	AddBlendState("Opaque", BlendState::Mode::Opaque);
	AddBlendState("AlphaBlend", BlendState::Mode::AlphaBlend);
	AddBlendState("AlphaPremultiplied", BlendState::Mode::AlphaPremultiplied);
	AddBlendState("Additive", BlendState::Mode::Additive);
}

void BlendStateManager::Terminate()
{
	for (auto&[key, value] : mInventory)
	{
		value->Terminate();
	}
}

bool BlendStateManager::AddBlendState(std::string name, BlendState::Mode mode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto blendState = std::make_unique<BlendState>();
		blendState->Initialize(mode);
		iter->second = std::move(blendState);
	}
	return success;
}

BlendState* BlendStateManager::GetBlendState(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}

