#include "Precompiled.h"
#include "SamplerManager.h"

using namespace Coo::Graphics;

namespace 
{
	std::unique_ptr<SamplerManager> sSamplerManager = nullptr;
}

void SamplerManager::StaticInitialize()
{
	ASSERT(sSamplerManager == nullptr, "[SamplerManager] System already initialized!");
	sSamplerManager = std::make_unique<SamplerManager>();
	sSamplerManager->Initialize();
}

void SamplerManager::StaticTerminate()
{
	if (sSamplerManager != nullptr)
	{
		sSamplerManager->Terminate();
		sSamplerManager.reset();
	}
}

SamplerManager* SamplerManager::Get()
{
	ASSERT(sSamplerManager != nullptr, "[SamplerManager] No Instance registed.");
	return sSamplerManager.get();
}

SamplerManager::~SamplerManager()
{
}

void SamplerManager::Initialize()
{
	AddSampler("PointClamp", Sampler::Filter::Point, Sampler::AddressMode::Clamp);
	AddSampler("PointWrap", Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	AddSampler("LinearClamp", Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	AddSampler("LinearWrap", Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	AddSampler("AnisotropicClamp", Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	AddSampler("AnisotropicWrap", Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	
}

void SamplerManager::Terminate()
{
	for (auto&[key, value] : mInventory) 
	{
		value->Terminate();
	}
}

bool Coo::Graphics::SamplerManager::AddSampler(std::string name, Sampler::Filter filter, Sampler::AddressMode addressMode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if(success)
	{
		auto sampler = std::make_unique<Sampler>();
		sampler->Initialize(filter, addressMode);
		iter->second = std::move(sampler);
	}
	return success;
}

Sampler* SamplerManager::GetSampler(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}
