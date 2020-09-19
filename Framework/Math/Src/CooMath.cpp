#include "Precompiled.h"
#include "CooMath.h"

namespace
{
	std::random_device mRandomDevice{};
	std::mt19937 mRandomEngine{ mRandomDevice() };
}

bool Coo::Math::Intersect(const Circle& c0, const Circle& c1)
{
	const float fRadii = c0.radius + c1.radius;
	const float fRadiiSquared = fRadii * fRadii;
	const float fDistanceSquared = DistanceSqr(c0.center, c1.center);
	return (fDistanceSquared < fRadiiSquared);
}

bool Coo::Math::Intersect(const AABB & aabb0, const AABB & aabb1)
{
	Vector3 amin = aabb0.center - aabb0.extend;
	Vector3 amax = aabb0.center + aabb0.extend;
	Vector3 bmin = aabb1.center - aabb1.extend;
	Vector3 bmax = aabb1.center + aabb1.extend;
	return (amin.x <= bmax.x && amax.x >= bmin.x) &&
		(amin.y <= bmax.y && amax.y >= bmin.y) &&
		(amin.z <= bmax.z && amax.z >= bmin.z);
}

bool Coo::Math::Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit)
{
	// https://truesculpt.googlecode.com/hg-history/Release%25200.8/Doc/ray_box_intersect.pdf

	// Returns true if ray intersects bounding box
	// Sets d1 to entry distance, d2 to exit distance (along ray.dir)

	Vector3 boxMin = aabb.center - aabb.extend;
	Vector3 boxMax = aabb.center + aabb.extend;
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	float divx = 1.0f / ray.dir.x;
	float divy = 1.0f / ray.dir.y;
	float divz = 1.0f / ray.dir.z;

	if (divx >= 0.0f)
	{
		tmin = (boxMin.x - ray.org.x) * divx;
		tmax = (boxMax.x - ray.org.x) * divx;
	}
	else
	{
		tmin = (boxMax.x - ray.org.x) * divx;
		tmax = (boxMin.x - ray.org.x) * divx;
	}
	if (divy >= 0.0f)
	{
		tymin = (boxMin.y - ray.org.y) * divy;
		tymax = (boxMax.y - ray.org.y) * divy;
	}
	else
	{
		tymin = (boxMax.y - ray.org.y) * divy;
		tymax = (boxMin.y - ray.org.y) * divy;
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	if (divz >= 0.0f)
	{
		tzmin = (boxMin.z - ray.org.z) * divz;
		tzmax = (boxMax.z - ray.org.z) * divz;
	}
	else
	{
		tzmin = (boxMax.z - ray.org.z) * divz;
		tzmax = (boxMin.z - ray.org.z) * divz;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}

	if (tzmax < tmax)
	{
		tmax = tzmax;
	}

	distEntry = tmin;
	distExit = tmax;
	return true;
}

bool Coo::Math::Intersect(const Ray & ray, const Vector3 & a, const Vector3 & b, const Vector3 & c, float & distance)
{
	// Reference: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

	// Find vectors for two edges sharing V1
	Vector3 e1 = b - a;
	Vector3 e2 = c - a;

	// Begin calculating determinant - also used to calculate u parameter
	Vector3 P = Cross(ray.dir, e2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = Dot(e1, P);
	// NOT CULLING
	if (IsZero(det))
	{
		return false;
	}

	float inv_det = 1.0f / det;

	// Calculate distance from V1 to ray origin
	Vector3 T = ray.org - a;

	// Calculate u parameter and test bound
	float u = Dot(T, P) * inv_det;

	// The intersection lies outside of the triangle
	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	// Prepare to test v parameter
	Vector3 Q = Cross(T, e1);

	// Calculate V parameter and test bound
	float v = Dot(ray.dir, Q) * inv_det;

	// The intersection lies outside of the triangle
	if (v < 0.0f || u + v  > 1.0f)
	{
		return false;
	}

	// Ray intersection
	float t = Dot(e2, Q) * inv_det;
	if (t <= 0.0f)
	{
		// No hit, no win
		return false;
	}

	distance = t;
	return true;
}

bool Coo::Math::Intersect(const Vector3 & point, const AABB & aabb)
{
	const Vector3 test = point - aabb.center;
	if (abs(test.x) > aabb.extend.x) return false;
	if (abs(test.y) > aabb.extend.y) return false;
	if (abs(test.z) > aabb.extend.z) return false;
	return true;
}

bool Coo::Math::Intersect(const Rect& r, const Circle& c)
{
	Vector2 closestPoint;
	closestPoint.x = Clamp(c.center.x, r.left, r.right);
	closestPoint.y = Clamp(c.center.y, r.top, r.bottom);

	const float distance = Distance(closestPoint, c.center);
	if (distance > c.radius)
	{
		return false;
	}
	return true;
}

float Coo::Math::RandomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>{min, max}(mRandomEngine);
}

int Coo::Math::Random()
{
	return std::uniform_int_distribution<>{0, std::numeric_limits<int>::max()}(mRandomEngine);
}

int Coo::Math::Random(int min, int max)
{
	return std::uniform_int_distribution<>{min, max}(mRandomEngine);
}

double Coo::Math::RandomReal()
{
	return std::uniform_real_distribution<double>{0.0, 1.0}(mRandomEngine);
}

double Coo::Math::RandomReal(double min, double max)
{
	return std::uniform_real_distribution<double>{min, max}(mRandomEngine);
}