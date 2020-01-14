#ifndef INCLUDED_COOENGINE_AI_STEERINGBEHAVIORTYPE_H
#define INCLUDED_COOENGINE_AI_STEERINGBEHAVIORTYPE_H

#include "SteeringBehavior.h"
namespace Coo::AI
{
// Nice TODO Interpose, Hide, Path following, Offset pursuit, Wall avoidance
class SeekBehavior : public Coo::AI::SteeringBehavior
{
public:
	Coo::Math::Vector2 Calculate(Coo::AI::Agent& agent);
	void SetTarget(const Math::Vector2& target) { destination = target; }

protected:
	Math::Vector2 destination = {};
};

class FleeBehavior : public Coo::AI::SteeringBehavior 
{
public:
	Coo::Math::Vector2 Calculate(Coo::AI::Agent& agent);
	void SetTarget(const Math::Vector2& target) { fleeTarget = target; }

protected:
	Math::Vector2 fleeTarget = {};
	float panicDistance = 100.0f;
};

class ArriveBehavior : public Coo::AI::SteeringBehavior 
{
public:
	Coo::Math::Vector2 Calculate(Coo::AI::Agent& agent);
	void SetSlowingRadius(float radius) { slowingRadius = radius; }
	void SetTarget(const Math::Vector2& target) { destination = target; }

private:
	Math::Vector2 destination = {};
	float slowingRadius = 100.0f;
};

class WanderBehavior : public Coo::AI::SteeringBehavior
{
public:
	Coo::Math::Vector2 Calculate(Coo::AI::Agent& agent);
	void SetWanderDistance(float distance) { wanderDistance = distance; }
	void SetWanderRadius(float radius) { wanderRadius = radius; }
	void SetWanderJitter(float jitter) { wanderJitter = Math::abs(jitter); }

private:
	float wanderDistance = 25.0f;
	float wanderRadius = 50.0f;
	float wanderJitter = 0.2f;
	float wanderAngle = Math::RandomFloat(-Math::TwoPi, Math::TwoPi);
};

class PursuitBehavior : public Coo::AI::SeekBehavior 
{
public:
	Coo::Math::Vector2 Calculate(Coo::AI::Agent& agent);
	void SetTarget(Agent* target) { evader = target; }

private:
	Agent* evader = nullptr;
};

class EvadeBehavior : public Coo::AI::FleeBehavior 
{
public:
	Coo::Math::Vector2 Calculate(Coo::AI::Agent& agent);
	void SetTarget(Agent* target) { pursuer = target; }

private:
	Agent* pursuer = nullptr;
};

class ObstacleAvoidanceBehavior : public Coo::AI::SteeringBehavior
{
public:
	Coo::Math::Vector2 Calculate(Coo::AI::Agent& agent);
	void SetMaxSight(float sight) { maxSight = sight; }

private:
	float lateralPower = 150.0f;
	float brakingPower = 0.1f;
	float maxSight = 80.0f;
};

class SeparationBehavior : public Coo::AI::SteeringBehavior 
{
public:
	Coo::Math::Vector2 Calculate(Coo::AI::Agent& agent);

private:

};

class AlignmentBehavior : public Coo::AI::SteeringBehavior
{
public:
	Coo::Math::Vector2 Calculate(Coo::AI::Agent& agent);

private:

};

class CohesionBehavior : public Coo::AI::SteeringBehavior
{
public:
	Coo::Math::Vector2 Calculate(Coo::AI::Agent& agent);
private:

};

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_STEERINGBEHAVIORTYPE_H
