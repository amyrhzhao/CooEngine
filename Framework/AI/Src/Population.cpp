#include "Precompiled.h"
#include "Population.h"
#include "Genome.h"
#include "Species.h"

using namespace Coo::AI::NEAT;

Coo::AI::NEAT::Population::Population(size_t input, size_t output, size_t bias)
{
	neuralNetConfig.input_size = input;
	neuralNetConfig.output_size = output;
	neuralNetConfig.bias_size = bias;
	neuralNetConfig.functional_nodes = input + output + bias;

	// Create a basic generation with default genomes
	for (size_t i = 0; i < speciatingConfig.population; ++i)
	{
		Genome newG = MakeFirstGenome();
		//Genome newGenome(neuralNetConfig.functional_nodes);
		//Mutate(newGenome);
		AddToSpecies(newG);
	}
}

void Coo::AI::NEAT::Population::NewGeneration()
{
	//mInnovation.Reset();
	CullSpecies(false);
	RankGlobally();
	RemoveStaleSpecies();

	for (auto& s : species)
	{
		CalculateAverageFitness(s);
	}
	RemoveWeakSpecies();

	std::vector<Genome> children;
	size_t sum = TotalAverageFitness();
	for (auto& s : species)
	{
		size_t breed =
			static_cast<size_t>(std::floor(((1. * s.averageFitness) / (1. * sum)) * 1. * speciatingConfig.population) - 1);
		//const size_t breed = static_cast<size_t>(std::floor(1.0 * s.averageFitness / sum * speciatingConfig.population) - 1);
		for (size_t i = 0; i < breed; ++i)
		{
			children.push_back(BreedChild(s));
		}
	}

	CullSpecies(true); // Now in each species we have only one genome

	// Preparing for making babies <3
	std::vector<Species*> species_pointer(0);
	for (auto& s : species)
	{
		species_pointer.push_back(&s);
	}

	if (!species.empty()) 
	{
		while ( (children.size() + species.size()) < speciatingConfig.population)
		{
			const size_t speciesIndex = Coo::Math::Random(0, static_cast<int>(species.size() - 1));
			children.push_back(BreedChild(*species_pointer[speciesIndex]));
		}
	}

	for (auto& genome : children)
		AddToSpecies(genome);
	++mGenerationNumber;
}

Genome Coo::AI::NEAT::Population::MakeFirstGenome()
{
	Genome genome(neuralNetConfig.functional_nodes);

	for (size_t i = 0; i < neuralNetConfig.input_size; ++i)
	{
		for (size_t o = 0; o < neuralNetConfig.output_size; ++o)
		{
			Gene gene;
			gene.fromNode = i;
			gene.toNode = neuralNetConfig.input_size + neuralNetConfig.bias_size + o;
			gene.innovationNum = mInnovation.AddGene(gene);
			gene.weight = Coo::Math::RandomReal(-2.0, 2.0);
			genome.genes[gene.innovationNum] = gene;
		}
	}

	for (size_t b = 0; b < neuralNetConfig.bias_size; ++b)
	{
		for (size_t o = 0; o < neuralNetConfig.output_size; ++o)
		{
			Gene gene;
			gene.fromNode = neuralNetConfig.input_size + b;
			gene.toNode = neuralNetConfig.input_size + neuralNetConfig.bias_size + o;
			gene.innovationNum = mInnovation.AddGene(gene);
			gene.weight = Coo::Math::RandomReal(-2.0, 2.0);
			genome.genes[gene.innovationNum] = gene;
		}
	}

	return genome;
}

Genome Coo::AI::NEAT::Population::Crossover(const Genome& g1, const Genome& g2)
{
	// Make sure g1 has the higher fitness
	if (g2.fitness > g1.fitness)
		return Crossover(g2, g1);

	Genome child(std::max(g1.maxNeuron, g2.maxNeuron));

	// coin flip random number distribution
	for (auto& [innovNum, gene] : g1.genes)
	{
		if (Coo::Math::Random(0, 1) == 0)
		{
			const auto it2 = g2.genes.find(innovNum);
			if (it2 != g2.genes.end())
			{
				child.genes[innovNum] = it2->second;
				continue;
			}
		}
		child.genes[innovNum] = gene;
	}
	return child;
}

void Coo::AI::NEAT::Population::MutateWeight(Genome& g)
{
	double step = mutationConfig.step_size;
	for (auto& [innovNum, gene] : g.genes)
	{
		if (Coo::Math::RandomReal() < mutationConfig.perturb_chance)
		{
			gene.weight += Coo::Math::RandomReal(-step, step);
		}
		else
		{
			gene.weight = Coo::Math::RandomReal(-2.0, 2.0);
		}
	}
}

void Coo::AI::NEAT::Population::MutateEnableDisable(Genome& g, bool enable)
{
	std::vector<Gene*> v;

	// Find all nodes that are enabled
	for (auto& [innovNum, gene] : g.genes)
	{
		if (gene.enabled != enable)
		{
			v.push_back(&gene);
		}
	}

	// Ramdomly pick one of them and set enable flag
	if (!v.empty())
	{
		const size_t index = Coo::Math::Random(0, v.size() - 1);
		v[index]->enabled = enable;
	}
}

