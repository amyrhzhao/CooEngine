#include "Precompiled.h"
#include "SteeringModule.h"

using namespace Coo;

Coo::AI::SteeringModule::SteeringModule(Agent & agent): mAgent(agent)
{

}

Math::Vector2 Coo::AI::SteeringModule::Calculate()
{
	Math::Vector2 total;
	for (auto&[name, behavior] : mBehaviors) 
	{
		if (behavior->IsActive()) 
		{
			total += behavior->Calculate(mAgent);
		}
	}
	return total;
}
