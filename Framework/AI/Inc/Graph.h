#ifndef INCLUDED_COOENGINE_AI_GRAPH_H
#define INCLUDED_COOENGINE_AI_GRAPH_H

#include "Common.h"

namespace Coo::AI {

constexpr auto InvalidNode = std::numeric_limits<size_t>::max();

struct Node 
{
	std::vector<size_t> neighbors;
};

template<typename NodeType>
class Graph 
{
public:
	void Initialize(size_t nodeCount);
	
	NodeType& GetNode(size_t index);
	const NodeType& GetNode(size_t index)const;
	size_t GetSize()const { return mNodes.size(); }

private:
	std::vector<NodeType> mNodes;
};

template<typename NodeType>
inline void Graph<NodeType>::Initialize(size_t nodeCount)
{
	mNodes.clear();
	mNodes.resize(nodeCount);
}

template<typename NodeType>
inline NodeType& Graph<NodeType>::GetNode(size_t index)
{
	return mNodes[index];
}

template<typename NodeType>
inline const NodeType& Graph<NodeType>::GetNode(size_t index) const
{
	return mNodes[index];
}

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_GRAPH_H
