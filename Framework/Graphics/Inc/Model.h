#ifndef INCLUDED_COOENGINE_GRAPHICS_MODEL_H
#define INCLUDED_COOENGINE_GRAPHICS_MODEL_H

#include "TextureManager.h"
namespace Coo::Graphics 
{

class Model 
{
public:
	enum class TextureType
	{
		Diffuse = 0,
		Specular,
		Ambient,
		Emissive,
		Height,
		Normals,
		Shininess,
		Opacity,
		Displacement,
		Lightmap,
		Reflection,
		BaseColor,
		NormalCamera,
		EmissionColor,
		Metalness,
		DiffuseRoughness,
		AmbientOcclusion,
		Unknown
	};

	bool Save(std::string fileName);
	bool Load(std::string fileName);

	void Initialize();
	void Terminate();

	void Render();

public:
	struct MeshData
	{
		BoneMesh mesh;
		uint32_t materialIndex = 0;
		MeshBuffer meshBuffer;
	};

	struct TextureData 
	{
		std::string textureName;
		TextureId textureId = 0;
	};

	struct MaterialData
	{
		std::vector<TextureData> textureData;
	};

	std::vector<MeshData> mMeshData;
	std::vector<MaterialData> mMaterialData;

private:
	void SetMaterial(uint32_t index);
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_MODEL_H
