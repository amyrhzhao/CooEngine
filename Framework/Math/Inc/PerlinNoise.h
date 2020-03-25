#ifndef INCLUDED_COOENGINE_MATH_PERLINNOISE_H
#define INCLUDED_COOENGINE_MATH_PERLINNOISE_H

namespace Coo::Math 
{
	class PerlinNoise 
	{
	public:
		PerlinNoise();
		PerlinNoise(uint32_t seed);

		float Get(float x, float y, float z);
	private:
		float Fade(float t);
		float Grad(int hash, float x, float y, float z);

		std::vector<int> p;
	};
} // namespace Coo::Math

#endif // !INCLUDED_COOENGINE_MATH_PERLINNOISE_H
