#pragma once

#include <Coo/Inc/Coo.h>

class Bird
{
public:
	void Update(float deltaTime);
	void Render();

	void Flap();

	void SetPosition(const Coo::Math::Vector2& pos) { mPosition = pos; }
	const Coo::Math::Vector2& GetPosition() const { return mPosition; }

private:
	Coo::Math::Vector2 mPosition;
	Coo::Math::Vector2 mVelocity;
	float mRadius = 30.0f;
};