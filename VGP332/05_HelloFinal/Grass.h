#pragma once
#include "Coo/Inc/Coo.h"

class Grass : public Coo::AI::Entity 
{
public:
	Grass(Coo::AI::AIWorld& world) : Coo::AI::Entity(world) {}
	void Initialize(Coo::Math::Vector2 position);
	Coo::Math::Circle GetBoundingCircle() { return {position, radius}; }

	bool IsActive() const { return isActive; }
	float Cost(float eat);
	void Render() const;

private:
	bool isActive = true;
	float resource;
};