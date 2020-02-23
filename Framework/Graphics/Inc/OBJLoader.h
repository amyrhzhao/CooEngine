#ifndef INCLUDED_COOENGINE_GRAPHICS_OBJLOADER_H
#define INCLUDED_COOENGINE_GRAPHICS_OBJLOADER_H

#include "Mesh.h"

namespace Coo::Graphics 
{
	class ObjLoader 
	{
	public:
		static void Load(const std::filesystem::path& filePath, float scale, Mesh& mesh);
	};
}

#endif // !INCLUDED_COOENGINE_GRAPHICS_OBJLOADER_H
