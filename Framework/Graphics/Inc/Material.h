#ifndef INCLUDED_COOENGINE_GRAPHICS_MATERIAL_H
#define INCLUDED_COOENGINE_GRAPHICS_MATERIAL_H

#include "Colors.h"

namespace Coo::Graphics {

struct Material
{
	Color ambient;
	Color diffuse;
	Color specular;
	float power;
	float padding[3];
};

}

#endif // !INCLUDED_COOENGINE_GRAPHICS_MATERIAL_H
