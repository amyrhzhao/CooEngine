#pragma once

#include <Coo/Inc/Coo.h>

class Pipe
{
public:
	void Update(float deltaTime);
	void Render();

	void Spawn(float gapSize);

	Coo::Math::Rect GetTopRect() const;
	Coo::Math::Rect GetBottomRect() const;

private:
	Coo::Math::Vector2 mPosition;
	Coo::Math::Vector2 mVelocity;
	float mGapSize;
};