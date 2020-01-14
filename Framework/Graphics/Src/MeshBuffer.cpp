#include "Precompiled.h"
#include "MeshBuffer.h"
#include "D3DUtil.h"
using namespace Coo::Graphics;



void MeshBuffer::Initialize(const void * vertexData, uint32_t vertexSize, uint32_t numVertice, const uint32_t * indexData, uint32_t numIndices, bool dynamic)
{
	InitializeVertexBuffer(vertexData, vertexSize, numVertice, dynamic);
	InitializeIndexBuffer(indexData, numIndices);
}

void MeshBuffer::Initialize(const void * vertexData, uint32_t vertexSize, uint32_t numVertice, bool dynamic)
{
	InitializeVertexBuffer(vertexData, vertexSize, numVertice, dynamic);
}

void MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
	mVertexSize = 0;
	mVertexCount = 0;
	mIndexCount = 0;
}

void Coo::Graphics::MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case Coo::Graphics::MeshBuffer::Topology::Points:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case Coo::Graphics::MeshBuffer::Topology::Lines:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case Coo::Graphics::MeshBuffer::Topology::Triangeles:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	default:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	}
}

void Coo::Graphics::MeshBuffer::Update(const void * vertexData, uint32_t numVertices)
{
	mVertexCount = numVertices;

	auto context = GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertexData, numVertices * mVertexSize);
	context->Unmap(mVertexBuffer, 0);
}

void MeshBuffer::Render() const
{
	auto context = GetContext();

	// Set vertex buffer
	UINT stride = mVertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	// Set primitive topology
	context->IASetPrimitiveTopology(mTopology);

	// Check if we are using index buffer
	if (mIndexBuffer != nullptr)
	{
		// Set index buffer
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Draw indexed mesh
		context->DrawIndexed(mIndexCount, 0, 0);
	}
	else
	{
		// Draw mesh
		context->Draw(mVertexCount, 0);
		//context->Draw(vertexCount, startVertexLocation);
	}
}

void MeshBuffer::InitializeVertexBuffer(const void * vertexData, uint32_t vertexSize, uint32_t numVertice, bool dynamic)
{
	mVertexCount = numVertice;
	mVertexSize = vertexSize;

	auto device = GetDevice();
	// Allocate memory in VRAM for vertex buffer and initialize
	// with vertex data
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = vertexSize * numVertice;
	vertexBufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertexData;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, vertexData ? &initData : nullptr, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");
}

void MeshBuffer::InitializeIndexBuffer(const uint32_t * indexData, uint32_t numIndices)
{
	mIndexCount = numIndices;

	auto device = GetDevice();
	// Allocate memory for index buffer
	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.ByteWidth = numIndices * sizeof(uint32_t);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = indexData;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&indexBufferDesc, &initData, &mIndexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create index buffer.");

}
