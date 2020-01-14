#include "GraphUtil.h"

using namespace Coo::Graphics;

void BuildGridBasedGraph(NavGraph& graph, const TileMap& map)
{
	auto columns = map.GetColumns();
	auto rows = map.GetRows();
	graph.Initialize(columns*rows);
	auto width = map.GetTileWidth();
	auto height = map.GetTileHeight();
	Vector2 position = {width * 0.5f, height * 0.5f};
	for (auto i = 0; i < columns * rows; ++i)
	{
		MapNode& currNode = graph.GetNode(i);
		size_t column = i % columns;
		size_t row = i / columns;
		currNode.position = position + Math::Vector2{ static_cast<float>(column*width) , static_cast<float>(row*height) };
		if (column != 0) { currNode.neighbors.push_back(i - 1); }
		if (row != 0) { currNode.neighbors.push_back(i - columns); }
		if (row != rows - 1) { currNode.neighbors.push_back(i + columns); }
		if (column != columns - 1) { currNode.neighbors.push_back(i + 1); }
		if (row != 0 && column != 0) { currNode.neighbors.push_back(i - columns - 1); }
		if (row != rows - 1 && column != 0) { currNode.neighbors.push_back(i + columns - 1); }
		if (row != 0 && column != columns - 1) { currNode.neighbors.push_back(i - columns + 1); }
		if (row != rows - 1 && column != columns - 1) { currNode.neighbors.push_back(i + columns + 1); }
	}
}

void RenderGraph(const NavGraph& graph, std::function<bool(size_t)> isWall)
{
	for (size_t i = 0; i < graph.GetSize(); ++i)
	{
		// Draw Screen Circles
		auto currNode = graph.GetNode(i);
		if (isWall(i))
		{
			SimpleDraw::AddScreenCircle(currNode.position, 5.0f, Colors::Red);
		}
		else
		{
			SimpleDraw::AddScreenCircle(currNode.position, 5.0f, Colors::YellowGreen);
		}
		// Draw Screen Lines
		for (size_t j = 0; j < currNode.neighbors.size(); ++j)
		{
			SimpleDraw::AddScreenLine(
				currNode.position,
				graph.GetNode(currNode.neighbors[j]).position,
				Colors::GreenYellow);
		}
	}
}

void RenderSearch(const NavGraph& graph,const GraphSearch::Context& context)
{
	for (size_t node : context.closedList)
	{
		auto parent = context.parent[node];
		if (parent != InvalidNode && node != InvalidNode)
		{
			SimpleDraw::AddScreenLine(graph.GetNode(node).position, graph.GetNode(parent).position, Colors::Red);
		}
	}

	if (context.found)
	{
		std::list<size_t> path;
		auto next = context.closedList.back();
		while (next != AI::InvalidNode)
		{
			path.push_front(next);
			next = context.parent[next];
		}
		for (size_t node : path)
		{
			SimpleDraw::AddScreenCircle(graph.GetNode(node).position, 10.0f, Colors::Cyan);
			auto parent = context.parent[node];
			if (parent != InvalidNode && node != InvalidNode)
			{
				SimpleDraw::AddScreenLine(graph.GetNode(node).position, graph.GetNode(parent).position, Colors::Cyan);
			}
		}
	}
}
