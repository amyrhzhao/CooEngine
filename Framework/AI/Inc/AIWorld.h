#ifndef INCLUDED_COOENGINE_AI_AIWORLD_H
#define INCLUDED_COOENGINE_AI_AIWORLD_H

#include "Agent.h"
#include "Graph.h"

namespace Coo::AI
{
class AIWorld 
{
public:
	using Obstacles = std::vector<Coo::Math::Circle>;
	using Walls = std::vector<Coo::Math::LineSegment>;

	// TODO: add Update
	// Update the world
	// optimize: spactial partitioning
	// bucket the agent
	// Grid -> std::vector<Cell> 
	// Cell -> agent
	// rows, columns, size
	
	//	struct Grid
	//	{
	//		struct Cell 
	//		{
	//			AgentList agents;
	//		};
	//		
	//		int rows;
	//		int columns;
	//		float size;
	//	};

	void Update();
	void DebugRender();

	void RegisterAgent(Agent* agent);
	void UnregisterAgent(Agent* agent);

	void RegisterEntity(Entity* entity);
	void UnregisterEntity(Entity* entity);
	
	void AddObstacle(const Coo::Math::Circle& obstacle);
	const Obstacles& GetObstacles() const {	return mObstacles;};

	void AddWall(const Coo::Math::LineSegment& wall);
	const Walls& GetWalls() const { return mWalls; }

	AgentList GetNeighborhood(const Coo::Math::Circle& range);
	EntityList GetEntity(int typeId = 0);

private:
	EntityList mEntities;
	AgentList mAgents;
	Obstacles mObstacles;
	Walls mWalls;
};

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_AIWORLD_H
