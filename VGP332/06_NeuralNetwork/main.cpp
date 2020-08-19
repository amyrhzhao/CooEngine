#include <Coo/Inc/Coo.h>
#include <iostream>

using namespace Coo;

void ShowVectorVals(const char* label, const std::vector<double>& vals)
{
	std::cout << label << " ";
	for (uint32_t i = 0; i < vals.size() - 1; ++i)
	{
		std::cout << vals[i] << ", ";
	}
	std::cout << vals.back() << std::endl;
}

int main(int argc, char* argv[])
{
	const std::vector<std::vector<double>> xorInputValues =
	{
		{ 0, 0 },
		{ 0, 1 },
		{ 1, 0 },
		{ 1, 1 }
	};

	const std::vector<std::vector<double>> xorOutputValues =
	{
		{ 0 },
		{ 1 },
		{ 1 },
		{ 0 }
	};

	AI::NeuralNetwork ann({ 2u, 2u, 1u });

	int example = 0;
	for (size_t i = 0; i < 10000; ++i)
	{
		ann.FeedFoward(xorInputValues[example]);
		ShowVectorVals("Inputs: ", xorInputValues[example]);

		ann.BackPropagate(xorOutputValues[example]);
		ShowVectorVals("Outputs: ", ann.GetResults());
		ShowVectorVals("Targets:", xorOutputValues[example]);
		std::cout << "\n";

		example = Coo::Math::Random(0, 3);
	}

	return 0;
}