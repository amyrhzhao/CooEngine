#ifndef INCLUDED_COOENGINE_AI_GOALCOMPOSITE_H
#define INCLUDED_COOENGINE_AI_GOALCOMPOSITE_H

#include "Goal.h"

namespace Coo::AI 
{

template <typename AgentType>
class GoalComposite : public Goal<AgentType>
{
public:
	using Status = Goal<AgentType>::Status;

	GoalComposite(AgentType& agent);
	virtual ~GoalComposite();

protected:
	void AddSubgoal(Goal<AgentType>* goal);
	void RemoveAllSubgoals();
	Status ProcessSubgoals();

	std::list<Goal<AgentType>*> mSubgoals;
};

template <typename AgentType>
GoalComposite<AgentType>::GoalComposite(AgentType& agent)
	: Goal<AgentType>(agent)
{
}

template <typename AgentType>
GoalComposite<AgentType>::~GoalComposite()
{
	RemoveAllSubgoals();
}

template <typename AgentType>
void GoalComposite<AgentType>::AddSubgoal(Goal<AgentType>* goal)
{
	mSubgoals.push_front(goal);
}

template <typename AgentType>
void GoalComposite<AgentType>::RemoveAllSubgoals()
{
	for (auto goal : mSubgoals)
	{
		if (goal != nullptr)
		{
			goal->Terminate();
			SafeDelete(goal);
		}
	}
	mSubgoals.clear();
}

template <typename AgentType>
typename GoalComposite<AgentType>::Status GoalComposite<AgentType>::ProcessSubgoals()
{
	// Remove all completed goals
	while (!mSubgoals.empty())
	{
		Goal<AgentType>* goal = mSubgoals.front();
		if (goal->mStatus != Status::Completed &&
			goal->mStatus != Status::Failed)
		{
			break;
		}

		goal->Terminate();
		SafeDelete(goal);
		mSubgoals.pop_front();
	}

	// If any Goals remain, continue to process
	if (!mSubgoals.empty())
	{
		Status status = mSubgoals.front()->Process();
		if (status == Status::Completed && mSubgoals.size() > 1)
		{
			return Status::Active;
		}
		else
		{
			return status;
		}
	}
	return Status::Completed;
}

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_GOALCOMPOSITE_H
