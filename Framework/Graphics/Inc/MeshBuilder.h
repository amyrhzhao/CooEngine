#ifndef INCLUDED_COOENGINE_GRAPHICS_MESHBUILDER_H
#define INCLUDED_COOENGINE_GRAPHICS_MESHBUILDER_H

#include "Mesh.h"

namespace Coo::Graphics {

class MeshBuilder
{
public:
	static MeshPC CreateCubePC(float x = 1.0f, float y = 1.0f, float z = 1.0f);
	static Mesh CreateCube(float x = 1.0f, float y = 1.0f, float z = 1.0f);

	static MeshPC CreateConePC(float radius = 1.0f,float height = 1.0f, uint32_t sides = 16);
	static MeshPX CreateConePX(float radius = 1.0f, float height = 1.0f, uint32_t sides = 16);
	
	static MeshPC CreateCylinderPC(float radius = 1.0f, float height = 1.0f, uint32_t sides = 16);
	static MeshPX CreateCylinderPX(float radius = 1.0f, float height = 1.0f, uint32_t sides = 16);
	
	static MeshPC CreatePlanePC(float width = 1.0f, float height = 1.0f);
	static MeshPX CreatePlanePX(float width = 1.0f, float height = 1.0f, uint32_t columns = 1, uint32_t raws = 1);
	static Mesh CreatePlane(float width = 1.0f, float height = 1.0f, uint32_t columns = 1, uint32_t raws = 1);

	static Mesh CreateSphere(float radius = 1.0f, uint32_t horizontalCount = 32, uint32_t verticalCount = 32);
	static MeshPC CreateSpherePC(float radius = 1.0f, uint32_t horizontalCount = 32, uint32_t verticalCount = 32);
	static MeshPN CreateSpherePN(float radius = 1.0f, uint32_t horizontalCount = 32, uint32_t verticalCount = 32);
	static MeshPX CreateSpherePX(float radius = 1.0f, uint32_t horizontalCount = 32, uint32_t verticalCount = 32);
	
	static MeshPC CreateIcosahedronPC();
	
	static MeshPX CreateNDCQuad();

	static BoneMesh CreateTentacle(uint32_t numBone = 3, float radius = 1.0f, float height = 1.0f ,uint32_t horizontalCount = 32);

	static Math::AABB ComputeBound(const Mesh& mesh);

	//Todo: static MeshPC CreateTorusPC(float ringRad = 1.0f, float tubeRad = 0.2f, uint32_t ringSides = 16, uint32_t tubeSides = 16);
};

} // namespace Coo::Graphics


#endif // !INCLUDED_COOENGINE_GRAPHICS_MESHBUILDER_H
