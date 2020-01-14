#include "Dude.h"
std::random_device myRandomDevice{};
std::mt19937 myRandomEngine{ myRandomDevice() };

float RandomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>{ min, max }(myRandomEngine);
}

Math::Vector2 RandomUnitCircle(bool normalized = true)
{
	Math::Vector2 randVal{
		RandomFloat(-1.0f, 1.0f) + 0.001f,
		RandomFloat(-1.0f, 1.0f)
	};
	if (normalized)
		return Math::Normalize(randVal);
	return randVal;
}

void RunState::Enter(Dude & agent)
{
	destination = RandomUnitCircle() * 250.0f + Math::Vector2{ 600.0f, 350.0f };
}
