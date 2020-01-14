#ifndef INCLUDED_COOENGINE_GRAPHICS_MESHIO_H
#define INCLUDED_COOENGINE_GRAPHICS_MESHIO_H

namespace Coo::Graphics {

class MeshIO 
{
public:
	static bool Save(const BoneMesh& mesh, FILE* file);
	static bool Load(BoneMesh& mesh, FILE* file);
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_MESHIO_H
