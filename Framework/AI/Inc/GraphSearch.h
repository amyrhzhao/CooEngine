#ifndef INCLUDED_COOENGINE_AI_GRAPHSEARCH_H
#define INCLUDED_COOENGINE_AI_GRAPHSEARCH_H

#include "Graph.h"

namespace Coo::AI::GraphSearch
{

struct Context
	{
		size_t nodeCount;
		std::list<size_t> openList;
		std::list<size_t> closedList;
		std::vector<size_t> parent;
		std::vector<bool> open;
		std::vector<bool> closed;
		std::vector<float> g;
		std::vector<float> h;
		size_t start = InvalidNode;
		size_t end = InvalidNode;
		bool found = false;

		void Init(size_t nodes)
		{
			nodeCount = nodes;
		}

		void Reset()
		{
			openList.clear();
			closedList.clear();
			parent.clear();
			open.clear();
			closed.clear();
			g.clear();
			h.clear();

			parent.resize(nodeCount, InvalidNode);
			open.resize(nodeCount, false);
			closed.resize(nodeCount, false);
			g.resize(nodeCount, 0.0f);
			h.resize(nodeCount, 0.0f);

			found = false;
		}

	};

template <typename GraphType, typename OpenPolicy, typename NextPolicy, typename ValidationPolicy>
struct BaseSearch
	{
		BaseSearch(const GraphType& graph)
			: graph{ graph }
		{}

		void Execute(Context& context)
		{
			if (!context.open[context.start])
			{
				Open(context, context.start, AI::InvalidNode);
			}

			while (!context.found && !context.openList.empty())
			{
				size_t nodeIndex = Next(context);

				if (nodeIndex == context.end)
				{
					context.found = true;
				}
				else
				{
					auto& node = graph.GetNode(nodeIndex);
					for (auto& n : node.neighbors)
					{
						if (IsValid(context, n))
						{
							Open(context, n, nodeIndex);
						}
					}
				}

				context.closedList.push_back(nodeIndex);
				context.closed[nodeIndex] = true;
			}
		}

		const GraphType& graph;
		OpenPolicy Open;
		NextPolicy Next;
		ValidationPolicy IsValid;
	};

struct PushBackPolicy
	{
		void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			context.openList.push_back(nodeIndex);
			context.open[nodeIndex] = true;
			context.parent[nodeIndex] = parentIndex;
		}
	};

struct PushFrontPolicy
	{
		void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			context.openList.push_front(nodeIndex);
			context.open[nodeIndex] = true;
			context.parent[nodeIndex] = parentIndex;
		}
	};

struct PopFrontPolicy
	{
		size_t operator()(Context& context)
		{
			auto next = context.openList.front();
			context.openList.pop_front();
			return next;
		}
	};

struct PopBackPolicy
	{
		size_t operator()(Context& context)
		{
			auto next = context.openList.back();
			context.openList.pop_back();
			return next;
		}
	};

template<typename CostPolicy>
struct InsertSortedPolicy
{
	void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			float g = Cost(context, parentIndex, nodeIndex);
			// if not open, insert sorted
			if (!context.open[nodeIndex])
			{
				auto iter = context.openList.rbegin();
				for (; iter != context.openList.rend();
					iter++)
				{
					if (context.g[*iter] > g) { break; }
				}
				context.openList.insert(iter.base(), nodeIndex);
				context.open[nodeIndex] = true;
				context.parent[nodeIndex] = parentIndex;
				context.g[nodeIndex] = g;
			}
			// if open, check the cost, then update the node(parent)
			else if (g < context.g[nodeIndex])
			{
				auto iter = context.openList.begin();
				for (; iter != context.openList.end(); iter++)
				{
					if (*iter == nodeIndex) { iter = context.openList.erase(iter); }
					if (context.g[*iter] <= g) { break; }
				}
				context.openList.insert(iter, nodeIndex);
				context.parent[nodeIndex] = parentIndex;
				context.g[nodeIndex] = g;
			}
		}

	CostPolicy Cost;
};

template<typename CostPolicy, typename HeuristicPolicy>
struct InsertSortedHeuristicPolicy
	{
		void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			float g = Cost(context, parentIndex, nodeIndex);
			float h = Heuristic(context, nodeIndex);
			if (!context.open[nodeIndex])
			{
				auto iter = context.openList.rbegin();
				for (; iter != context.openList.rend(); iter++)
				{
					if ((context.g[*iter] + context.h[*iter]) > (g + h))
						break;
				}
				context.openList.insert(iter.base(), nodeIndex);
				context.open[nodeIndex] = true;
				context.parent[nodeIndex] = parentIndex;
				context.g[nodeIndex] = g;
				context.h[nodeIndex] = h;
			}
			else if (g < context.g[nodeIndex])
			{
				auto iter = context.openList.begin();
				for (; iter != context.openList.end(); iter++)
				{
					if (*iter == nodeIndex)
						iter = context.openList.erase(iter);
					if ((context.g[*iter] + context.h[*iter]) > (g + h))
						break;
				}
				context.openList.insert(iter, nodeIndex);
				context.parent[nodeIndex] = parentIndex;
				context.g[nodeIndex] = g;
			}
		}

		CostPolicy Cost;
		HeuristicPolicy Heuristic;
	};

template<typename GraphType, typename ValidationPolicy>
using BFS = BaseSearch<GraphType, PushBackPolicy, PopFrontPolicy, ValidationPolicy>;

template<typename GraphType, typename ValidationPolicy>
using DFS = BaseSearch<GraphType, PushFrontPolicy, PopFrontPolicy, ValidationPolicy>;

template<typename GraphType, typename ValidationPolicy, typename CostPolicy>
using Dijkstra = BaseSearch<GraphType, InsertSortedPolicy<CostPolicy>, PopBackPolicy, ValidationPolicy>;

template<typename GraphType, typename ValidationPolicy, typename CostPolicy, typename HeuristicPolicy>
using AStar = BaseSearch<GraphType, InsertSortedHeuristicPolicy<CostPolicy, HeuristicPolicy>, PopBackPolicy, ValidationPolicy>;

} // namespace Coo::AI::GraphSearch

#endif // !INCLUDED_COOENGINE_AI_GRAPHSEARCH_H
