#ifndef INCLUDED_COOENGINE_MATH_CIRCLE_H
#define INCLUDED_COOENGINE_MATH_CIRCLE_H

#include "Vector2.h"

namespace Coo::Math 
{
struct Circle
{
	Vector2 center = { 0.0f,0.0f };
	float radius = 1.0f;

	Circle() = default;
	Circle(const Vector2& center, float radius): center(center), radius(radius) {}
};
} // namespace Coo::Math

#endif // !INCLUDED_COOENGINE_MATH_CIRCLE_H
