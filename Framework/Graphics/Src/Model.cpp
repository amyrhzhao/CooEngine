#include "Precompiled.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshIO.h"
#include "Model.h"
#include "TextureManager.h"

using namespace Coo::Graphics;

bool Model::Save(std::string fileName)
{
	std::filesystem::path path(fileName);
	path.replace_extension("model");

	FILE* modelFile = nullptr;
	auto sucess = fopen_s(&modelFile, path.string().c_str(), "wb");
	if (sucess != 0 || modelFile == nullptr)
	{
		LOG("Fail to create file");
		return false;
	}

	// saving mesh data
	uint32_t numMeshes = static_cast<uint32_t>(mMeshData.size());
	fwrite(&numMeshes, sizeof(uint32_t), 1, modelFile);
	for (uint32_t i = 0; i < numMeshes; ++i)
	{
		bool success = Coo::Graphics::MeshIO::Save(mMeshData[i].mesh, modelFile);
		if (!success) { LOG("Fail to write mesh %d", static_cast<int>(i)); return false; }
		fwrite(&(mMeshData[i].materialIndex), sizeof(uint32_t), 1, modelFile);
	}

	// saving material data
	uint32_t numMaterial = static_cast<uint32_t>(mMaterialData.size());
	fwrite(&numMaterial, sizeof(uint32_t), 1, modelFile);
	for (uint32_t i = 0; i < numMaterial; ++i)
	{
		auto& materialData = mMaterialData[i];
		uint32_t numTexture = static_cast<uint32_t>(materialData.textureData.size());
		fwrite(&numTexture, sizeof(uint32_t), 1, modelFile);
		for (uint32_t j = 0; j < numTexture; ++j)
		{
			auto& textureData = materialData.textureData[j];
			uint32_t lengthName = static_cast<uint32_t>(textureData.textureName.size());
			fwrite(&lengthName, sizeof(uint32_t), 1, modelFile);
			fwrite(textureData.textureName.data(), lengthName, 1, modelFile);
		}
	}

	fclose(modelFile);
	return true;
}

bool Model::Load(std::string fileName)
{
	FILE* inputFile;
	auto success = fopen_s(&inputFile, fileName.c_str(), "rb");
	if (success != 0)
	{
		LOG("fail to open fail");
		return false;
	}

	// loading mesh data
	uint32_t numMeshes;
	fread_s(&numMeshes, sizeof(uint32_t), sizeof(uint32_t), 1, inputFile);
	mMeshData.resize(numMeshes);
	for (uint32_t i = 0; i < numMeshes; ++i)
	{
		MeshIO::Load(mMeshData[i].mesh, inputFile);
		fread_s(&(mMeshData[i].materialIndex), sizeof(uint32_t), sizeof(uint32_t), 1, inputFile);
	}

	// loading material data
	uint32_t numMaterial;
	fread_s(&numMaterial, sizeof(uint32_t), sizeof(uint32_t), 1, inputFile);
	mMaterialData.resize(numMaterial);
	for (uint32_t i = 0; i < numMaterial; ++i)
	{
		auto& materialData = mMaterialData[i];
		uint32_t numTexture;
		fread_s(&numTexture, sizeof(uint32_t), sizeof(uint32_t), 1, inputFile);
		materialData.textureData.resize(numTexture);
		for (uint32_t j = 0; j < numTexture; ++j)
		{
			auto& textureData = materialData.textureData[j];
			uint32_t lengthName;
			fread_s(&lengthName, sizeof(uint32_t), sizeof(uint32_t), 1, inputFile);
			textureData.textureName.resize(lengthName);
			fread_s(textureData.textureName.data(), lengthName, lengthName, 1, inputFile);
		}
	}

	fclose(inputFile);
	return true;
}

void Model::Initialize()
{
	// initialize mesh buffers
	for (uint32_t i = 0; i < mMeshData.size(); ++i)
	{
		auto& meshData = mMeshData[i];
		meshData.meshBuffer.Initialize(meshData.mesh);
	}

	// initialize textureId
	auto tm = TextureManager::Get();
	for (uint32_t i = 0; i < mMaterialData.size(); ++i)
	{
		for (uint32_t j = 0; j < mMaterialData[i].textureData.size(); ++j)
		{
			auto& texture = mMaterialData[i].textureData[j];
			if (!texture.textureName.empty())
			{
				texture.textureId = tm->LoadTexture(texture.textureName, false);
			}
		}
	}
}

void Model::Terminate()
{
	// terminating mesh buffers
	for (uint32_t i = 0; i < mMeshData.size(); ++i)
	{
		auto& meshData = mMeshData[i];
		meshData.meshBuffer.Terminate();
	}
}

void Model::Render()
{
	for (uint32_t i = 0; i < mMeshData.size(); ++i)
	{
		auto& meshData = mMeshData[i];
		SetMaterial(meshData.materialIndex);
		meshData.meshBuffer.Render();
	}
}

void Model::SetMaterial(uint32_t index)
{
	auto tm = TextureManager::Get();
	auto& materialData = mMaterialData[index];

	// diffuse map
	size_t diffuse = materialData.textureData[static_cast<uint32_t>(TextureType::Diffuse)].textureId;
	if (diffuse != 0) { tm->BindPS(diffuse, 0); }
	
	// specular map
	size_t specular = materialData.textureData[static_cast<uint32_t>(TextureType::Specular)].textureId;
	if (specular != 0) { tm->BindPS(specular, 1); }

	// normal map
	size_t normals = materialData.textureData[static_cast<uint32_t>(TextureType::Normals)].textureId;
	if (normals != 0) { tm->BindPS(normals, 3); }
	
	// displacement map
	size_t displacement = materialData.textureData[static_cast<uint32_t>(TextureType::Displacement)].textureId;
	if (displacement != 0) { tm->BindVS(displacement, 2); }
}