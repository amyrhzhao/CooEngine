#include "Grass.h"

const float initResource = 10.0f;

void Grass::Initialize(Coo::Math::Vector2 pos)
{
	position = pos;
	radius = 10.0f;
	resource = initResource;
	isActive = true;
}

float Grass::Cost(float eat)
{
	if (eat < resource) 
	{
		resource -= eat;
		return eat;
	}
	else
	{
		resource = 0.0f;
		isActive = false;
		return resource;
	}
}

void Grass::Render() const
{
	if (isActive) 
	{
		Coo::Graphics::SimpleDraw::AddScreenCircle(position, radius, Coo::Graphics::Colors::Green);
	}
}
