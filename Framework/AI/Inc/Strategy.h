#ifndef INCLUDED_COOENGINE_AI_STRATEGY_H
#define INCLUDED_COOENGINE_AI_STRATEGY_H

#include "Goal.h"

namespace Coo::AI 
{

template <typename AgentType>
class Strategy
{
public:
	Strategy(AgentType& agent) : mAgent(agent) {}
	virtual ~Strategy() {}

	virtual float CalculateDesirability() = 0;
	virtual Goal<AgentType>* CreateGoal() = 0;

protected:
	AgentType& mAgent;
};

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_STRATEGY_H
