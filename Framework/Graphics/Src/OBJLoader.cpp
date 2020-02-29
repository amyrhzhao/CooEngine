#include "Precompiled.h"
#include "OBJLoader.h"

using namespace Coo;
using namespace Coo::Graphics;
using namespace Coo::Math;

namespace
{
	// https://www.marti.works/calculating-tangents-for-your-mesh/

	void CalculateTangent(Vertex& v0, const Vector3& bitangent)
	{
		Vector3 t = v0.tangent - (v0.normal * Dot(v0.normal, v0.tangent));
		t = Normalize(t);

		Vector3 c = Cross(v0.normal, v0.tangent);
		float w = (Dot(c, bitangent) < 0.0f) ? -1.0f : 1.0f;
		v0.tangent = t;
	}

	void CalculateTangent(Vertex& v0, Vertex& v1, Vertex& v2)
	{
		Vector3 edge1 = v1.position - v0.position;
		Vector3 edge2 = v2.position - v0.position;
		Vector2 uv1 = v1.texcoord - v0.texcoord;
		Vector2 uv2 = v2.texcoord - v0.texcoord;
		float r = 1.0f / (uv1.x * uv2.y - uv1.y * uv2.x);
		Vector3 tangent = 
		{
			((edge1.x * uv2.y) - (edge2.x * uv1.y)) * r,
			((edge1.y * uv2.y) - (edge2.y * uv1.y)) * r,
			((edge1.z * uv2.y) - (edge2.z * uv1.y)) * r
		};
		Vector3 bitangent(
			((edge1.x * uv2.x) - (edge2.x * uv1.x)) * r,
			((edge1.y * uv2.x) - (edge2.y * uv1.x)) * r,
			((edge1.z * uv2.x) - (edge2.z * uv1.x)) * r
		);
		v0.tangent += tangent;
		v1.tangent += tangent;
		v2.tangent += tangent;
		CalculateTangent(v0, bitangent);
		CalculateTangent(v1, bitangent);
		CalculateTangent(v2, bitangent);
		return;
	}
	

}

// Only support some Obj Files
void Coo::Graphics::ObjLoader::Load(const std::filesystem::path& filePath, float scale, Mesh& mesh)
{
	std::vector<uint32_t> positionIndices, normalIndices, uvIndices;
	std::vector<Math::Vector3> positions;
	std::vector<Math::Vector3> normals;
	std::vector<Math::Vector2> uvs;

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	ASSERT(file, "[ObjLoader] Failed to open file %s.", filePath.u8string().c_str());

	while (true)
	{
		char buffer[128];
		int res = fscanf_s(file, "%s", buffer, static_cast<unsigned int>(std::size(buffer)));
		if (res == EOF) { break; }

		if (strcmp(buffer, "v") == 0)
		{
			Math::Vector3 position;
			fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			positions.push_back(position * scale);
		}
		else if (strcmp(buffer, "vt") == 0)
		{
			Math::Vector2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back({uv.x, 1.0f - uv.y});
		}
		else if (strcmp(buffer, "vn") == 0)
		{
			Math::Vector3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(buffer, "f") == 0)
		{
			uint32_t v[4], t[4], n[4];
			fgets(buffer, static_cast<int>(std::size(buffer)), file);
			if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2],
				&v[3], &t[3], &n[3]) == 12)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);

				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[2]);
				positionIndices.push_back(v[3]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[2]);
				uvIndices.push_back(t[3]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[2]);
				normalIndices.push_back(n[3]);
			}
			else if(sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2]) == 9)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);
			}
			else
			{
				ASSERT(false, "[ObjLoader] Unsupported OBJ format. File: %s", filePath.u8string().c_str());
			}
		}
	}
	fclose(file);

	// Reconstruct vertices
	std::vector<Vertex> vertices;
	vertices.reserve(positionIndices.size());
	for (size_t i = 0; i < positionIndices.size(); ++i) 
	{
		Vertex vertex;
		vertex.position = positions[positionIndices[i] - 1];
		vertex.normal = normals[normalIndices[i] - 1];
		vertex.texcoord = uvs[uvIndices[i] - 1];
		vertices.emplace_back(vertex);
	}

	for (size_t i = 0; i < vertices.size() / 3; ++i) 
	{
		CalculateTangent(vertices[i], vertices[i + 1], vertices[i + 2]);
	}

	std::vector<uint32_t> indices;
	indices.resize(vertices.size());
	std::iota(indices.begin(), indices.end(), 0);

	// Initialize mesh
	mesh.vertices = vertices;
	mesh.indices = indices;
}
