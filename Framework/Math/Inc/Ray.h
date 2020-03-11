#ifndef INCLUDED_COOENGINE_MATH_RAY_H
#define INCLUDED_COOENGINE_MATH_RAY_H

#include "Vector3.h"
namespace Coo::Math 
{
	struct Ray
	{
		Vector3 org = Vector3::Zero();
		Vector3 dir = Vector3::ZAxis();
	
		Ray(float x, float y, float z, float dx, float dy, float dz) : org(x, y, z), dir(dx, dy, dz) {}
		Ray(const Vector3& org, const Vector3& dir) : org(org), dir(dir) {}
	};

}// namespace Coo::Math

#endif // !INCLUDED_COOENGINE_MATH_RAY_H
