#ifndef INCLUDED_GRAPHUTIL_H
#define INCLUDED_GRAPHUTIL_H

#include "TileMap.h"

using namespace Coo;
using namespace Coo::Math;
using namespace Coo::AI;

enum class SearchType
{
	DFS,
	BFS,
	Dijkstra,
	AStar
};

struct MapNode : public AI::Node
{
	Vector2 position;
};

using NavGraph = AI::Graph<MapNode>;

void BuildGridBasedGraph(NavGraph& graph, const TileMap& map);
void RenderGraph(const NavGraph& graph, std::function<bool(size_t)> isWall);

template<typename ValidationPolicy1,typename  ValidationPolicy2, typename CostPolicy, typename HeuristicPolicy>
void ExecuteSearch(const NavGraph& graph, GraphSearch::Context& context, size_t start, size_t end, SearchType search) 
{
	context.start = start;
	context.end = end;
	switch (search)
	{
	case SearchType::DFS:
	{
		AI::GraphSearch::DFS<NavGraph, ValidationPolicy1> dfs(graph);
		dfs.Execute(context);
		break;
	}
	case SearchType::BFS:
	{
		AI::GraphSearch::BFS<NavGraph, ValidationPolicy1> bfs(graph);
		bfs.Execute(context);
		break;
	}
	case SearchType::Dijkstra:
	{
		AI::GraphSearch::Dijkstra<NavGraph, ValidationPolicy2, CostPolicy> dijkstra(graph);
		dijkstra.Execute(context);
		break;
	}
	case SearchType::AStar: 
	{
		AI::GraphSearch::AStar<NavGraph, ValidationPolicy2, CostPolicy, HeuristicPolicy> aStar(graph);
		aStar.Execute(context);
		break;
	}
	}
}

void RenderSearch(const NavGraph& graph, const GraphSearch::Context& context);

#endif // !INCLUDED_GRAPHUTIL_H