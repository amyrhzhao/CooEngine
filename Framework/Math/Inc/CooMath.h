#pragma once
#ifndef INCLUDED_COOENGINE_COOMATH_H
#define INCLUDED_COOENGINE_COOMATH_H

#include "Common.h"

#include "AABB.h"
#include "Circle.h"
#include "LineSegment.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "PerlinNoise.h"
#include "Ray.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

#include "MetaRegistration.h"

namespace Coo::Math {

	// Constants
	constexpr static float Pi = 3.14159265358979f;
	constexpr static float TwoPi = 6.28318530717958f;
	constexpr static float PiByTwo = 1.57079632679489f;
	constexpr static float RootTwo = 1.41421356237309f;
	constexpr static float RootThree = 1.73205080756887f;
	constexpr static float DegToRad = Pi / 180.0f;
	constexpr static float RadToDeg = 180.0f / Pi;

	// Helper functions
	template<class T>
	T max(T t1, T t2)
	{
		return (t1 > t2 ? t1 : t2);
	}
	template<class T>
	T min(T t1, T t2)
	{
		return (t1 < t2 ? t1 : t2);
	}
	template<class T>
	T abs(T t)
	{
		return (t > 0 ? t : -t);
	}
	template<class T>
	constexpr T Lerp(T t1, T t2, float t)
	{
		return t1 + ((t2 - t1) * t);
	}
	inline bool Compare(float a, float b, float epsilon = FLT_MIN) { return abs(a - b) <= epsilon; }
	inline bool IsZero(float value) { return Compare(value, 0.0f); }

	// Vector2
	constexpr float Dot(const Vector2& v0, const Vector2& v1)
	{
		return (v0.x * v1.x) + (v0.y * v1.y);
	}
	constexpr float MagnitudeSqr(const Vector2& v)
	{
		return Dot(v, v);
	}
	constexpr float DistanceSqr(const Vector2& v0, const Vector2& v1)
	{
		return MagnitudeSqr(v0 - v1);
	}
	inline float Magnitude(const Vector2& v)
	{
		return sqrt(MagnitudeSqr(v));
	}
	inline Vector2 Normalize(const Vector2& v)
	{
		return v * (1.0f / Magnitude(v));
	}
	inline float Distance(const Vector2& v0, const Vector2& v1)
	{
		return sqrt(DistanceSqr(v0, v1));
	}
	inline Vector2 Truncate(const Vector2& v, float max)
	{
		float magnitude = Magnitude(v);
		float i = magnitude != 0.0f ? max / magnitude : 0.0f;
		if (i < 1.0f)
		{
			return v * i;
		}
		return v;
	}
	inline Vector2 TransformCoord(const Vector2& v, const Matrix3& m)
	{
		return Vector2
		(
			v.x * m._11 + v.y * m._21 + m._31,
			v.x * m._12 + v.y * m._22 + m._32
		);
	}
	constexpr Vector2 Lerp(const Vector2& v0, const Vector2& v1, float t)
	{
		return v0 + ((v1 - v0) * t);
	}

