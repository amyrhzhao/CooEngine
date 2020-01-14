#ifndef INCLUDED_COOENGINE_AI_STEERINGBEHAVIOR_H
#define INCLUDED_COOENGINE_AI_STEERINGBEHAVIOR_H

#include "Common.h"

namespace Coo::AI {

class Agent;

class SteeringBehavior 
{
public:
	SteeringBehavior() = default;
	virtual ~SteeringBehavior() = default;

	virtual Math::Vector2 Calculate(Agent& agent) = 0;

	void SetActive(bool active)	{ mActive = active; }
	bool IsActive() const		{ return mActive; }

	void SetWeight(float weight) { mWeight = weight; }

protected:
	float mWeight = 1.0f;

private:
	bool mActive = false;
};

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_STEERINGBEHAVIOR_H
