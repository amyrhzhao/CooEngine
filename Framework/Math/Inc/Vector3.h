#ifndef INCLUDED_COOENGINE_MATH_VECTOR3_H
#define INCLUDED_COOENGINE_MATH_VECTOR3_H

namespace Coo::Math {

struct Vector3
{
	union 
	{
		struct { float x, y, z; };
		std::array<float, 3> v;
	};

	constexpr Vector3() noexcept : Vector3{ 0.0f } {}
	constexpr explicit Vector3(float f) noexcept : x{ f }, y{ f }, z{ f } {}
	constexpr Vector3(float x, float y, float z) noexcept : x{ x }, y{ y }, z{ z } {}

	constexpr Vector3 operator-() const						{ return { -x, -y, -z }; }
	constexpr Vector3 operator+(const Vector3& vec) const	{ return { x + vec.x, y + vec.y, z + vec.z }; }
	constexpr Vector3 operator-(const Vector3& vec) const	{ return { x - vec.x, y - vec.y, z - vec.z }; }
	constexpr Vector3 operator*(float s) const				{ return { x * s, y * s, z * s }; }
	constexpr Vector3 operator/(float s) const				{ return { x / s, y / s, z / s }; }
	
	constexpr Vector3& operator+=(const Vector3& vec)	{ x += vec.x; y += vec.y; z += vec.z; return *this; }
	constexpr Vector3& operator-=(const Vector3& vec)	{ x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
	constexpr Vector3& operator*=(float s)				{ x *= s; y *= s; z *= s; return *this; }
	constexpr Vector3& operator/=(float s)				{ x /= s; y /= s; z /= s; return *this; }
	
	static constexpr Vector3 Zero()		{ return Vector3{ 0.0f, 0.0f, 0.0f }; }
	static constexpr Vector3 XAxis()	{ return Vector3{ 1.0f, 0.0f, 0.0f }; }
	static constexpr Vector3 YAxis()	{ return Vector3{ 0.0f, 1.0f, 0.0f }; }
	static constexpr Vector3 ZAxis()	{ return Vector3{ 0.0f, 0.0f, 1.0f }; }
	
};


} // namespace Coo::Math

#endif // !INCLUDED_COOENGINE_MATH_VECTOR3_H

