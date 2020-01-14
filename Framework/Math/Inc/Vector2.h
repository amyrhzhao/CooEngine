#ifndef INCLUDED_COOENGINE_MATH_VECTOR2_H
#define INCLUDED_COOENGINE_MATH_VECTOR2_H

namespace Coo::Math {

struct Vector2
{
	union 
	{
		struct { float x, y; };
		std::array<float, 2> v;
	};

	constexpr Vector2() noexcept : Vector2{ 0.0f } {}
	constexpr explicit Vector2(float f) noexcept : x{ f }, y{ f } {}
	constexpr Vector2(float x, float y) noexcept : x{ x }, y{ y } {}

	constexpr Vector2 operator-() const						{ return { -x, -y }; }
	constexpr Vector2 operator+(const Vector2& v) const		{ return { x + v.x, y + v.y }; }
	constexpr Vector2 operator-(const Vector2& v) const		{ return { x - v.x, y - v.y }; }
	constexpr Vector2 operator*(float s) const				{ return { x * s, y * s }; }
	constexpr Vector2 operator/(float s) const				{ return { x / s, y / s }; }
	
	constexpr Vector2& operator+=(const Vector2& v)		{ x += v.x; y += v.y; return *this; }
	constexpr Vector2& operator-=(const Vector2& v)		{ x -= v.x; y -= v.y; return *this; }
	constexpr Vector2& operator*= (float s)				{ x *= s; y *= s; return *this; }
	constexpr Vector2& operator/= (float s)				{ x /= s; y /= s; return *this; }

	static constexpr Vector2 Zero()		{ return Vector2{ 0.0f, 0.0f }; }
	static constexpr Vector2 XAxis()	{ return Vector2{ 1.0f, 0.0f }; }
	static constexpr Vector2 YAxis()	{ return Vector2{ 0.0f, 1.0f }; }
};



} // namespace Coo::Math

#endif // !INCLUDED_COOENGINE_MATH_VECTOR2_H
