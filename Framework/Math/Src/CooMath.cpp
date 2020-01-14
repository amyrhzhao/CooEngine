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
