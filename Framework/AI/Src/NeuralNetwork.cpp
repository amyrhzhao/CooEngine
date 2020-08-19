#include "Precompiled.h"
#include "NeuralNetwork.h"

using namespace Coo::AI;

namespace
{
	double TransferFunction(double x)
	{
		return tanh(x);
	}

	double TransferFunctionDerivative(double x)
	{
		return 1 - (x * x);
	}
}

Neuron::Neuron(size_t numOutputs, size_t myIndex)
	: mMyIndex(myIndex)
{
	for (size_t i = 0; i < numOutputs; ++i)
		mOutputWeights.push_back({ Coo::Math::RandomReal(), 0 });
}

void Neuron::FeedForward(const Layer& previousLayer)
{
	double sum = 0;

	for (size_t neuron = 0; neuron < previousLayer.size(); ++neuron)
	{
		auto& n = previousLayer[neuron];
		sum += n.GetOutputValue() * n.mOutputWeights[mMyIndex].weight;
	}

	mOutputValue = TransferFunction(sum);
}

void Neuron::CalculateOutputGradients(double targetValue)
{
	double delta = targetValue - mOutputValue;
	mGradient = delta * TransferFunctionDerivative(mOutputValue);
}

void Neuron::CalculateHiddenGradients(const Layer& nextLayer)
{
	double sumDOW = 0;
	for (size_t neuron = 0; neuron + 1 < nextLayer.size(); ++neuron)
		sumDOW += mOutputWeights[neuron].weight * nextLayer[neuron].mGradient;
	mGradient = sumDOW * TransferFunctionDerivative(mOutputValue);
}

void Neuron::UpdateInputWeights(Layer& previousLayer)
{
	const double learningRate = 0.15;

	for (size_t neuron = 0; neuron < previousLayer.size(); ++neuron)
	{
		auto& n = previousLayer[neuron];
		double deltaWeight = learningRate * n.GetOutputValue() * mGradient;
		n.mOutputWeights[mMyIndex].weight += deltaWeight;
	}
}

NeuralNetwork::NeuralNetwork(const std::vector<size_t>& topology)
{
	const size_t numLayers = topology.size();
	assert(numLayers >= 2);

	mLayers.resize(numLayers);
	for (size_t layer = 0; layer < numLayers; ++layer)
	{
		const size_t numOutputs = (layer + 1 == numLayers) ? 0 : topology[layer + 1];
		const size_t numNeurons = topology[layer];

		// +1 for the bias neuron
		mLayers[layer].reserve(numNeurons + 1);
		for (size_t neuron = 0; neuron <= numNeurons; ++neuron)
			mLayers[layer].emplace_back(numOutputs, neuron);
	}
}

void NeuralNetwork::FeedFoward(const std::vector<double>& inputValues)
{
	assert(inputValues.size() + 1 == mLayers.front().size());

	// Assign the input values into the input neurons
	for (size_t i = 0; i < inputValues.size(); ++i)
		mLayers.front()[i].SetOutputValue(inputValues[i]);

	// Forward propagate
	for (size_t layer = 1; layer < mLayers.size(); ++layer)
	{
		auto& currentLayer = mLayers[layer];
		auto& previousLayer = mLayers[layer - 1];

		for (size_t neuron = 0; neuron + 1 < currentLayer.size(); ++neuron)
			currentLayer[neuron].FeedForward(previousLayer);
	}
}

void NeuralNetwork::BackPropagate(const std::vector<double>& targetValues)
{
	assert(targetValues.size() + 1 == mLayers.back().size());

	Layer& outputLayer = mLayers.back();

	// Calculate output layer gradient
	for (size_t neuron = 0; neuron + 1 < outputLayer.size(); ++neuron)
		outputLayer[neuron].CalculateOutputGradients(targetValues[neuron]);

	// Calculate gradients on hidden layers
	for (size_t layer = mLayers.size() - 2; layer > 0; --layer)
	{
		Layer& hiddenLayer = mLayers[layer];
		Layer& nextLayer = mLayers[layer + 1];

		for (size_t neuron = 0; neuron < hiddenLayer.size(); ++neuron)
			hiddenLayer[neuron].CalculateHiddenGradients(nextLayer);
	}

	// Update connection weights
	for (size_t layer = mLayers.size() - 1; layer > 0; --layer)
	{
		Layer& currentLayer = mLayers[layer];
		Layer& previousLayer = mLayers[layer - 1];
		for (size_t neuron = 0; neuron + 1 < currentLayer.size(); ++neuron)
			currentLayer[neuron].UpdateInputWeights(previousLayer);
	}
}

std::vector<double> NeuralNetwork::GetResults() const
{
	std::vector<double> results;
	const Layer& outputLayer = mLayers.back();
	for (size_t neuron = 0; neuron + 1 < outputLayer.size(); ++neuron)
		results.push_back(outputLayer[neuron].GetOutputValue());
	return results;
}