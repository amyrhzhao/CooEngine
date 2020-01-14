#ifndef INCLUDED_COOENGINE_GRAPHICS_BONE_H
#define INCLUDED_COOENGINE_GRAPHICS_BONE_H

#include "Common.h"

namespace Coo::Graphics 
{

struct Bone 
{
	std::string name;
	int index = -1;

	Bone* parent = nullptr;
	int parentIndex = -1;

	std::vector<Bone*> children;
	std::vector<int> childIndices;

	Math::Matrix4 toParentTransform;
	Math::Matrix4 offsetTransform;
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_BONE_H
