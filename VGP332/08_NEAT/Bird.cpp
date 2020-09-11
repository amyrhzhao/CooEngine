#include "Bird.h"

void Bird::Update(float deltaTime)
{
	const float gravity = 3000.0f;
	mVelocity.y += gravity * deltaTime;
	mPosition += mVelocity * deltaTime;
}

void Bird::Render()
{
	Coo::Graphics::SimpleDraw::AddScreenCircle(mPosition, mRadius, Coo::Graphics::Colors::Yellow);
}

void Bird::Flap()
{
	mVelocity.y = -500.0f;
}