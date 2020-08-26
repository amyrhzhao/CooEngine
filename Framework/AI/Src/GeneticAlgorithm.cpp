#include "Precompiled.h"
#include "GeneticAlgorithm.h"

using Genome = Coo::AI::GeneticAlgorithm::Genome;

void Coo::AI::GeneticAlgorithm::Initialize(int populationSize, int chromoLength, int maxGeneValue, float crossoverRate, float mutationRate, ComputeFitness computeFitness)
{
	mComputeFitness = std::move(computeFitness);

	// Reset
	mGeneration = 0;
	mMaxGeneValue = maxGeneValue;
	mCrossoverRate = crossoverRate;
	mMutationRate = mutationRate;
	mFound = false;

	// Create initial population
	mPopulation.clear();
	mPopulation.resize(populationSize);
	for (auto& genome : mPopulation) 
	{
		genome.chromosome.reserve(chromoLength);
		for (int i = 0; i < chromoLength; ++i) 
		{
			genome.chromosome.push_back(Coo::Math::Random(0, maxGeneValue));
		}
	}

	SortPopulation();
}

void Coo::AI::GeneticAlgorithm::Advance()
{
	if (mFound)
		return;

	++mGeneration;

	std::vector<Genome> newGeneration;
	newGeneration.reserve(mPopulation.size());

	// Perform Elitism: keep top 10%, mate the rest using top 50%
	const int eliteCount = ((int)mPopulation.size() * 10) / 100;
	for (int i = 0; i < eliteCount; ++i) 
	{
		newGeneration.push_back(mPopulation[i]);
	}

	const int mateCount = (int)mPopulation.size() - eliteCount;
	const int cutoff = (int)mPopulation.size() / 2;
	for (int i = 0; i < mateCount; ++i) 
	{
		const auto& parent1 = mPopulation[Coo::Math::Random(0, cutoff)];
		const auto& parent2 = mPopulation[Coo::Math::Random(0, cutoff)];
		newGeneration.push_back(Mate(parent1, parent2));
	}

	mPopulation = std::move(newGeneration);
	SortPopulation();

	if (mPopulation[0].fitness == 0.0f)
	{
		mFound = true;
	}
}

Genome Coo::AI::GeneticAlgorithm::Mate(const Genome& parent1, const Genome& parent2)
{
	Genome offspring;
	offspring.chromosome.reserve(parent1.chromosome.size());
	for (size_t i = 0; i < parent1.chromosome.size(); ++i) 
	{
		if (Coo::Math::RandomFloat(0.0f, 1.0f) < mMutationRate) 
		{
			offspring.chromosome.push_back(Coo::Math::Random(0, mMaxGeneValue));
		}
		else if (Coo::Math::RandomFloat(0.0f, 1.0f) < mCrossoverRate) 
		{
			offspring.chromosome.push_back(parent1.chromosome[i]);
		}
		else
		{
			offspring.chromosome.push_back(parent2.chromosome[i]);
		}			
	}

	return offspring;
}

void Coo::AI::GeneticAlgorithm::SortPopulation()
{
	for (auto& genome : mPopulation) 
	{
		mComputeFitness(genome);
	}
	std::sort(mPopulation.begin(), mPopulation.end(),
		[](const auto& a, const auto& b)
		{
			return a.fitness < b.fitness;
		});
}
