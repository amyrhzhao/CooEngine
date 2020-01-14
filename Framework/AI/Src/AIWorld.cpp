#include "Precompiled.h"
#include "AIWorld.h"

using namespace Coo::AI;
using namespace Coo::Math;

void AIWorld::AddObstacle(const Circle& obstacle)
{
	mObstacles.push_back(obstacle);
}

void AIWorld::AddWall(const LineSegment& wall)
{
	mWalls.push_back(wall);
}

AgentList Coo::AI::AIWorld::GetNeighborhood(const Coo::Math::Circle & range)
{
	// TODO
	// iterate through every agent
	// check if the agent is in the range
	// add it to return neighborhood
	AgentList neighbors;
	float radiusSqr = range.radius * range.radius;
	for (auto agent : mAgents) 
	{
		auto distSqr = Math::DistanceSqr(agent->position, range.center);
		if (distSqr == 0.0f) { continue; }
		if (distSqr < radiusSqr) 
		{
			neighbors.emplace_back(agent);
		}
	}
	return neighbors;
}

EntityList Coo::AI::AIWorld::GetEntity(int typeId)
{
	if(typeId == 0)
	{
		return mEntities;
	}
	else
	{
		EntityList entities;
		for (auto entity : mEntities) 
		{
			if (entity->typeId == typeId) 
			{
				entities.push_back(entity);
			}
		}
		return entities;
	}
}

void Coo::AI::AIWorld::Update()
{
	// Calculate neighbors for all the agents
	for (auto& agent : mAgents) 
	{
		agent->neighbourhood = GetNeighborhood({ agent->position, agent->neighbourhoodRadius });
	}
}

void Coo::AI::AIWorld::DebugRender()
{
	// Render all the obstacles
	for (auto& obstacle : mObstacles)
	{
		Coo::Graphics::SimpleDraw::AddScreenCircle(obstacle.center, obstacle.radius, Coo::Graphics::Colors::Red);
	}
	// Render all the wall 
	for (auto& wall : mWalls) 
	{
		Coo::Graphics::SimpleDraw::AddScreenLine(wall.from, wall.to, Coo::Graphics::Colors::OrangeRed);
	}

}

void AIWorld::RegisterAgent(Agent* agent)
{
	mAgents.push_back(agent);
}

void AIWorld::UnregisterAgent(Agent* agent)
{
	auto iter = std::find(mAgents.begin(), mAgents.end(), agent);
	if (iter != mAgents.end()) 
	{
		std::iter_swap(iter, mAgents.end() - 1);
		mAgents.pop_back();
	}
}

void Coo::AI::AIWorld::RegisterEntity(Entity* entity)
{
	mEntities.push_back(entity);
}

void Coo::AI::AIWorld::UnregisterEntity(Entity* entity)
{
	auto iter = std::find(mEntities.begin(), mEntities.end(), entity);
	if (iter != mEntities.end())
	{
		std::iter_swap(iter, mEntities.end() - 1);
		mEntities.pop_back();
	}
}
