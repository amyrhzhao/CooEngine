#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace Coo;
using namespace Coo::Graphics;
using namespace Coo::Math;

MeshPC MeshBuilder::CreateCubePC(float x, float y, float z)
{
	VertexPC vertices[] =
	{
		{ { -x, +y, -z },{ 1.0f, 0.0f, 0.0f, 1.0f }},	//0	
		{ { +x, +y, -z },{ 1.0f, 0.0f, 0.0f, 1.0f }},	//1
		{ { -x, -y, -z },{ 0.0f, 1.0f, 0.0f, 1.0f }},	//2
		{ { +x, -y, -z },{ 0.0f, 1.0f, 0.0f, 1.0f }},	//3
		{ { -x, +y, +z }, {0.0f, 0.0f, 1.0f, 1.0f }},	//4	
		{ { +x, +y, +z }, {0.0f, 0.0f, 1.0f, 1.0f }},	//5
		{ { -x, -y, +z }, {1.0f, 1.0f, 0.0f, 1.0f }},	//6
		{ { +x, -y, +z }, {1.0f, 1.0f, 0.0f, 1.0f }},	//7
	};

	constexpr uint32_t indices[]
	{
		0,1,2,//|
		1,3,2,//|--- Front face
		1,7,3,//|
		1,5,7,//|--- Right face
		5,4,6,//|
		5,6,7,//|--- Back face
		4,0,2,//|
		4,2,6,//|--- Left face
		4,1,0,//|
		4,5,1,//|--- Top face
		3,7,6,//|
		2,3,6 //|--- Bottom face
	};

	MeshPC mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

Mesh MeshBuilder::CreateCube(float x, float y, float z)
{
	Vertex vertices[] =
	{
		{ { x, -y, -z}, { 1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f,  1.0f}, {0.0f, 1.0f} },
		{ { x,  y, -z}, { 1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f,  1.0f}, {0.0f, 0.0f} },
		{ { x,  y,  z}, { 1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f,  1.0f}, {1.0f, 0.0f} },
		{ { x, -y,  z}, { 1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f,  1.0f}, {1.0f, 1.0f} },
		{ {-x, -y,  z}, {-1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f, -1.0f}, {0.0f, 1.0f} },
		{ {-x,  y,  z}, {-1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} },
		{ {-x,  y, -z}, {-1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} },
		{ {-x, -y, -z}, {-1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} },
		{ {-x,  y, -z}, { 0.0f,  1.0f,  0.0f}, { 1.0f, 0.0f,  0.0f}, {0.0f, 1.0f} },
		{ {-x,  y,  z}, { 0.0f,  1.0f,  0.0f}, { 1.0f, 0.0f,  0.0f}, {0.0f, 0.0f} },
		{ { x,  y,  z}, { 0.0f,  1.0f,  0.0f}, { 1.0f, 0.0f,  0.0f}, {1.0f, 0.0f} },
		{ { x,  y, -z}, { 0.0f,  1.0f,  0.0f}, { 1.0f, 0.0f,  0.0f}, {1.0f, 1.0f} },
		{ {-x, -y,  z}, { 0.0f, -1.0f,  0.0f}, { 1.0f, 0.0f,  0.0f}, {0.0f, 1.0f} },
		{ {-x, -y, -z}, { 0.0f, -1.0f,  0.0f}, { 1.0f, 0.0f,  0.0f}, {0.0f, 0.0f} },
		{ { x, -y, -z}, { 0.0f, -1.0f,  0.0f}, { 1.0f, 0.0f,  0.0f}, {1.0f, 0.0f} },
		{ { x, -y,  z}, { 0.0f, -1.0f,  0.0f}, { 1.0f, 0.0f,  0.0f}, {1.0f, 1.0f} },
		{ { x, -y,  z}, { 0.0f,  0.0f,  1.0f}, {-1.0f, 0.0f,  0.0f}, {0.0f, 1.0f} },
		{ { x,  y,  z}, { 0.0f,  0.0f,  1.0f}, {-1.0f, 0.0f,  0.0f}, {0.0f, 0.0f} },
		{ {-x,  y,  z}, { 0.0f,  0.0f,  1.0f}, {-1.0f, 0.0f,  0.0f}, {1.0f, 0.0f} },
		{ {-x, -y,  z}, { 0.0f,  0.0f,  1.0f}, {-1.0f, 0.0f,  0.0f}, {1.0f, 1.0f} },
		{ {-x, -y, -z}, { 0.0f,  0.0f, -1.0f}, { 1.0f, 0.0f,  0.0f}, {0.0f, 1.0f} },
		{ {-x,  y, -z}, { 0.0f,  0.0f, -1.0f}, { 1.0f, 0.0f,  0.0f}, {0.0f, 0.0f} },
		{ { x,  y, -z}, { 0.0f,  0.0f, -1.0f}, { 1.0f, 0.0f,  0.0f}, {1.0f, 0.0f} },
		{ { x, -y, -z}, { 0.0f,  0.0f, -1.0f}, { 1.0f, 0.0f,  0.0f}, {1.0f, 1.0f} }
	};

	constexpr uint32_t indices[] =
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	Mesh mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPC MeshBuilder::CreateConePC(float radius, float height, uint32_t sides)
{
	std::vector<VertexPC> vertices{ sides + 2 , { Math::Vector3::Zero() ,Graphics::Colors::DeepSkyBlue }, };
	std::size_t indicesCount = static_cast<std::size_t>(sides * 2 * 3);
	std::vector<uint32_t> indices;
	indices.resize(indicesCount);
	float halfHeight = height * 0.5f;

	// center of base
	vertices[0].position = { 0.0f, -halfHeight,0.0f };
	vertices[0].color = Graphics::Colors::DarkBlue;

	// vertices on the base
	for (uint32_t i = 0, n = sides - 1; i < sides; i++)
	{
		float ratio = (float)i / n;
		float r = ratio * (Math::Pi * 2.0f);
		float x = cosf(r) * radius;
		float z = sinf(r) * radius;
		vertices[i + 1].position = { x, -halfHeight, z };
	}

	// apex
	vertices[sides + 1].position = { 0.0f, halfHeight, 0.0f };
	vertices[sides + 1].color = Graphics::Colors::DarkBlue;

	// construct base
	for (int i = 0, n = sides - 1; i < n; i++)
	{
		int offset = i * 3;
		indices[offset] = 0;
		indices[offset + 1] = i + 1;
		indices[offset + 2] = i + 2;
	}

	// construct sides
	int baseOffset = sides * 3;
	for (int i = 0, n = sides - 1; i < n; i++)
	{
		int offset = i * 3 + baseOffset;
		indices[offset] = i + 1;
		indices[offset + 1] = sides + 1;
		indices[offset + 2] = i + 2;
	}

	MeshPC mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPX MeshBuilder::CreateConePX(float radius, float height, uint32_t sides)
{
	std::vector<VertexPX> vertices;
	std::vector<uint32_t> indices;
	float initY = height * 0.5f;

	for (uint32_t i = 0, n = sides - 1; i < sides; i++)
	{
		// Set vertices
		float ratio = (float)i / n;
		float r = ratio * (Math::Pi * 2.0f);
		float x = cosf(r) * radius;
		float z = sinf(r) * radius;
		vertices.push_back({ {0.0, initY, 0.0f},{ratio, 0.0f} });// apex
		vertices.push_back({ {x, -initY, z}, {ratio, 1.0f} });// side of base
		vertices.push_back({ {0.0f, -initY, 0.0f }, {ratio, 0.0f} });// center of base
		// Set indices
		uint32_t offset = i * 3;
		indices.push_back(offset);		//
		indices.push_back(offset + 4);	//
		indices.push_back(offset + 1);	// side
		indices.push_back(offset + 4);	//
		indices.push_back(offset + 2);	//
		indices.push_back(offset + 1);	// base
	}

	MeshPX mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC(float radius, float height, uint32_t sides)
{
	std::vector<VertexPC> vertices{ (sides * 2) + 2 , { Math::Vector3::Zero() ,Graphics::Colors::AliceBlue } };
	std::size_t indicesCount = static_cast<std::size_t>(sides * 4 * 3);
	std::vector<uint32_t> indices;
	indices.resize(indicesCount);
	float halfHeight = height * 0.5f;

	// center of bottom base
	vertices[0].position = { 0.0f, -halfHeight, 0.0f };
	vertices[0].color = Graphics::Colors::DeepSkyBlue;

	// center of top base
	int bottomVertexOffset = sides + 1;
	vertices[bottomVertexOffset].position = { 0.0f, halfHeight, 0.0f };
	vertices[bottomVertexOffset].color = Graphics::Colors::DarkBlue;

	// vertices on bases 
	for (uint32_t i = 0, n = sides - 1; i < sides; i++)
	{
		float ratio = (float)i / n;
		float r = ratio * (Math::Pi * 2.0f);
		float x = cosf(r) * radius;
		float z = sinf(r) * radius;
		// bottom base
		vertices[i + 1].position = { x, -halfHeight, z };
		vertices[i + 1].color = Graphics::Colors::CornflowerBlue;
		// top base
		vertices[i + 1 + bottomVertexOffset].position = { x,halfHeight,z };
		vertices[i + 1 + bottomVertexOffset].color = Graphics::Colors::DodgerBlue;
	}

	// construct bases
	int bottomIndicesOffset = sides * 3;
	for (int i = 0, n = sides - 1; i < n; i++)
	{
		int offset = i * 3;
		// bottom base
		indices[offset] = 0;
		indices[offset + 1] = i + 1;
		indices[offset + 2] = i + 2;
		// top base
		indices[bottomIndicesOffset + offset] = bottomVertexOffset;
		indices[bottomIndicesOffset + offset + 1] = bottomVertexOffset + i + 2;
		indices[bottomIndicesOffset + offset + 2] = bottomVertexOffset + i + 1;
	}

	// construct sides
	int baseOffset = sides * 2 * 3;
	for (int i = 0, n = sides - 1; i < n; i++)
	{
		int offset = i * 6 + baseOffset;
		indices[offset] = i + 1;
		indices[offset + 1] = i + bottomVertexOffset + 2;
		indices[offset + 2] = i + 2;
		indices[offset + 3] = i + 1;
		indices[offset + 4] = i + bottomVertexOffset + 1;
		indices[offset + 5] = i + bottomVertexOffset + 2;
	}

	MeshPC mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPX MeshBuilder::CreateCylinderPX(float radius, float height, uint32_t sides)
{
	std::vector<VertexPX> vertices;
	std::vector<uint32_t> indices;
	float initY = height * 0.5f;

	for (uint32_t i = 0, n = sides - 1; i < sides; i++)
	{
		// Set vertices
		float ratio = (float)i / n;
		float r = ratio * (Math::Pi * 2.0f);
		float x = cosf(r) * radius;
		float z = sinf(r) * radius;
		vertices.push_back({ {0.0f, initY, 0.0f }, {ratio, 1.0f} });	// center of top
		vertices.push_back({ {x, initY, z},{ratio, 0.0f} });			// side of top
		vertices.push_back({ {0.0f, -initY, 0.0f }, {ratio, 0.0f} });	// center of base
		vertices.push_back({ {x, -initY, z}, {ratio, 1.0f} });			// side of base
		// Set indices
		uint32_t offset = i * 4;
		indices.push_back(offset);		//
		indices.push_back(offset + 5);	//
		indices.push_back(offset + 1);	// top
		indices.push_back(offset + 5);	//
		indices.push_back(offset + 7);	//
		indices.push_back(offset + 1);	// right side triangle
		indices.push_back(offset + 1);	//
		indices.push_back(offset + 7);	//
		indices.push_back(offset + 3);	// left side triangle
		indices.push_back(offset + 7);	// 
		indices.push_back(offset + 2);	// 
		indices.push_back(offset + 3);	// base
	}


	MeshPX mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPC MeshBuilder::CreatePlanePC(float width, float height)
{
	std::vector<VertexPC> vertices;
	std::vector<uint32_t> indices;

	float x = width * 0.5f;
	float y = height * 0.5f;

	vertices.push_back({ { +x, +y, 0.0f },Graphics::Colors::Gray });
	vertices.push_back({ { +x, -y, 0.0f },Graphics::Colors::Gray });
	vertices.push_back({ { -x, -y, 0.0f },Graphics::Colors::Gray });
	vertices.push_back({ { -x, +y, 0.0f },Graphics::Colors::Gray });

	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(3);
	indices.emplace_back(3);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(2);
	indices.emplace_back(1);
	indices.emplace_back(0);
	indices.emplace_back(2);
	indices.emplace_back(0);
	indices.emplace_back(3);

	MeshPC mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPX MeshBuilder::CreatePlanePX(float width, float height, uint32_t columns, uint32_t rows)
{
	std::vector<VertexPX> vertices;
	std::vector<uint32_t> indices;

	float widthOffset = columns * width * 0.5f;
	float heightOffset = rows * height * 0.5f;

	for (uint32_t i = 0; i < rows; ++i)
	{
		for (uint32_t j = 0; j < columns; ++j)
		{
			// Set vertices
			float iniX = -(j * width) - widthOffset;
			float iniY = -(i * height) + heightOffset;
			vertices.push_back({ {iniX,iniY, 0.0f},{0.0f,0.0f} });
			vertices.push_back({ {iniX + width,iniY, 0.0f},{1.0f,0.0f} });
			vertices.push_back({ {iniX + width,iniY - height, 0.0f},{1.0f,1.0f} });
			vertices.push_back({ {iniX,iniY - height, 0.0f},{0.0f,1.0f} });
			// Set indices
			int offset = (j * columns + i) * 4;
			indices.push_back(0 + offset);
			indices.push_back(1 + offset);
			indices.push_back(2 + offset);
			indices.push_back(0 + offset);
			indices.push_back(2 + offset);
			indices.push_back(3 + offset);
		}
	}

	MeshPX mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

Mesh MeshBuilder::CreateSphere(float radius, uint32_t hCount, uint32_t vCount)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (uint32_t i = 0, m = vCount; i <= m; ++i)
	{
		float vratio = (float)(i) / (float)vCount;
		Math::Vector3 pos = { 0.0f,radius,0.0f };
		Math::Vector3 normal = Math::Normalize(pos);
		Math::Vector2 texcoord = { vratio,0 };
		vertices.push_back({ pos, normal, {}, texcoord });
	}

	for (uint32_t j = 0, n = hCount - 1; j <= n; ++j)
	{
		float hratio = (float)(j + 1) / (float)hCount;
		float hr = hratio * Math::Pi;
		float hs = sinf(hr);
		float hc = cosf(hr);
		for (uint32_t i = 0, m = vCount; i <= m; ++i)
		{
			float vratio = (float)(i) / (float)vCount;
			float vr = vratio * Math::Pi * 2.0f;
			float vs = sinf(vr);
			float vc = cosf(vr);
			float x = hs * vc * radius;
			float y = hc * radius;
			float z = hs * vs * radius;
			Math::Vector3 pos = { x, y, z };
			Math::Vector3 normal = Math::Normalize(pos);
			Math::Vector2 texcoord = { vratio,hratio };
			Math::Vector3 tangent = Math::Normalize({ -z, 0.0f, x });
			vertices.push_back({ pos, normal, tangent, texcoord });
		}
	}

	for (uint32_t i = 0; i <= vCount; ++i)
	{
		uint32_t const a = i + vCount;
		uint32_t const b = a + 1;

		indices.push_back(i);
		indices.push_back(b);
		indices.push_back(a);

	}

	for (uint32_t j = 0; j < hCount - 2; ++j)
	{
		uint32_t aStart = (j + 1)* (vCount + 1);
		uint32_t bStart = (j + 2) * (vCount + 1);
		for (uint32_t i = 0; i < vCount; ++i)
		{
			const uint32_t a = aStart + i;
			const uint32_t a1 = aStart + (i + 1);
			const uint32_t b = bStart + i;
			const uint32_t b1 = bStart + (i + 1);

			indices.push_back(a);
			indices.push_back(a1);
			indices.push_back(b1);
			indices.push_back(a);
			indices.push_back(b1);
			indices.push_back(b);

		}
	}
	for (uint32_t i = 0; i <= vCount; ++i)
	{
		uint32_t const a = static_cast<uint32_t>(vertices.size() - 2 * vCount + i - 2);
		uint32_t const b = static_cast<uint32_t>(vertices.size() - 2 * vCount + i - 1);
		uint32_t const c = static_cast<uint32_t>(vertices.size() - vCount + i - 1);

		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
	}

	Mesh mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPC MeshBuilder::CreateSpherePC(float radius, uint32_t hCount, uint32_t vCount)
{
	std::vector<VertexPC> vertices;
	std::vector<uint32_t> indices;

	vertices.push_back({ {0.0f,radius,0.0f},{Graphics::Colors::BlanchedAlmond} });
	for (uint32_t j = 0; j < hCount - 1; ++j)
	{
		float const polar = Math::Pi * float(j + 1) / float(hCount);
		float const sp = sinf(polar);
		float const cp = cosf(polar);
		for (uint32_t i = 0; i < vCount; ++i)
		{
			float const azimuth = 2.0f * Math::Pi * float(i) / float(vCount);
			float const sa = sinf(azimuth);
			float const ca = cosf(azimuth);
			float const x = sp * ca * radius;
			float const y = cp * radius;
			float const z = sp * sa * radius;
			vertices.push_back({ Math::Vector3{x,y,z},Graphics::Colors::PaleVioletRed });
		}
	}
	vertices.push_back({ {0.0f,-radius,0.0f},{Graphics::Colors::BlanchedAlmond} });

	// top
	for (uint32_t i = 0; i < vCount; ++i)
	{
		uint32_t const a = i + 1;
		uint32_t const b = (i + 1) % vCount + 1;
		indices.push_back(0);
		indices.push_back(b);
		indices.push_back(a);
	}

	// sides
	for (uint32_t j = 0; j < hCount - 2; ++j)
	{
		uint32_t aStart = j * vCount + 1;
		uint32_t bStart = (j + 1) * vCount + 1;
		for (uint32_t i = 0; i < vCount; ++i)
		{
			const uint32_t a = aStart + i;
			const uint32_t a1 = aStart + (i + 1) % vCount;
			const uint32_t b = bStart + i;
			const uint32_t b1 = bStart + (i + 1) % vCount;
			indices.push_back(a);
			indices.push_back(a1);
			indices.push_back(b1);
			indices.push_back(a);
			indices.push_back(b1);
			indices.push_back(b);
		}
	}

	// bottom
	for (uint32_t i = 0; i < vCount; ++i)
	{
		uint32_t const a = i + vCount * (hCount - 2) + 1;
		uint32_t const b = (i + 1) % vCount + vCount * (hCount - 2) + 1;
		indices.push_back(static_cast<uint32_t>(vertices.size() - 1));
		indices.push_back(a);
		indices.push_back(b);
	}

	MeshPC mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPN MeshBuilder::CreateSpherePN(float radius, uint32_t hCount, uint32_t vCount)
{
	std::vector<VertexPN> vertices;
	std::vector<uint32_t> indices;

	for (uint32_t i = 0, m = vCount; i <= m; ++i)
	{
		Math::Vector3 pos = { 0.0f,radius,0.0f };
		Math::Vector3 normal = Math::Normalize(pos);
		vertices.push_back({ pos, normal });
	}

	for (uint32_t j = 0, n = hCount - 1; j <= n; ++j)
	{
		float hratio = (float)(j + 1) / (float)hCount;
		float hr = hratio * Math::Pi;
		float hs = sinf(hr);
		float hc = cosf(hr);
		for (uint32_t i = 0, m = vCount; i <= m; ++i)
		{
			float vratio = (float)(i) / (float)vCount;
			float vr = vratio * Math::Pi * 2.0f;
			float vs = sinf(vr);
			float vc = cosf(vr);
			float x = hs * vc * radius;
			float y = hc * radius;
			float z = hs * vs * radius;
			Math::Vector3 pos = { x, y, z };
			Math::Vector3 normal = Math::Normalize(pos);
			vertices.push_back({ pos, normal });
		}
	}

	for (uint32_t i = 0; i <= vCount; ++i)
	{
		uint32_t const a = i + vCount;
		uint32_t const b = a + 1;

		indices.push_back(i);
		indices.push_back(b);
		indices.push_back(a);

	}

	for (uint32_t j = 0; j < hCount - 2; ++j)
	{
		uint32_t aStart = (j + 1)* (vCount + 1);
		uint32_t bStart = (j + 2) * (vCount + 1);
		for (uint32_t i = 0; i < vCount; ++i)
		{
			const uint32_t a = aStart + i;
			const uint32_t a1 = aStart + (i + 1);
			const uint32_t b = bStart + i;
			const uint32_t b1 = bStart + (i + 1);

			indices.push_back(a);
			indices.push_back(a1);
			indices.push_back(b1);
			indices.push_back(a);
			indices.push_back(b1);
			indices.push_back(b);

		}
	}
	for (uint32_t i = 0; i <= vCount; ++i)
	{
		uint32_t const a = static_cast<uint32_t>(vertices.size() - 2 * vCount + i - 2);
		uint32_t const b = static_cast<uint32_t>(vertices.size() - 2 * vCount + i - 1);
		uint32_t const c = static_cast<uint32_t>(vertices.size() - vCount + i - 1);

		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
	}

	MeshPN mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPX MeshBuilder::CreateSpherePX(float radius, uint32_t hCount, uint32_t vCount)
{
	std::vector<VertexPX> vertices;
	std::vector<uint32_t> indices;

	for (uint32_t i = 0, m = vCount; i <= m; ++i)
	{
		float vratio = (float)(i) / (float)vCount;
		vertices.push_back({ {0.0f,radius,0.0f},{vratio,0} });
	}

	for (uint32_t j = 0, n = hCount - 1; j <= n; ++j)
	{
		float hratio = (float)(j + 1) / (float)hCount;
		float hr = hratio * Math::Pi;
		float hs = sinf(hr);
		float hc = cosf(hr);
		for (uint32_t i = 0, m = vCount; i <= m; ++i)
		{
			float vratio = (float)(i) / (float)vCount;
			float vr = vratio * Math::Pi * 2.0f;
			float vs = sinf(vr);
			float vc = cosf(vr);
			float x = hs * vc * radius;
			float y = hc * radius;
			float z = hs * vs * radius;
			vertices.push_back({ {x, y, z},{vratio,hratio} });
		}
	}

	for (uint32_t i = 0; i <= vCount; ++i)
	{
		uint32_t const a = i + vCount;
		uint32_t const b = a + 1;

		indices.push_back(i);
		indices.push_back(b);
		indices.push_back(a);

	}

	for (uint32_t j = 0; j < hCount - 2; ++j)
	{
		uint32_t aStart = (j + 1)* (vCount + 1);
		uint32_t bStart = (j + 2) * (vCount + 1);
		for (uint32_t i = 0; i < vCount; ++i)
		{
			const uint32_t a = aStart + i;
			const uint32_t a1 = aStart + (i + 1);
			const uint32_t b = bStart + i;
			const uint32_t b1 = bStart + (i + 1);

			indices.push_back(a);
			indices.push_back(a1);
			indices.push_back(b1);
			indices.push_back(a);
			indices.push_back(b1);
			indices.push_back(b);

		}
	}
	for (uint32_t i = 0; i <= vCount; ++i)
	{
		uint32_t const a = static_cast<uint32_t>(vertices.size() - 2 * vCount + i - 2);
		uint32_t const b = static_cast<uint32_t>(vertices.size() - 2 * vCount + i - 1);
		uint32_t const c = static_cast<uint32_t>(vertices.size() - vCount + i - 1);

		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
	}

	MeshPX mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPC MeshBuilder::CreateIcosahedronPC()
{
	std::vector<VertexPC> vertices;
	std::vector<uint32_t> indices;

	const float t = (1.0f + sqrt(5.0f)) / 2.0f;
	vertices.push_back({ Math::Normalize(Math::Vector3{ -1.0f,t,0.0f }), Graphics::Colors::Orange });
	vertices.push_back({ Math::Normalize(Math::Vector3{ 1.0f, t, 0.0 }), Graphics::Colors::Red });
	vertices.push_back({ Math::Normalize(Math::Vector3{ -1.0f, -t, 0.0 }), Graphics::Colors::PaleVioletRed });
	vertices.push_back({ Math::Normalize(Math::Vector3{ 1.0f, -t, 0.0 }), Graphics::Colors::Coral });
	vertices.push_back({ Math::Normalize(Math::Vector3{ 0.0, -1.0f, t }), Graphics::Colors::PapayaWhip });
	vertices.push_back({ Math::Normalize(Math::Vector3{ 0.0, 1.0f, t }), Graphics::Colors::OldLace });
	vertices.push_back({ Math::Normalize(Math::Vector3{ 0.0, -1.0f, -t }), Graphics::Colors::Violet });
	vertices.push_back({ Math::Normalize(Math::Vector3{ 0.0, 1.0f, -t }), Graphics::Colors::MediumVioletRed });
	vertices.push_back({ Math::Normalize(Math::Vector3{ t, 0.0, -1.0f }), Graphics::Colors::Firebrick });
	vertices.push_back({ Math::Normalize(Math::Vector3{ t, 0.0, 1.0f }), Graphics::Colors::OrangeRed });
	vertices.push_back({ Math::Normalize(Math::Vector3{ -t, 0.0, -1.0f }), Graphics::Colors::LemonChiffon });
	vertices.push_back({ Math::Normalize(Math::Vector3{ -t, 0.0, 1.0f }), Graphics::Colors::Wheat });

	indices.emplace_back(0);
	indices.emplace_back(11);
	indices.emplace_back(5);
	indices.emplace_back(0);
	indices.emplace_back(5);
	indices.emplace_back(1);
	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(7);
	indices.emplace_back(0);
	indices.emplace_back(7);
	indices.emplace_back(10);
	indices.emplace_back(0);
	indices.emplace_back(10);
	indices.emplace_back(11);
	indices.emplace_back(1);
	indices.emplace_back(5);
	indices.emplace_back(9);
	indices.emplace_back(5);
	indices.emplace_back(11);
	indices.emplace_back(4);
	indices.emplace_back(11);
	indices.emplace_back(10);
	indices.emplace_back(2);
	indices.emplace_back(10);
	indices.emplace_back(7);
	indices.emplace_back(6);
	indices.emplace_back(7);
	indices.emplace_back(1);
	indices.emplace_back(8);
	indices.emplace_back(3);
	indices.emplace_back(9);
	indices.emplace_back(4);
	indices.emplace_back(3);
	indices.emplace_back(4);
	indices.emplace_back(2);
	indices.emplace_back(3);
	indices.emplace_back(2);
	indices.emplace_back(6);
	indices.emplace_back(3);
	indices.emplace_back(6);
	indices.emplace_back(8);
	indices.emplace_back(3);
	indices.emplace_back(8);
	indices.emplace_back(9);
	indices.emplace_back(4);
	indices.emplace_back(9);
	indices.emplace_back(5);
	indices.emplace_back(2);
	indices.emplace_back(4);
	indices.emplace_back(11);
	indices.emplace_back(6);
	indices.emplace_back(2);
	indices.emplace_back(10);
	indices.emplace_back(8);
	indices.emplace_back(6);
	indices.emplace_back(7);
	indices.emplace_back(9);
	indices.emplace_back(8);
	indices.emplace_back(1);

	MeshPC mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPX MeshBuilder::CreateNDCQuad()
{
	std::vector<VertexPX> vertices;
	std::vector<uint32_t> indices;

	vertices.emplace_back(VertexPX{ {-1.0f, -1.0f, 0.0f }, {0.0f, 1.0f} });
	vertices.emplace_back(VertexPX{ {-1.0f, +1.0f, 0.0f }, {0.0f, 0.0f} });
	vertices.emplace_back(VertexPX{ {+1.0f, +1.0f, 0.0f }, {1.0f, 0.0f} });
	vertices.emplace_back(VertexPX{ {+1.0f, -1.0f, 0.0f }, {1.0f, 1.0f} });

	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);

	indices.emplace_back(0);
	indices.emplace_back(2);
	indices.emplace_back(3);

	MeshPX mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

BoneMesh MeshBuilder::CreateTentacle(uint32_t numBone, float radius, float height, uint32_t sides)
{
	std::vector<BoneVertex> vertices;
	std::vector<uint32_t> indices;

	uint32_t slices = static_cast<uint32_t>(numBone * 5 + 1);
	float unitHeight = height / (slices - 1);
	float unitAngleRad = -Math::TwoPi / (sides - 1);
	float uUnit = 1.0f / (sides - 1);
	float vUnit = 1.0f / (slices - 1);
	float unitBoneHeight = height / numBone;

	for (uint32_t i = 0; i < slices; ++i)
	{
		for (uint32_t j = 0; j < sides; ++j)
		{
			float currentHeight = static_cast<float>(i) * unitHeight;

			Vector3 position{
				radius * sinf(j * unitAngleRad),
				currentHeight,
				radius * cosf(j * unitAngleRad) };
			Vector3 normal = Normalize({ position.x, 0.0f, position.z });
			Vector3 tangent{ -normal.z, normal.y,normal.x };
			Vector2 texcoord{ 1.0f - static_cast<float>(j) * uUnit, 1.0f - static_cast<float>(i) * vUnit };
			int boneIndices[4] = { 0 };
			float boneWeights[4] = { 0.0f };
			uint32_t currentAttachedBone = static_cast<uint32_t>(position.y / unitBoneHeight);

			if (currentAttachedBone != numBone)
			{
				float differenceToBoneCenter = position.y - (currentAttachedBone * unitBoneHeight + unitBoneHeight * 0.5f);
				boneIndices[0] = currentAttachedBone;

				if (differenceToBoneCenter < 0.0f)
				{
					boneIndices[1] = (currentAttachedBone > 0) ? currentAttachedBone - 1 : 0;
					boneWeights[0] = (currentAttachedBone > 0) ? Lerp(1.0f, 0.5f, (-differenceToBoneCenter) / (unitBoneHeight * 0.5f)) : 1.0f;
					boneWeights[1] = (currentAttachedBone > 0) ? Lerp(0.0f, 0.5f, (-differenceToBoneCenter) / (unitBoneHeight * 0.5f)) : 0.0f;
				}
				else
				{
					boneIndices[1] = (currentAttachedBone + 1 < numBone) ? currentAttachedBone + 1 : currentAttachedBone;
					boneWeights[0] = (currentAttachedBone + 1 < numBone) ? Lerp(1.0f, 0.5f, (differenceToBoneCenter) / (unitBoneHeight * 0.5f)) : 1.0f;
					boneWeights[1] = (currentAttachedBone + 1 < numBone) ? Lerp(0.0f, 0.5f, (differenceToBoneCenter) / (unitBoneHeight * 0.5f)) : 0.0f;
				}
			}
			else
			{
				boneIndices[0] = currentAttachedBone - 1;
				boneWeights[0] = 1.0f;
			}

			vertices.push_back({
				position,
				normal,
				tangent,
				texcoord,
				{boneIndices[0], boneIndices[1], boneIndices[2], boneIndices[3]},
				{boneWeights[0], boneWeights[1], boneWeights[2], boneWeights[3]} 
				});

			if (i == 0 && j != 0 && j != sides - 1)
			{
				indices.push_back(static_cast<uint32_t>(j));
				indices.push_back(static_cast<uint32_t>(j + 1));
				indices.push_back(static_cast<uint32_t>(0));
			}
			if ((i != slices - 1) && (j != sides - 1))
			{
				indices.push_back(static_cast<uint32_t>((i * sides + j)));				//|
				indices.push_back(static_cast<uint32_t>(((i + 1) * sides + j)));			//|
				indices.push_back(static_cast<uint32_t>(((i + 1) * sides + j + 1)));		//|--- left triangle

				indices.push_back(static_cast<uint32_t>((i * sides + j)));				//|
				indices.push_back(static_cast<uint32_t>(((i + 1) * sides + j + 1)));		//|
				indices.push_back(static_cast<uint32_t>((i * sides + j + 1)));			//|--- right triangle
			}
			if ((i == slices - 1) && (j != 0) && (j != sides - 1))
			{
				indices.push_back(static_cast<uint32_t>(i * sides + j + 1));				//|
				indices.push_back(static_cast<uint32_t>(i * sides + j));					//|--- top circle
				indices.push_back(static_cast<uint32_t>(i * sides));						//|
			}
		}
	}

	BoneMesh mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}




