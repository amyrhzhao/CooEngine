#pragma once

#include <Coo/Inc/Coo.h>

class Wolf : public Coo::AI::Agent 
{
public:
	Wolf(Coo::AI::AIWorld& world) : Coo::AI::Agent(world) {}
	void Initialize(Coo::Math::Vector2 position);
	Coo::Math::Circle GetBoundingCircle() { return { position,radius }; }
};