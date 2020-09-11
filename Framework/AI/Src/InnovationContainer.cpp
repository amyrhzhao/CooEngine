#include "Precompiled.h"
#include "InnovationContainer.h"
#include "Genome.h"

using namespace Coo::AI::NEAT;

void Coo::AI::NEAT::InnovationContainer::Reset()
{
	mHistory.clear();
}

size_t Coo::AI::NEAT::InnovationContainer::AddGene(const Gene& gene)
{
	auto pair = std::make_pair(gene.fromNode, gene.toNode);
	auto it = mHistory.find(pair);
	if (it == mHistory.end())
		return mHistory[pair] = ++mInnovationNumber;
	else
		return (*it).second;
}
