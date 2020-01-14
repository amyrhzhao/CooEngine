#ifndef INCLUDED_COOENGINE_GRAPHICS_MESHBUFFER_H
#define INCLUDED_COOENGINE_GRAPHICS_MESHBUFFER_H

#include "VertexType.h"
#include "Mesh.h"

namespace Coo::Graphics {

class MeshBuffer
{
public:
	enum class Topology {Points, Lines, Triangeles};

	template<class T>
	void Initialize(const T& mesh, bool dynamic = false);

	void Initialize(const void* vertexData, uint32_t vertexSize, uint32_t numVertice, bool dynamic = false);
	void Initialize(const void* vertexData, uint32_t vertexSize, uint32_t numVertice, const uint32_t* indexData, uint32_t numIndices, bool dynamic = false);
	void Terminate();

	void SetTopology(Topology topology);

	void Update(const void* vertexData, uint32_t numVertices);
	void Render() const;

private:
	ID3D11Buffer* mVertexBuffer{ nullptr };
	ID3D11Buffer* mIndexBuffer{ nullptr };

	uint32_t mVertexSize{ 0 };
	uint32_t mVertexCount{ 0 };
	uint32_t mIndexCount{ 0 };

	D3D11_PRIMITIVE_TOPOLOGY mTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	void InitializeVertexBuffer(const void* vertexData, uint32_t vertexSize, uint32_t numVertice, bool dynamic = false);
	void InitializeIndexBuffer(const uint32_t* indexData, uint32_t numIndices);
};

template<class T>
inline void MeshBuffer::Initialize(const T& mesh, bool dynamic)
{
	Initialize(
		mesh.vertices.data(),
		static_cast<uint32_t>(sizeof(T::VertexType)),
		static_cast<uint32_t>(mesh.vertices.size()),
		mesh.indices.data(),
		static_cast<uint32_t>(mesh.indices.size()),
		dynamic
	);
}

} // namespace Coo::Graphics


#endif // !INCLUDED_COOENGINE_GRAPHICS_MESHBUFFER_H
