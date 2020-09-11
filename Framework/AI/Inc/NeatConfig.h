#pragma once

namespace Coo::AI::NEAT
{
	struct MutationConfig
	{
		double connection_mutate_chance = 0.25;
		double perturb_chance = 0.90;
		double crossover_chance = 0.75;
		double link_mutation_chance = 2.0;
		double node_mutation_chance = 0.50;
		double bias_mutation_chance = 0.40;
		double step_size = 0.1;
		double disable_mutation_chance = 0.4;
		double enable_mutation_chance = 0.2;
	};

	struct SpeciatingConfig
	{
		size_t population = 240;
		double delta_disjoint = 2.0;
		double delta_weights = 0.4;
		double delta_threshold = 1.3;
		size_t stale_species = 15;
	};

	struct NeuralNetConfig
	{
		size_t input_size;
		size_t bias_size;
		size_t output_size;
		size_t functional_nodes;
	};
}