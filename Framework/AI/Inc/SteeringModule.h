#ifndef INCLUDED_COOENGINE_AI_STEERINGMODULE_H
#define INCLUDED_COOENGINE_AI_STEERINGMODULE_H

#include "SteeringBehavior.h"
namespace Coo::AI {

class SteeringModule 
{
public:
	SteeringModule(Agent& agent);

	template <class BehaviorType>
	BehaviorType* AddBehavior(std::string name);

	template <class BehaviorType>
	BehaviorType* GetBehavior(const std::string& name);

	Math::Vector2 Calculate();

private:
	using BehaviorMap = std::unordered_map<std::string, std::unique_ptr<SteeringBehavior>>;

	Agent& mAgent;
	BehaviorMap mBehaviors;
};

template<class BehaviorType>
inline BehaviorType* SteeringModule::AddBehavior(std::string name)
{
	auto [iter, success] =  mBehaviors.try_emplace(std::move(name), std::make_unique<BehaviorType>());
	return success ? static_cast<BehaviorType*>(iter->second.get()) : nullptr;
}

template<class BehaviorType>
inline BehaviorType * SteeringModule::GetBehavior(const std::string& name)
{
	auto iter = mBehaviors.find(name);
	return iter == mBehaviors.end() ? nullptr : static_cast<BehaviorType*>(iter->second.get());
}

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_STEERINGMODULE_H