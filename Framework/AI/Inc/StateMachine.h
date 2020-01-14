#ifndef INCLUDED_COOENGINE_AI_STATEMACHINE_H
#define INCLUDED_COOENGINE_AI_STATEMACHINE_H

#include "State.h"

namespace Coo::AI 
{

template<class AgentType>
class StateMachine 
{
public:
	using StateType = State<AgentType>;

	StateMachine(AgentType& agent);

	template<class NewStateType>
	void AddState();

	void Update(float deltaTime);
	void ChangeState(std::string stateName);

private:
	using StateMap = std::unordered_map<std::string, std::unique_ptr<StateType>>;

	AgentType& mAgent;
	StateType* mCurrentState = nullptr;
	StateMap mStates;
};

template<class AgentType>
inline StateMachine<AgentType>::StateMachine(AgentType& agent)
	: mAgent(agent)
{
}

template<class AgentType>
inline void StateMachine<AgentType>::Update(float deltaTime)
{
	mCurrentState->Update(mAgent, deltaTime);
}

template<class AgentType>
inline void StateMachine<AgentType>::ChangeState(std::string stateName)
{
	auto iter = mStates.find(stateName);
	if (iter == mStates.end())
		return;

	mCurrentState->Exit(mAgent);
	mCurrentState = iter->second.get();
	mCurrentState->Enter(mAgent);
}

template<class AgentType>
template<class NewStateType>
inline void StateMachine<AgentType>::AddState()
{
	static_assert(std::is_base_of<StateType, NewStateType>::value, "[StateMachine] Can only add state for AgentType");
	auto[state, success] = mStates.emplace(NewStateType::GetName(), std::make_unique<NewStateType>());
	if (!mCurrentState)
		mCurrentState = state->second.get();
}

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_STATEMACHINE_H
