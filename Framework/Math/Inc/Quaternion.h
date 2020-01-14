#ifndef INCLUDED_COOENGINE_MATH_QUATERNION_H
#define INCLUDED_COOENGINE_MATH_QUATERNION_H

namespace Coo::Math 
{

struct Quaternion
{
	float x, y, z, w;
	
	constexpr Quaternion() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	constexpr Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
	
	const static Quaternion Zero() { return Quaternion(0.0f, 0.0f, 0.0f, 0.0f); }
	const static Quaternion Identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }

	constexpr Quaternion operator+(const Quaternion& rhs) const { return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	constexpr Quaternion operator-(const Quaternion& rhs) const { return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	constexpr Quaternion operator*(float s) const { return Quaternion(x * s, y * s, z * s, w * s); }
	constexpr Quaternion operator/(float s) const { return Quaternion(x / s, y / s, z / s, w / s); }
	constexpr Quaternion operator*(const Quaternion& rhs) const 
	{
		return Quaternion{
		 x * rhs.w + y * rhs.z - z * rhs.y + w * rhs.x,
		-x * rhs.z + y * rhs.w + z * rhs.x + w * rhs.y,
		 x * rhs.y - y * rhs.x + z * rhs.w + w * rhs.z,
		-x * rhs.x - y * rhs.y - z * rhs.z + w * rhs.w };
	}
};

} // namespace Coo::Math

#endif // !INCLUDED_COOENGINE_MATH_QUATERNION_H
