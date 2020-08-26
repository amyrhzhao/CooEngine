#include "Coo/Inc/Coo.h"

int main()
{
	const std::string kTarget = "I love Genetic Algorithm!";
	const int validGeneValue = 126 - 32;

	Coo::AI::GeneticAlgorithm ga;
	ga.Initialize(100, (int)kTarget.size(), validGeneValue, 0.45f, 0.1f, 
		[&kTarget](auto& genome)
		{
			genome.fitness = 0.0f;
			for (size_t i = 0; i < kTarget.size(); ++i) 
			{
				if (kTarget[i] != genome.chromosome[i] + 32) 
				{
					genome.fitness += 1.0f; // penalize any characters that dont match the target
				}
			}
		});

	auto Print = [](const Coo::AI::GeneticAlgorithm& ga)
	{
		int gen = ga.GetGeneration();
		auto& fittest = ga.BestGenome();
		std::string result;
		for (auto i : fittest.chromosome)
		{
			result += (char)(i + 32);
		}
		LOG("Generation: %d - Result: %s - Fitness: %f", gen, result.c_str(), fittest.fitness);
	};

	while (true)
	{
		ga.Advance();
		if (ga.Found()) 
		{
			break;
		}
		Print(ga);
	}
	Print(ga);

	return 0;
}