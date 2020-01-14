#include "Precompiled.h"
#include "TextureManager.h"

using namespace Coo::Graphics;

namespace
{
	std::unique_ptr<TextureManager> sTextureManager = nullptr;
}

void TextureManager::StaticInitialize(std::filesystem::path rootPath)
{
	ASSERT(sTextureManager == nullptr, "[TextureManager] System already initialized!");
	sTextureManager = std::make_unique<TextureManager>();
	sTextureManager->SetRootPath(rootPath);
}

void TextureManager::StaticTerminate()
{
	if (sTextureManager != nullptr)
	{
		sTextureManager->FlushInventory();
		sTextureManager.reset();
	}
}

TextureManager* TextureManager::Get()
{
	ASSERT(sTextureManager != nullptr, "[TextureManager] No Instance registed.");
	return sTextureManager.get();
}

TextureManager::~TextureManager()
{
	ASSERT(mInventory.empty(), "[TextureManager] Clear() must be called to clean up!");
}

void TextureManager::SetRootPath(std::filesystem::path rootPath)
{
	ASSERT(std::filesystem::is_directory(rootPath), "[TextureManager] %ls is not a directory!", rootPath.c_str());
	mRootPath = std::move(rootPath);
}

TextureId TextureManager::LoadTexture(std::filesystem::path fileName, bool useRootPath)
{
	auto hash = std::filesystem::hash_value(fileName);
	auto[iter, success] = mInventory.insert({ hash,nullptr });
	if (success) 
	{
		auto& texturePtr = iter->second;
		texturePtr = std::make_unique<Texture>();
		texturePtr->Initialize(useRootPath ? mRootPath / fileName : fileName);
	}
	return hash;
}

const Texture* TextureManager::GetTexture(TextureId textureId) const
{
	if (auto iter = mInventory.find(textureId);
		iter != mInventory.end())
	{
		return iter->second.get();
	}
	return nullptr;
}

void TextureManager::FlushInventory()
{
	for (auto& item : mInventory)
	{
		if (item.second)
		{
			item.second->Terminate();
			item.second.reset();
		}
	}
	mInventory.clear();
}

void TextureManager::BindPS(TextureId id, uint32_t slot) const
{
	auto texture = GetTexture(id);
	if (texture)
	{
		texture->BindPS(slot);
	}
	return;
}

void TextureManager::BindVS(TextureId id, uint32_t slot) const
{
	auto texture = GetTexture(id);
	if (texture)
	{
		texture->BindVS(slot);
	}
	return;
}