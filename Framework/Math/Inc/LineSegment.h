#ifndef INCLUDED_COOENGINE_MATH_LINESEGMENT_H
#define INCLUDED_COOENGINE_MATH_LINESEGMENT_H

namespace Coo::Math
{
struct LineSegment 
{
	Vector2 from;
	Vector2 to;

	LineSegment() : from(0.0f, 0.0f), to(0.0f, 0.0f) {}
	LineSegment(const Vector2& v0, const Vector2& v1) : from(v0), to(v1) {}
};
}

#endif // !INCLUDED_COOENGINE_MATH_LINESEGMENT_H
