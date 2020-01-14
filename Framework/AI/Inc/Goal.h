#ifndef INCLUDED_COOENGINE_AI_GOAL_H
#define INCLUDED_COOENGINE_AI_GOAL_H

namespace Coo::AI 
{

template <typename AgentType>
class Goal
{
public:
	enum class Status
	{
		Inactive,
		Active,
		Completed,
		Failed
	};

	Goal(AgentType& agent);
	virtual ~Goal();

	// Function to activate this action
	virtual void Activate() = 0;

	// Function to update this action
	virtual Status Process() = 0;

	// Function to terminate this action
	virtual void Terminate() = 0;

protected:
	void ActivateIfInactive();

	AgentType& mAgent;
	Status mStatus;
};

template <typename AgentType>
Goal<AgentType>::Goal(AgentType& agent)
	: mAgent(agent)
	, mStatus(Status::Inactive)
{
}

template <typename AgentType>
Goal<AgentType>::~Goal()
{
}

template <typename AgentType>
void Goal<AgentType>::ActivateIfInactive()
{
	if (mStatus == Status::Inactive)
	{
		Activate();
	}
}

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_GOAL_H
