#ifndef INCLUDED_COOENGINE_AI_AGENT_H
#define INCLUDED_COOENGINE_AI_AGENT_H

#include "Entity.h"

namespace Coo::AI {

class AIWorld;

class Agent;
using AgentList = std::vector<Agent*>;

class Agent : public Entity
{
public:
	// RULE: You can only create agents, if you have an AIWorld
	Agent(AIWorld& world);
	~Agent();
	Math::Matrix3 LocalToWorld();

	//Math::Matrix4 LocalToWorld()
	//{
	//	Coo::Math::Vector3 look = { 0.0f,0.0f,1.0f };
	//	Coo::Math::Vector3 up = { heading.x, heading.y, 0.0f };
	//	Coo::Math::Vector3 right = Coo::Math::Cross(up,look);
	//	Coo::Math::Vector3 pos = { position.x, position.y, 0.0f };
	//	return
	//	{
	//		right.x, right.y, right.z, 0.0f,
	//		up.x, up.y, up.z, 0.0f,
	//		look.x, look.y, look.z, 0.0f,
	//		pos.x, pos.y, pos.z, 1.0f
	//	};
	//}

	Math::Vector2 velocity = { 1.0f,0.0f };
	Math::Vector2 heading = { 1.0f, 0.0f };
	AgentList neighbourhood;
	float neighbourhoodRadius = 100.0f;
	float maxSpeed = 1.0f;
	float mass = 1.0f;
};

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_AGENT_H
