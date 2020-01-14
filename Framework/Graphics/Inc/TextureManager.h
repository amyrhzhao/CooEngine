#ifndef INCLUDED_COOENGINE_GRAPHICS_TEXTUREMANAGER_H
#define INCLUDED_COOENGINE_GRAPHICS_TEXTUREMANAGER_H

#include "Texture.h"

namespace Coo::Graphics
{
using TextureId = size_t;

class TextureManager 
{
public:
	static void StaticInitialize(std::filesystem::path rootPath);
	static void StaticTerminate();
	static TextureManager* Get();

	TextureManager() = default;
	~TextureManager();

	void SetRootPath(std::filesystem::path rootPath);

	TextureId LoadTexture(std::filesystem::path fileName, bool useRootPath = true);
	//TODO: void LoadArchive(std::wstring fileName);
	const Texture* GetTexture(TextureId textureId) const;
	void FlushInventory();

	void BindPS(TextureId id, uint32_t slot = 0)const;
	void BindVS(TextureId id, uint32_t slot = 0)const;

private:
	std::filesystem::path mRootPath;
	std::unordered_map<TextureId, std::unique_ptr<Texture>> mInventory;
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_TEXTUREMANAGER_H
