#pragma once

#include "Common.h"

namespace Coo::AI::NEAT 
{
	struct Gene;

	class InnovationContainer 
	{
	public:
		void Reset();

		size_t AddGene(const Gene& gene);
		size_t Number() const { return mInnovationNumber; }

	private:
		friend class Population;
		
		void SetInnovationNumber(size_t num) 
		{
			mInnovationNumber = num;
			Reset();
		}

		std::map<std::pair<size_t, size_t>, size_t> mHistory;
		size_t mInnovationNumber = 0;
	};
}