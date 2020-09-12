#include <Coo/Inc/Coo.h>
#include <iostream>
#include <cstdio>
size_t XORTest(Coo::AI::NEAT::NeuralNet& n, bool log)
{
	using namespace Coo::AI::NEAT;
	size_t fitness = 0;

	std::string message;
	if (log)
		message = "	> begin xor test\n		>";

	auto output = n.Evaluate({ 0.0, 0.0 });
	if (log)
		message += std::to_string(output[0]) + " ";
	fitness += static_cast<int>(std::min(1.0 / ((0.0 - output[0]) * (0.0f - output[0])), 50.0));

	output = n.Evaluate({ 0.0, 1.0 });
	if (log)
		message += std::to_string(output[0]) + " ";
	fitness += static_cast<int>(std::min(1.0 / ((1.0 - output[0]) * (1.0f - output[0])), 50.0));

	output = n.Evaluate({ 1.0, 0.0 });
	if (log)
		message += std::to_string(output[0]) + " ";
	fitness += static_cast<int>(std::min(1.0 / ((1.0 - output[0]) * (1.0f - output[0])), 50.0));

	output = n.Evaluate({ 1.0, 1.0 });
	if (log)
		message += std::to_string(output[0]) + " ";
	fitness += static_cast<int>(std::min(1.0 / ((0.0 - output[0]) * (0.0f - output[0])), 50.0));

	if (log)
	{
		message += ") fitness" + std::to_string(fitness) + "\n";

		std::cout << message;
	}

	return fitness;
}

void Test()
{
	using namespace Coo::AI::NEAT;
	Population p(2, 1, 0);
	NeuralNet bestGuy;
	MutationConfig mutationConfig;
	mutationConfig.connection_mutate_chance = 0.65;
	mutationConfig.perturb_chance = 0.90;
	mutationConfig.crossover_chance = 0.75;
	mutationConfig.link_mutation_chance = 0.85;
	mutationConfig.node_mutation_chance = 0.50;
	mutationConfig.bias_mutation_chance = 0.20;
	mutationConfig.step_size = 0.1;
	mutationConfig.disable_mutation_chance = 0.2;
	mutationConfig.enable_mutation_chance = 0.2;
	p.mutationConfig = mutationConfig;

	p.speciatingConfig.population = 300;
	size_t max_fitness = 0;
	while (max_fitness < 200)
	{
		size_t current_fitness = 0;
		size_t min_fitness = 100000;
		for (auto& s : p.species)
		{
			for (auto& g : s.genomes)
			{
				NeuralNet n;
				n.Initialize(g, p.neuralNetConfig);
				current_fitness = XORTest(n, false);
				if (current_fitness < min_fitness)
					min_fitness = current_fitness;
				if (current_fitness > max_fitness)
				{
					max_fitness = current_fitness;
					bestGuy = n;
				}
				g.fitness = current_fitness;
			}
		}
		printf("Generation %d successfully tested. Species:%d, Global min: %d, Global max: %d\n", p.Generation(), p.species.size(), min_fitness, max_fitness);
		p.NewGeneration();
	}

}
int main()
{
	Test();
	return 0;
}

