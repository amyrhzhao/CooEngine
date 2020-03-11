#ifndef INCLUDED_COOENGINE_MATH_AABB_H
#define INCLUDED_COOENGINE_MATH_AABB_H

#include "Vector3.h"
namespace Coo::Math 
{
	struct AABB
	{
		Vector3 center;
		Vector3 extend;

		constexpr AABB() : center(0.0f, 0.0f, 0.0f), extend(0.0f, 0.0f, 0.0f) {}
		constexpr AABB(const Vector3& center, const Vector3& extend) : center(center), extend(extend) {}
		
		constexpr static AABB FromMinMax(const Vector3& min, const Vector3& max) 
		{
			return AABB{ (min + max) * 0.5f, (max - min) * 0.5f };
		}
	};
}

#endif // !INCLUDED_COOENGINE_MATH_AABB_H