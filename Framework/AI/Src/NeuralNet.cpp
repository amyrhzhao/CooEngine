#include "Precompiled.h"
#include "NeuralNet.h"
#include "Genome.h"

using namespace Coo::AI::NEAT;

namespace
{
	double Sigmoid(double x)
	{
		return 2.0 / (1.0 + std::exp(-4.9 * x));
	}
}

void Coo::AI::NEAT::NeuralNet::Initialize(const Genome& genome, const NeuralNetConfig& netConfig)
{
	mNodes.clear();
	mInputNodes.clear();
	mBiasNodes.clear();
	mOutputNodes.clear();

	// Add all node types
	for (size_t i = 0; i < netConfig.input_size; ++i)
	{
		mNodes.emplace_back().type = Neuron::Type::Input;
		mInputNodes.push_back(mNodes.size() - 1);
	}
	for (size_t i = 0; i < netConfig.bias_size; ++i)
	{
		mNodes.emplace_back().type = Neuron::Type::Bias;
		mBiasNodes.push_back(mNodes.size() - 1);
	}
	for (size_t i = 0; i < netConfig.output_size; ++i)
	{
		mNodes.emplace_back().type = Neuron::Type::Output;
		mOutputNodes.push_back(mNodes.size() - 1);
	}

	std::map<size_t, size_t> nodeIndexLookup;
	for (size_t i = 0; i < mInputNodes.size() + mOutputNodes.size() + mBiasNodes.size(); ++i)
		nodeIndexLookup[i] = i;

	// Go through all the connections and add any hidden nodes
	for (auto& [innov, gene] : genome.genes)
	{
		if (!gene.enabled)
			continue;

		if (nodeIndexLookup.find(gene.fromNode) == nodeIndexLookup.end())
		{
			nodeIndexLookup[gene.fromNode] = mNodes.size();
			mNodes.emplace_back().type = Neuron::Type::Hidden;
		}
		if (nodeIndexLookup.find(gene.toNode) == nodeIndexLookup.end())
		{
			nodeIndexLookup[gene.toNode] = mNodes.size();
			mNodes.emplace_back().type = Neuron::Type::Hidden;
		}

	}

	// Connect nodes
	for (auto& [innov, gene] : genome.genes)
	{
		auto toNodeIndex = nodeIndexLookup[gene.toNode];
		auto fromNodeIndex = nodeIndexLookup[gene.fromNode];
		mNodes[toNodeIndex].inNodes.push_back(
			std::make_pair(fromNodeIndex, gene.weight));
	}
}

std::vector<double> Coo::AI::NEAT::NeuralNet::Evaluate(const std::vector<double>& input) const
{
	// Reset all nodes
	for (auto& node : mNodes) 
	{
		node.value = 0.0;
		node.visited = false;
	}

	// Assign values to the input nodes
	ASSERT(input.size() == mInputNodes.size(), "[NeuralNet] Input count mismatch.");
	for (size_t i = 0; i < input.size(); ++i) 
	{
		mNodes[mInputNodes[i]].value = input[i];
		mNodes[mInputNodes[i]].visited = true;
	}

	// Assign 1 to all bias nodes
	for (size_t i = 0; i < mBiasNodes.size(); i++) 
	{
		mNodes[mBiasNodes[i]].value = 1.0;
		mNodes[mBiasNodes[i]].visited = true;
	}

	// Start evaluating from the output nodes
	std::stack<size_t> s;
	for (auto outputNodeIndex : mOutputNodes)
		s.push(outputNodeIndex);

	// Continue until all nodes are evaluated
	while (!s.empty()) {
		size_t t = s.top();

		if (mNodes[t].visited) {
			double sum = 0.0;
			for (auto& [index, weight] : mNodes[t].inNodes) 
				sum += mNodes[index].value * weight;
			mNodes[t].value = Sigmoid(sum);
			s.pop();
		}

		else {
			mNodes[t].visited = true;

			// Add unvisited parents
			for (auto& [index, weight] : mNodes[t].inNodes) 
			{
				if (!mNodes[index].visited)
					s.push(index);
			}
		}
	}

	// Extract results from output nodes
	std::vector<double> outputs;
	for (auto outputNodeIndex : mOutputNodes)
		outputs.push_back(mNodes[outputNodeIndex].value);

	return outputs;
}
