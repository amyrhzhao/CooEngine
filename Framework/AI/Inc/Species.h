#pragma once

#include "Genome.h"

namespace Coo::AI::NEAT 
{
	struct Species 
	{
		size_t topFitness = 0;
		size_t averageFitness = 0;
		size_t staleness = 0;

		std::vector<Genome> genomes;
	};
}