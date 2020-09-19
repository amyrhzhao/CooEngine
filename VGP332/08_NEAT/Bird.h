#pragma once

#include <Coo/Inc/Coo.h>

class PipeManager;

class Bird
{
public:
	void Update(float deltaTime, const PipeManager& pm);
	void Render();

	void Flap();

	void SetPosition(const Coo::Math::Vector2& pos) { mPosition = pos; }
	const Coo::Math::Vector2& GetPosition() const { return mPosition; }
	bool IsAlive() const { return mAlive; }
	Coo::Math::Circle GetBound() const { return { mPosition, mRadius }; }

	std::unique_ptr<Coo::AI::NEAT::NeuralNet> brain;
	float fitness = 0.0f;

	void Spawn(const Coo::Math::Vector2& pos);
	void Kill();

private:
	Coo::Math::Vector2 mPosition;
	Coo::Math::Vector2 mVelocity;
	float mRadius = 30.0f;
	bool mAlive = true;
};