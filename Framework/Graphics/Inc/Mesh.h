#ifndef INCLUDED_COOENGINE_GRAPHICS_MESH_H
#define INCLUDED_COOENGINE_GRAPHICS_MESH_H

#include "VertexType.h"

namespace Coo::Graphics {

template <class T>
struct MeshBase 
{
	using VertexType = T;
	std::vector<VertexType> vertices;
	std::vector<uint32_t> indices;
};

using MeshPC = MeshBase<VertexPC>;
using MeshPX = MeshBase<VertexPX>;
using MeshPNX = MeshBase<VertexPNX>;
using MeshPN = MeshBase<VertexPN>;
using Mesh = MeshBase<Vertex>;
using BoneMesh = MeshBase<BoneVertex>;

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_MESH_H
