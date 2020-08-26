#pragma once

namespace Coo::AI
{
	class GeneticAlgorithm
	{
	public:
		struct Genome
		{
			std::vector<int> chromosome;
			float fitness = 0.0f;
		};

		//using Crossover = std::function<Genome(const Genome&, const Genome&)>;
		using ComputeFitness = std::function<void(Genome&)>;
		
		// Randomly generate the initial population
		void Initialize(int populationSize, int chromoLength, int maxGeneValue, 
			float crossoverRate, float mutationRate, ComputeFitness computeFitness);

		// Apply crossover and mutation to produce the next generation
		void Advance();

		// Accessors
		const Genome& BestGenome() const { return mPopulation.front(); }
		int GetGeneration() const { return mGeneration; }
		bool Found() const { return mFound; }

	private:
		Genome Mate(const Genome& parent1, const Genome& parent2);
		void SortPopulation();

		ComputeFitness mComputeFitness;

		std::vector<Genome> mPopulation;

		int mGeneration = 0;
		int mMaxGeneValue = 0;

		float mCrossoverRate = 0.0f;
		float mMutationRate = 0.0f;

		bool mFound = false;
	};
}