void Coo::AI::NEAT::Population::MutateLink(Genome& g, bool force_bias)
{
	/* network encoding:
	 * | input nodes | bias | output nodes |
	 */

	auto is_input = [&](unsigned int node) -> bool
	{
		return node < neuralNetConfig.input_size;
	};
	auto is_output = [&](unsigned int node) -> bool
	{
		return node < neuralNetConfig.functional_nodes&& node >=
			(neuralNetConfig.input_size + neuralNetConfig.bias_size);
	};
	auto is_bias = [&](unsigned int node) -> bool
	{
		return node < (neuralNetConfig.input_size + neuralNetConfig.bias_size)
			&& node >= neuralNetConfig.input_size;
	};

	size_t neuron1 = Coo::Math::Random(0, g.maxNeuron - 1);
	size_t neuron2 = Coo::Math::Random(
		neuralNetConfig.input_size + neuralNetConfig.bias_size,
		g.maxNeuron - 1);

	if (is_output(neuron1) && is_output(neuron2))
		return;
	if (is_bias(neuron2))
		return;
	if (neuron1 == neuron2 && (!force_bias))
		return;
	if (is_output(neuron1))
		std::swap(neuron1, neuron2);

	if (force_bias) {
		auto i1 = neuralNetConfig.input_size;
		auto i2 = neuralNetConfig.input_size + neuralNetConfig.bias_size - 1;
		neuron1 = Coo::Math::Random(
			std::min(i1, i2),
			std::max(i1, i2));
	}

	// Check for recurrency using BFS
	{
		bool has_recurrence = false;
		if (is_bias(neuron1) || is_input(neuron1))
		{
			has_recurrence = false;
		}
		else
		{
			std::queue<size_t> que;
			std::vector<std::vector<size_t>> connections(g.maxNeuron);

			for (auto& [innovNum, gene] : g.genes)
			{
				connections[gene.fromNode].push_back(gene.toNode);
			}
			connections[neuron1].push_back(neuron2);

			for (size_t i = 0; i < connections[neuron1].size(); ++i)
				que.push(connections[neuron1][i]);

			while (!que.empty()) {
				size_t tmp = que.front();
				if (tmp == neuron1)
				{
					has_recurrence = true;
					break;
				}
				que.pop();
				for (size_t i = 0; i < connections[tmp].size(); ++i)
					que.push(connections[tmp][i]);
			}
		}
		if (has_recurrence)
			return;
	}

	// If genome already has this connection
	for (const auto& [innovNum, gene] : g.genes)
	{
		if (gene.fromNode == neuron1 && gene.toNode == neuron2)
		{
			return;
		}
	}

	// Add new innovation if needed
	Gene newGene;
	newGene.fromNode = neuron1;
	newGene.toNode = neuron2;
	newGene.innovationNum = mInnovation.AddGene(newGene);
	newGene.weight = Coo::Math::RandomReal(-2.0, 2.0);
	g.genes[newGene.innovationNum] = newGene;
}

void Coo::AI::NEAT::Population::MutateNode(Genome& g)
{
	if (g.genes.empty())
		return;

	g.maxNeuron++;

	// Randomly choose a gene to mutate
	const size_t gene_id = Coo::Math::Random(0, g.genes.size() - 1);
	auto it = g.genes.begin();
	std::advance(it, gene_id);

	auto& gene = it->second;
	if (gene.enabled == false)
		return;

	gene.enabled = false;

	Gene new_gene1;
	new_gene1.fromNode = gene.fromNode;
	new_gene1.toNode = g.maxNeuron - 1; // to the last created neuron
	new_gene1.weight = 1.0;
	new_gene1.innovationNum = mInnovation.AddGene(new_gene1);
	new_gene1.enabled = true;

	Gene new_gene2;
	new_gene2.fromNode = g.maxNeuron - 1; // from the last created neuron
	new_gene2.toNode = gene.toNode;
	new_gene2.weight = gene.weight;
	new_gene2.innovationNum = mInnovation.AddGene(new_gene2);
	new_gene2.enabled = true;

	g.genes[new_gene1.innovationNum] = new_gene1;
	g.genes[new_gene2.innovationNum] = new_gene2;
}

void Coo::AI::NEAT::Population::Mutate(Genome& g)
{
	if (Coo::Math::RandomReal(0.0, 1.0) < mutationConfig.connection_mutate_chance)
		MutateWeight(g);

	if (Coo::Math::RandomReal(0.0, 1.0) < mutationConfig.link_mutation_chance)
		MutateLink(g, false);

	if (Coo::Math::RandomReal(0.0, 1.0) < mutationConfig.bias_mutation_chance)
		MutateLink(g, true);

	if (Coo::Math::RandomReal(0.0, 1.0) < mutationConfig.node_mutation_chance)
		MutateNode(g);

	if (Coo::Math::RandomReal(0.0, 1.0) < mutationConfig.enable_mutation_chance)
		MutateEnableDisable(g, true);

	if (Coo::Math::RandomReal(0.0, 1.0) < mutationConfig.disable_mutation_chance)
		MutateEnableDisable(g, false);

}

