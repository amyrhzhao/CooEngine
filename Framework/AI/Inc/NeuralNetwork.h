#pragma once

#include "Common.h"

namespace Coo::AI
{
	class Neuron;
	using Layer = std::vector<Neuron>;

	struct Connection
	{
		double weight = 0;
		double deltaWeight = 0;
	};

	class Neuron
	{
	public:
		Neuron(size_t numOutputs, size_t myIndex);

		void SetOutputValue(double value) { mOutputValue = value; }
		double GetOutputValue() const { return mOutputValue; }

		void FeedForward(const Layer& previousLayer);
		void CalculateOutputGradients(double targetValue);
		void CalculateHiddenGradients(const Layer& nextLayer);
		void UpdateInputWeights(Layer& previousLayer);

	private:
		std::vector<Connection> mOutputWeights;
		size_t mMyIndex = 0;
		double mOutputValue = 0;
		double mGradient = 0;
	};

	class NeuralNetwork
	{
	public:
		NeuralNetwork(const std::vector<size_t>& topology);

		void FeedFoward(const std::vector<double>& inputValues);
		void BackPropagate(const std::vector<double>& targetValues);
		std::vector<double> GetResults() const;

	private:
		std::vector<Layer> mLayers;
	};
}