	// Vector3
	constexpr float Dot(const Vector3& v0, const Vector3& v1)
	{
		return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z);
	}
	constexpr float MagnitudeSqr(const Vector3& v)
	{
		return Dot(v, v);
	}
	constexpr float DistanceSqr(const Vector3& v0, const Vector3& v1)
	{
		return MagnitudeSqr(v0 - v1);
	}
	constexpr Vector3 Cross(const Vector3& v0, const Vector3& v1)
	{
		return { (v0.y * v1.z) - (v0.z * v1.y),
				(v0.z * v1.x) - (v0.x * v1.z),
				(v0.x * v1.y) - (v0.y * v1.x) };
	}
	inline float Magnitude(const Vector3& v)
	{
		return sqrt(MagnitudeSqr(v));
	}
	inline float Distance(const Vector3& v0, const Vector3& v1)
	{
		return sqrt(DistanceSqr(v0, v1));
	}
	inline Vector3 Normalize(const Vector3& v)
	{
		return v * (1.0f / Magnitude(v));
	}
	constexpr Vector3 RotationX(const Vector3& v, float rad)
	{
		return
		{
			v.x,
			v.y * cosf(rad) - v.z * sinf(rad),
			v.y * sinf(rad) + v.z * cosf(rad)
		};
	}
	constexpr Vector3 RotationY(const Vector3& v, float rad)
	{
		return
		{
			v.x * cosf(rad) + v.z * sinf(rad),
			v.y,
			v.z * cosf(rad) - v.x * sinf(rad)
		};
	}
	constexpr Vector3 RotationZ(const Vector3& v, float rad)
	{
		return
		{
			v.x * cosf(rad) - v.y * sinf(rad),
			v.x * sinf(rad) + v.y * cosf(rad),
			v.z
		};
	}
	inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		return Vector3
		(
			v.x * m._11 + v.y * m._21 + v.z * m._31,
			v.x * m._12 + v.y * m._22 + v.z * m._32,
			v.x * m._13 + v.y * m._23 + v.z * m._33
		);
	}
	constexpr Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t)
	{
		return v0 + ((v1 - v0) * t);
	}

	// Matrix3
	constexpr float Determinant(const Matrix3& m)
	{
		float det = 0.0f;
		det = (m._11 * (m._22 * m._33 - m._23 * m._32));
		det -= (m._12 * (m._21 * m._33 - m._23 * m._31));
		det += (m._13 * (m._21 * m._32 - m._22 * m._31));
		return det;
	}
	constexpr Matrix3 Adjoint(const Matrix3& m)
	{
		return Matrix3
		(
			(m._22 * m._33 - m._23 * m._32),
			-(m._12 * m._33 - m._13 * m._32),
			(m._12 * m._23 - m._13 * m._22),

			-(m._21 * m._33 - m._23 * m._31),
			(m._11 * m._33 - m._13 * m._31),
			-(m._11 * m._23 - m._13 * m._21),

			(m._21 * m._32 - m._22 * m._31),
			-(m._11 * m._32 - m._12 * m._31),
			(m._11 * m._22 - m._12 * m._21)
		);
	}
	constexpr Matrix3 Inverse(const Matrix3& m)
	{
		const float determinant = Determinant(m);
		const float invDet = 1.0f / determinant;
		return Adjoint(m) * invDet;
	}
	constexpr Matrix3 Transpose(const Matrix3& m)
	{
		return Matrix3
		(
			m._11, m._21, m._31,
			m._12, m._22, m._32,
			m._13, m._23, m._33
		);
	}
	constexpr Matrix3 Translation(float x, float y) { return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, 1.0f); }
	constexpr Matrix3 Translation(const Vector2& v) { return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, 1.0f); }
	constexpr Matrix3 Rotation(float rad) { return Matrix3(cosf(rad), sinf(rad), 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 1.0f); }
	constexpr Matrix3 Scaling(float s) { return Matrix3(s, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 1.0f); }
	constexpr Matrix3 Scaling(float sx, float sy) { return Matrix3(sx, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 1.0f); }
	constexpr Matrix3 Scaling(const Vector2& s) { return Matrix3(s.x, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 1.0f); }

	// Matrix4
	constexpr float Determinant(const Matrix4& m)
	{
		float det = 0.0f;
		det = (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
		det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
		det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
		det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
		return det;
	}
	constexpr Matrix4 Adjoint(const Matrix4& m)
	{
		return Matrix4
		{
			 (m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
			-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
			 (m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
			-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

			-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
			 (m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
			-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
			 (m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

			 (m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
			-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
			 (m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
			-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

			-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
			 (m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
			-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
			 (m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
		};
	}
	constexpr Matrix4 Inverse(const Matrix4& m)
	{
		const float determinant = Determinant(m);
		const float invDet = 1.0f / determinant;
		return Adjoint(m) * invDet;
	}
	constexpr Matrix4 Transpose(const Matrix4& m)
	{
		return Matrix4
		{
			m._11, m._21, m._31, m._41,
			m._12, m._22, m._32, m._42,
			m._13, m._23, m._33, m._43,
			m._14, m._24, m._34, m._44
		};
	}
	constexpr Matrix4 Scale(float s)
	{
		return Matrix4
		{
			   s, 0.0f, 0.0f, 0.0f,
			0.0f,    s, 0.0f, 0.0f,
			0.0f, 0.0f,    s, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	constexpr Matrix4 Scale(float sx, float sy, float sz)
	{
		return Matrix4
		{
			  sx, 0.0f, 0.0f, 0.0f,
			0.0f,   sy, 0.0f, 0.0f,
			0.0f, 0.0f,   sz, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	constexpr Matrix4 Scale(const Vector3 & s)
	{
		return Scale(s.x, s.y, s.z);
	}
	constexpr Matrix4 Translate(float x, float y, float z)
	{
		return Matrix4
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			   x,    y,    z, 1.0f
		};
	}
	constexpr Matrix4 Translate(const Vector3 & v)
	{
		return Translate(v.x, v.y, v.z);
	}
	constexpr Matrix4 RotationX(float rad)
	{
		return Matrix4
		{
			1.0f,       0.0f,      0.0f, 0.0f,
			0.0f,  cosf(rad), sinf(rad), 0.0f,
			0.0f, -sinf(rad), cosf(rad), 0.0f,
			0.0f,       0.0f,      0.0f, 1.0f
		};
	}
	constexpr Matrix4 RotationY(float rad)
	{
		return Matrix4
		{
			cosf(rad), 0.0f, -sinf(rad), 0.0f,
				 0.0f, 1.0f,       0.0f, 0.0f,
			sinf(rad), 0.0f,  cosf(rad), 0.0f,
				 0.0f, 0.0f,       0.0f, 1.0f
		};
	}
	constexpr Matrix4 RotationZ(float rad)
	{
		return Matrix4
		{
			cosf(rad), sin(rad), 0.0f, 0.0f,
		   -sinf(rad), cos(rad), 0.0f, 0.0f,
				 0.0f,     0.0f, 1.0f, 0.0f,
				 0.0f,     0.0f, 0.0f, 1.0f
		};
	}
	inline Matrix4 RotationAxis(const Vector3& axis, float rad)
	{
		const Vector3 u = Normalize(axis);
		const float x = u.x;
		const float y = u.y;
		const float z = u.z;
		const float s = sin(rad);
		const float c = cos(rad);

		return Matrix4
		(
			c + (x * x * (1.0f - c)),
			x * y * (1.0f - c) + z * s,
			x * z * (1.0f - c) - y * s,
			0.0f,

			x * y * (1.0f - c) - z * s,
			c + (y * y * (1.0f - c)),
			y * z * (1.0f - c) + x * s,
			0.0f,

			x * z * (1.0f - c) + y * s,
			y * z * (1.0f - c) - x * s,
			c + (z * z * (1.0f - c)),
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
	}
	inline Matrix4 MatrixRotationQuaternion(const Quaternion& q)
	{
		return Matrix4
		(
			1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
			(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
			(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
			0.0f,

			(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
			1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
			(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
			0.0f,

			(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
			(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
			1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
	}

	// Quaternion
	constexpr Quaternion Conjugate(const Quaternion& q) { return Quaternion(-q.x, -q.y, -q.z, q.w); }
	inline float Magnitude(const Quaternion& q) { return sqrtf((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w)); }
	inline Quaternion Normalize(const Quaternion& q) { return q / Magnitude(q); }
	inline Quaternion QuaternionRotationMatrix(const Matrix4& m)
	{
		//	trace: https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
		float qw = sqrtf(1.0f + m._11 + m._22 + m._33) * 0.5f;
		float w4 = 4.0f * qw;
		return Quaternion
		{
			(m._32 - m._23) / w4,
			(m._13 - m._31) / w4,
			(m._21 - m._12) / w4,
			qw
		};
	}
	inline Quaternion QuaternionLookRotation(const Vector3& forward, const Vector3& up)
	{
		Vector3 z = Normalize(forward);
		Vector3 x = Normalize(Cross(up, z));
		Vector3 y = Normalize(Cross(z, x));
		Matrix4 mat
		{
			x.x, x.y, x.z, 0.0f,
			y.x, y.y, y.z, 0.0f,
			z.x, z.y, z.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		return QuaternionRotationMatrix(mat);
	}
	inline Quaternion QuaternionRotationAxis(const Vector3& axis, float rad)
	{
		const float c = cos(rad * 0.5f);
		const float s = sin(rad * 0.5f);
		const Math::Vector3 a = Math::Normalize(axis);
		return Quaternion
		(
			a.x * s,
			a.y * s,
			a.z * s,
			c
		);

	}
	inline Quaternion QuaternionFromToRotation(const Vector3& from, const Vector3& to)
	{
		float cos_theta = Dot(Normalize(from), Normalize(to));
		float angle = acosf(cos_theta);
		Vector3 w = Normalize(Cross(from, to));
		return QuaternionRotationAxis(w, angle);
	}
	inline Quaternion Lerp(Quaternion q0, Quaternion q1, float t) { return q0 * (1.0f - t) + (q1 * t); }
	inline Quaternion Slerp(Quaternion q0, Quaternion q1, float t)
	{
		float dot = (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z) + (q0.w * q1.w);
		if (dot < 0.0f)
		{
			dot = -dot;
			q1.x = -q1.x;
			q1.y = -q1.y;
			q1.z = -q1.z;
			q1.w = -q1.w;
		}
		else if (dot > 0.999f)
		{
			return Normalize(Lerp(q0, q1, t));
		}

		float theta = acosf(dot);
		float sintheta = sinf(theta);
		float scale0 = sinf(theta * (1.0f - t)) / sintheta;
		float scale1 = sinf(theta * t) / sintheta;

		return Quaternion
		(
			(q0.x * scale0) + (q1.x * scale1),
			(q0.y * scale0) + (q1.y * scale1),
			(q0.z * scale0) + (q1.z * scale1),
			(q0.w * scale0) + (q1.w * scale1)
		);
	}

	// Intersection
	bool Intersect(const Circle& c0, const Circle& c1);
	bool Intersect(const AABB& aabb0, const AABB& aabb1);
	bool Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit);
	bool Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, float& distance);
	bool Intersect(const Vector3& point, const AABB& aabb);

	// Random
	int Random();
	int Random(int min, int max);
	float RandomFloat(float min, float max);
	double RandomReal();
	double RandomReal(double min, double max);

} // namespace Coo::Math


#endif // !INCLUDED_COOENGINE_COOMATH_H
