#pragma once

#include <Coo/Inc/Coo.h>

class Pipe
{
public:
	void Update(float deltaTime);
	void Render();

	void Spawn(float gapSize);

	Coo::Math::Vector4 GetTopRect() const;
	Coo::Math::Vector4 GetBottomRect() const;

private:
	Coo::Math::Vector2 mPosition;
	Coo::Math::Vector2 mVelocity;
	float mGapSize;
};