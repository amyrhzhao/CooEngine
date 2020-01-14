#include "Precompiled.h"
#include "Mesh.h"
#include "MeshIO.h"

using namespace Coo::Graphics;

namespace
{
	void Serialize(const BoneMesh& mesh, Coo::Core::StreamWriter& sw)
	{
		// vertices
		const std::vector<BoneVertex>& vertices = mesh.vertices;
		size_t numVertex = vertices.size();
		sw.Write(numVertex);
		for (size_t i = 0; i < numVertex; ++i)
		{
			const BoneVertex& vertice = vertices[i];
			// position
			sw.Write(vertice.position.x);
			sw.Write(vertice.position.y);
			sw.Write(vertice.position.z);
			// normal
			sw.Write(vertice.normal.x);
			sw.Write(vertice.normal.y);
			sw.Write(vertice.normal.z);
			// tangent
			sw.Write(vertice.tangent.x);
			sw.Write(vertice.tangent.y);
			sw.Write(vertice.tangent.z);
			// textureCoord
			sw.Write(vertice.texcoord.x);
			sw.Write(vertice.texcoord.y);
			// boneIndices
			sw.Write(vertice.boneIndices[0]);
			sw.Write(vertice.boneIndices[1]);
			sw.Write(vertice.boneIndices[2]);
			sw.Write(vertice.boneIndices[3]);
			// boneWeights
			sw.Write(vertice.boneWeights[0]);
			sw.Write(vertice.boneWeights[1]);
			sw.Write(vertice.boneWeights[2]);
			sw.Write(vertice.boneWeights[3]);
		}

		// indices
		const std::vector<uint32_t>& indices = mesh.indices;
		size_t numIndices = indices.size();
		sw.Write(numIndices);
		for (size_t i = 0; i < numIndices; ++i)
		{
			sw.Write(indices[i]);
		}
	}

	void Deserialize(BoneMesh& mesh, FILE* file)
	{
		// vertices
		{
			std::vector<BoneVertex>& vertices = mesh.vertices;
			size_t numVertex;
			fread_s(&numVertex, sizeof(size_t), sizeof(size_t), 1, file);
			vertices.resize(numVertex);
			uint8_t* verticesData = new uint8_t[sizeof(BoneVertex)*numVertex];
			fread_s(verticesData, sizeof(BoneVertex)*numVertex, sizeof(BoneVertex), numVertex, file);
			Coo::Core::MemoryStream inStream(verticesData, static_cast<uint32_t>(sizeof(BoneVertex) * numVertex));
			Coo::Core::StreamReader sr(inStream);
			for (size_t i = 0; i < numVertex; ++i)
			{
				BoneVertex& vertice = vertices[i];
				// position
				sr.Read(vertice.position.x);
				sr.Read(vertice.position.y);
				sr.Read(vertice.position.z);
				// normal
				sr.Read(vertice.normal.x);
				sr.Read(vertice.normal.y);
				sr.Read(vertice.normal.z);
				// tangent
				sr.Read(vertice.tangent.x);
				sr.Read(vertice.tangent.y);
				sr.Read(vertice.tangent.z);
				// textureCoord
				sr.Read(vertice.texcoord.x);
				sr.Read(vertice.texcoord.y);
				// boneIndices
				sr.Read(vertice.boneIndices[0]);
				sr.Read(vertice.boneIndices[1]);
				sr.Read(vertice.boneIndices[2]);
				sr.Read(vertice.boneIndices[3]);
				// boneWeights
				sr.Read(vertice.boneWeights[0]);
				sr.Read(vertice.boneWeights[1]);
				sr.Read(vertice.boneWeights[2]);
				sr.Read(vertice.boneWeights[3]);
			}
		}

		// indices
		{
			std::vector<uint32_t>& indices = mesh.indices;
			size_t numIndices;
			fread_s(&numIndices, sizeof(size_t), sizeof(size_t), 1, file);
			indices.resize(numIndices);
			uint8_t* indicesData = new uint8_t[sizeof(uint32_t)*numIndices];
			fread_s(indicesData, sizeof(uint32_t)*numIndices, sizeof(uint32_t), numIndices, file);
			Coo::Core::MemoryStream inStream(indicesData, static_cast<uint32_t>(sizeof(uint32_t) * numIndices));
			Coo::Core::StreamReader sr(inStream);
			for (size_t i = 0; i < numIndices; ++i)
			{
				sr.Read(indices[i]);
			}
		}
	}
}

bool Coo::Graphics::MeshIO::Save(const BoneMesh& mesh, FILE* file)
{
	Core::MemoryStream outStream;
	Core::StreamWriter writer(outStream);
	Serialize(mesh, writer);
	uint32_t streamSize = outStream.GetSize();
	size_t wrote = fwrite(outStream.GetData(), streamSize, 1, file);
	return wrote == 1;
}

bool Coo::Graphics::MeshIO::Load(BoneMesh& mesh, FILE* file)
{
	Deserialize(mesh, file);
	return true;
}