double Coo::AI::NEAT::Population::Disjoint(const Genome& g1, const Genome& g2)
{
	size_t disjoint_count = 0;
	for (const auto& [innovNum, gene] : g1.genes)
	{
		if (g2.genes.find(innovNum) == g2.genes.end())
		{
			++disjoint_count;
		}
	}

	for (const auto& [innovNum, gene] : g2.genes)
	{
		if (g1.genes.find(innovNum) == g1.genes.end())
		{
			++disjoint_count;
		}
	}

	return static_cast<double>(disjoint_count) / std::max(g1.genes.size(), g2.genes.size());
}

double Coo::AI::NEAT::Population::Weights(const Genome& g1, const Genome& g2)
{

	double sum = 0.0;
	size_t numMatch = 0;

	for (auto& [innovNum, gene] : g1.genes)
	{
		auto it2 = g2.genes.find(innovNum);
		if (it2 != g2.genes.end())
		{
			sum += std::abs(gene.weight - it2->second.weight);
			++numMatch;
		}
	}

	return sum / numMatch;
}

bool Coo::AI::NEAT::Population::IsSameSpecies(const Genome& g1, const Genome& g2)
{
	double dd = speciatingConfig.delta_disjoint * Disjoint(g1, g2);
	double dw = speciatingConfig.delta_weights * Weights(g1, g2);
	return dd + dw < speciatingConfig.delta_threshold;
}

void Coo::AI::NEAT::Population::RankGlobally()
{
	std::vector<Genome*> global;
	for (auto& s : species)
		for (auto& g : s.genomes)
			global.push_back(&g);

	std::sort(global.begin(), global.end(), [](auto a, auto b) {
		return a->fitness < b->fitness;
		});

	for (size_t j = 0; j < global.size(); ++j)
	{
		global[j]->globalRank = j + 1;
	}
}

void Coo::AI::NEAT::Population::CalculateAverageFitness(Species& s)
{
	size_t total = 0;
	for (auto& g : s.genomes)
		total += g.globalRank;
	s.averageFitness = total / s.genomes.size();
}

size_t Coo::AI::NEAT::Population::TotalAverageFitness() const
{
	size_t total = 0;
	for (const auto& s : species)
		total += s.averageFitness;
	return total;
}

void Coo::AI::NEAT::Population::CullSpecies(bool cut_to_one)
{
	for (auto& s : species)
	{
		std::sort(s.genomes.begin(), s.genomes.end(),
			[](auto& a, auto& b) { return a.fitness > b.fitness; });

		const size_t numToKeep = cut_to_one ? 1 :
			static_cast<size_t>(std::ceil(s.genomes.size() / 2.0));
		while (s.genomes.size() > numToKeep)
			s.genomes.pop_back();
	}
}

Genome Coo::AI::NEAT::Population::BreedChild(const Species& s)
{
	Genome child(0);
	const size_t parent1 = Coo::Math::Random(0, static_cast<int>(s.genomes.size() - 1));
	if (Coo::Math::RandomReal() < mutationConfig.crossover_chance)
	{
		const size_t parent2 = Coo::Math::Random(0, static_cast<int>(s.genomes.size() - 1));
		const Genome& g1 = s.genomes[parent1];
		const Genome& g2 = s.genomes[parent2];
		child = Crossover(g1, g2);
	}
	else
	{
		child = s.genomes[parent1];
	}

	Mutate(child);
	return child;
}

void Coo::AI::NEAT::Population::RemoveStaleSpecies()
{
	// Update top fitness of each species
	for (auto& s : species)
	{
		auto g = std::max_element(s.genomes.begin(), s.genomes.end(), [](auto& a, auto b)
			{
				return a.fitness < b.fitness;
			});

		if (g->fitness > s.topFitness)
		{
			s.topFitness = g->fitness;
			s.staleness = 0;
		}
		else
		{
			s.staleness++;
		}
	}

	if (species.size() == 1) 
	{
		return;
	}

	// Prune any species that have not improve for some time
	auto backup = species;
	backup.remove_if([this](auto& s)
		{
			return s.staleness >= speciatingConfig.stale_species;
		});
	species = backup;
}

void Coo::AI::NEAT::Population::RemoveWeakSpecies()
{
	size_t sum = TotalAverageFitness();
	auto backup = species;
	backup.remove_if([this, sum](auto& s)
		{
			return 1.0 > std::floor(1.0 * s.averageFitness / (1.0 * sum) * 1.0 * speciatingConfig.population);
		});
	ASSERT(!backup.empty(), "Ooops");
	species = backup;
}

void Coo::AI::NEAT::Population::AddToSpecies(Genome& child)
{
	for (auto& s : species)
	{
		if (IsSameSpecies(child, s.genomes[0]))
		{
			s.genomes.push_back(child);
			return;
		}
	}

	Species newSpecies;
	newSpecies.genomes.push_back(child);
	species.push_back(newSpecies);
}
