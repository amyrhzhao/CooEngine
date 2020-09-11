#pragma once

#include "Common.h"
#include "InnovationContainer.h"
#include "NeatConfig.h"

namespace Coo::AI::NEAT
{
	struct Genome;
	struct Species;

	class Population 
	{
	public:
		MutationConfig mutationConfig;
		SpeciatingConfig speciatingConfig;
		NeuralNetConfig neuralNetConfig;

	private:
		// Evolutionary methods
		Genome Crossover(const Genome& g1, const Genome* g2);
		void MutateWeight(Genome& g);
		void MutateEnableDisable(Genome& g, bool enable);
		void MutateLink(Genome& g, bool force_bias);
		void MutateNode(Genome& g);
		void Mutate(Genome& g);

		double Disjoint(const Genome& g1, const Genome& g2);
		double Weights(const Genome& g1, const Genome& g2);
		bool IsSameSpecies(const Genome& g1, const Genome& g2);

		// Species ranking
		void RankGlobally();
		void CalculateAverageFitness(Species& s);
		size_t TotalAverageFitness();

		// Evolution
		void CullSpecies(bool cut_to_one);
		Genome BreedChild(Species& s);
		void RemoveStaleSpecies();
		void RemoveWeakSpecies();
		void AddToSpecies(Genome& child);

		InnovationContainer mInnovation;
		size_t mGenerationNumber;
	};
}