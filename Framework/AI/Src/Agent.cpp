#include "Precompiled.h"
#include "AIWorld.h"
#include "Agent.h"

using namespace Coo::AI;
using namespace Coo::Math;

Agent::Agent(AIWorld& world) : Entity(world)
{
	world.RegisterAgent(this);
}

Agent::~Agent()
{
	world.UnregisterAgent(this);
}

Matrix3 Agent::LocalToWorld()
{
	Math::Vector2 y = heading;
	Math::Vector2 x = { heading.y, -heading.x };
	Math::Vector2 p = position;
	return
	{
		x.x, x.y, 0.0f,
		y.x, y.y, 0.0f,
		p.x, p.y, 1.0f
	};
}

