#include "Pipe.h"

void Pipe::Update(float deltaTime)
{
	mPosition += mVelocity * deltaTime;
}

void Pipe::Render()
{
	auto topRect = GetTopRect();
	Coo::Graphics::SimpleDraw::AddScreenRect(topRect.left, topRect.right, topRect.top, topRect.bottom, Coo::Graphics::Colors::Green);
	auto bottomRect = GetBottomRect();
	Coo::Graphics::SimpleDraw::AddScreenRect(bottomRect.left, bottomRect.right, bottomRect.top, bottomRect.bottom, Coo::Graphics::Colors::Green);
}

void Pipe::Spawn(float gapSize)
{
	mGapSize = gapSize;
	mVelocity = Coo::Math::Vector2{ -100.0f, 0.0f };
	mPosition.x = (float)500;
	mPosition.y = Coo::Math::RandomFloat(gapSize, 720 - gapSize);
}

Coo::Math::Rect Pipe::GetTopRect() const
{
	return {
		mPosition.x,
		-FLT_MAX,
		mPosition.x + mGapSize,
		mPosition.y - mGapSize,
	};
}

Coo::Math::Rect Pipe::GetBottomRect() const
{
	return {
		mPosition.x,
		mPosition.y + mGapSize,
		mPosition.x + mGapSize,
		FLT_MAX
	};
}