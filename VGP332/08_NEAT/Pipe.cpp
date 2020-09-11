#include "Pipe.h"

void Pipe::Update(float deltaTime)
{
	mPosition += mVelocity * deltaTime;
}

void Pipe::Render()
{
	auto topRect = GetTopRect();
	Coo::Graphics::SimpleDraw::AddScreenRect(topRect.r, topRect.b, topRect.g, topRect.a, Coo::Graphics::Colors::Green);
	auto bottomRect = GetBottomRect();
	Coo::Graphics::SimpleDraw::AddScreenRect(bottomRect.r, bottomRect.b, bottomRect.g, bottomRect.a, Coo::Graphics::Colors::Green);
}

void Pipe::Spawn(float gapSize)
{
	mGapSize = gapSize;
	mVelocity = Coo::Math::Vector2{ -100.0f, 0.0f };
	mPosition.x = (float)500;
	mPosition.y = Coo::Math::RandomFloat(gapSize, 720 - gapSize);
}

Coo::Math::Vector4 Pipe::GetTopRect() const
{
	return {
		mPosition.x,
		mPosition.y - mGapSize - 720, // Height
		mPosition.x + mGapSize,
		mPosition.y - mGapSize,
	};
}

Coo::Math::Vector4 Pipe::GetBottomRect() const
{
	return {
		mPosition.x,
		mPosition.y + mGapSize,
		mPosition.x + mGapSize,
		mPosition.y + mGapSize + 500, // Width
	};
}