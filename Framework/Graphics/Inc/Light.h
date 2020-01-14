#ifndef INCLUDED_COOENGINE_GRAPHICS_LIGHT_H
#define INCLUDED_COOENGINE_GRAPHICS_LIGHT_H

#include "Colors.h"

namespace Coo::Graphics {

struct PointLight 
{
	Math::Vector3 position;
	float padding;
	Color ambient;
	Color diffuse;
	Color specular;
};

struct DirectionalLight
{
	Math::Vector3 direction;
	float padding;
	Color ambient;
	Color diffuse;
	Color specular;
};

}


#endif // !INCLUDED_COOENGINE_GRAPHICS_LIGHT_H
