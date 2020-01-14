#ifndef INCLUDED_COOENGINE_AI_STATE_H
#define INCLUDED_COOENGINE_AI_STATE_H

#include "Common.h"

namespace Coo::AI 
{

template <class AgentType>
class State
{
public:
	virtual ~State() = default;
	virtual void Enter(AgentType& agent) = 0;
	virtual void Update(AgentType& agent, float deltaTime) = 0;
	virtual void Exit(AgentType& agent) = 0;
};

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_STATE_H
