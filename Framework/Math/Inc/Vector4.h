#ifndef INCLUDED_COOENGINE_MATH_VECTOR4_H
#define INCLUDED_COOENGINE_MATH_VECTOR4_H

namespace Coo::Math {

struct Vector4
{
	union{float x; float r;};
	union{float y; float g;};
	union{float z; float b;};
	union{float w; float a;};

	constexpr Vector4() noexcept : Vector4{ 0.0f } {}
	constexpr explicit Vector4(float f) noexcept : Vector4{f, f, f, f} {}
	constexpr Vector4(const Vector3& v, float w) noexcept : Vector4{ v.x, v.y, v.z, w } {}
	constexpr Vector4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
	
	constexpr Vector4 operator-() const						{ return { -x, -y, -z, -w}; }
	constexpr Vector4 operator+(const Vector4& v) const		{ return { x + v.x, y + v.y, z + v.z, w + v.w}; }
	constexpr Vector4 operator-(const Vector4& v) const		{ return { x - v.x, y - v.y, z - v.z, w - v.w}; }
	constexpr Vector4 operator*(float s) const				{ return { x * s, y * s, z * s, w * s }; }
	constexpr Vector4 operator/(float s) const				{ return { x / s, y / s, z / s, w / s }; }

	constexpr Vector4& operator+=(const Vector4& v)		{ x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	constexpr Vector4& operator-=(const Vector4& v)		{ x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	constexpr Vector4& operator*=(float s)				{ x *= s; y *= s; z *= s; w *= s; return *this; }
	constexpr Vector4& operator/=(float s)				{ x /= s; y /= s; z /= s; w /= s; return *this; }
};

} // namespace Coo::Math

#endif // !INCLUDED_COOENGINE_MATH_VECTOR4_H
