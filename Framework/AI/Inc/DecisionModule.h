#ifndef INCLUDED_COOENGINE_AI_DECISIONMODULE_H
#define INCLUDED_COOENGINE_AI_DECISIONMODULE_H

#include "Goal.h"
#include "GoalComposite.h"
#include "Strategy.h"

namespace Coo::AI 
{

template <typename AgentType>
class DecisionModule
{
public:
	typedef Goal<AgentType> GoalType;
	typedef Strategy<AgentType> StrategyType;

	DecisionModule(AgentType& agent);
	~DecisionModule();

	void AddStrategy(StrategyType* strategy);
	void Purge();

	void Update();

private:
	void Arbitrate();

	AgentType& mAgent;
	std::vector<StrategyType*> mStrategies;
	StrategyType* mCurrentStrategy;
	GoalType* mCurrentGoal;
};

template <typename AgentType>
DecisionModule<AgentType>::DecisionModule(AgentType& agent)
	: mAgent(agent)
	, mCurrentStrategy(nullptr)
	, mCurrentGoal(nullptr)
{
}

template <typename AgentType>
DecisionModule<AgentType>::~DecisionModule()
{
	Purge();
}

template <typename AgentType>
void DecisionModule<AgentType>::AddStrategy(StrategyType* strategy)
{
	mStrategies.push_back(strategy);
}

template <typename AgentType>
void DecisionModule<AgentType>::Purge()
{
	// Remove all strategies
	for (auto strategy : mStrategies)
	{
		X::SafeDelete(strategy);
	}
	mStrategies.clear();

	// Remove current goal
	if (mCurrentGoal != nullptr)
	{
		mCurrentGoal->Terminate();
		X::SafeDelete(mCurrentGoal);
	}
}

template <typename AgentType>
void DecisionModule<AgentType>::Update()
{
	// Evaluate our priorities
	Arbitrate();

	// Update the current action
	if (mCurrentGoal != nullptr)
	{
		mCurrentGoal->Process();
	}
}

template <typename AgentType>
void DecisionModule<AgentType>::Arbitrate()
{
	float mostDesirable = 0.0f;
	StrategyType* bestStrategy = nullptr;

	// Find the most desirable strategy
	for (auto strategy : mStrategies)
	{
		float desirability = strategy->CalculateDesirability();
		if (desirability > mostDesirable)
		{
			mostDesirable = desirability;
			bestStrategy = strategy;
		}
	}

	// Check if our strategy has changed
	if (mCurrentStrategy != bestStrategy)
	{
		// Set new strategy
		mCurrentStrategy = bestStrategy;

		// Exit the current goal
		if (mCurrentGoal != nullptr)
		{
			mCurrentGoal->Terminate();
			X::SafeDelete(mCurrentGoal);
		}

		// Set new goal
		if (mCurrentStrategy != nullptr)
		{
			mCurrentGoal = mCurrentStrategy->CreateGoal();
		}
	}
}

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_DECISIONMODULE_H
