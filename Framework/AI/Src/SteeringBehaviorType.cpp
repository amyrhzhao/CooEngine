#include "Precompiled.h"
#include "SteeringBehaviorType.h"
#include "Agent.h"
#include "AIWorld.h"

using namespace Coo::AI;
using namespace Coo::Math;
Vector2 SeekBehavior::Calculate(Agent & agent)
{
	auto desiredVelocity = Normalize(destination - agent.position) * agent.maxSpeed;
	return (desiredVelocity - agent.velocity) * mWeight;
}

Vector2 ArriveBehavior::Calculate(Agent & agent)
{
	// Calculate the desired velocity
	Vector2 desiredVelocity = destination - agent.position;
	float distance = Magnitude(desiredVelocity);
	if (distance < slowingRadius)
	{
		// Inside the slowing area
		desiredVelocity = Math::Normalize(desiredVelocity) * agent.maxSpeed * (distance / slowingRadius);
	}
	else
	{
		// Outside the slowing area
		desiredVelocity = Math::Normalize(desiredVelocity) * agent.maxSpeed;
	}

	// Check if the agent is on the target
	if (DistanceSqr(destination, agent.position) < 1.0f)
	{
		agent.velocity = { 0.0f, 0.0f };
		return { 0.0f,0.0f };
	}

	// Calculate and return the steering force
	return (desiredVelocity - agent.velocity) * mWeight * slowingRadius / distance;
}

Vector2 WanderBehavior::Calculate(Agent & agent)
{
	// Calculate the circle center
	Math::Vector2 circleCenter;
	if (MagnitudeSqr(agent.velocity) == 0.0f)
	{
		return {};
	}
	circleCenter = Math::Normalize(agent.velocity);
	circleCenter *= wanderDistance;

	// Calculate the displacement force
	Math::Vector2 displacement{ 0.0f, -1.0f };
	displacement *= wanderRadius;

	// Randomly change direction by making it change its current angle
	float len = Math::Magnitude(displacement);
	displacement.x = cosf(wanderAngle) * len;
	displacement.y = sinf(wanderAngle) * len;

	// Change wanderAngle a bit
	wanderAngle += Math::RandomFloat(-wanderJitter, wanderJitter);

	// Calculate and return the wander force
	Vector2 wanderForce;
	wanderForce = circleCenter + displacement;
	return wanderForce * mWeight;
}

Vector2 ObstacleAvoidanceBehavior::Calculate(Agent& agent)
{
	Matrix3 localToWorld = agent.LocalToWorld();
	Matrix3 worldToLocal = Inverse(localToWorld);

	Vector2 avoidanceForce = {};
	auto obstacles = agent.world.GetObstacles();
	std::vector<Circle> obstaclesInRange;
	// Eliminate obstacles that are too far
	for (auto& obstacle : obstacles)
	{
		float distance = Magnitude(obstacle.center - agent.position);
		if (distance <= maxSight)
		{
			obstaclesInRange.push_back(obstacle);
		}
	}

	// Convert into agent space
	for (auto iter = obstaclesInRange.begin(); iter != obstaclesInRange.end();)
	{
		(*iter).center = TransformCoord((*iter).center, worldToLocal);
		// Eliminate obstacles behind agent
		if ((*iter).center.y < 0.0f)
		{
			iter = obstaclesInRange.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	float widthExpend = agent.radius;
	for (auto iter = obstaclesInRange.begin(); iter != obstaclesInRange.end();)
	{
		// Expand remaining obstables by width of bounding box
		(*iter).radius += widthExpend;
		// Eliminate obstables whos abs(local x) < expanded radius
		if (abs((*iter).center.x) > (*iter).radius)
		{
			iter = obstaclesInRange.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	Circle* closestTarget = nullptr;
	float distToClosest = std::numeric_limits<float>().max();

	for (auto& obstacle : obstaclesInRange)
	{
		float A = obstacle.center.y;
		float B = obstacle.center.x;
		float r = obstacle.radius;
		// Compute intersection points
		float intersection1 = max(A + sqrtf(r * r - B * B), 0.0f);
		float intersection2 = max(A - sqrtf(r * r - B * B), 0.0f);
		float y = min(intersection1, intersection2);
		// Obstacle with closest intersection point used for computing steering force
		if (y < distToClosest)
		{
			closestTarget = &obstacle;
			distToClosest = y;
		}
	}

	if (closestTarget)
	{
		// Compute lateral force
		Vector2 lateralForce;
		//lateralForce = Vector2::XAxis() * (lateralPower / abs(closestTarget->center.x) ) * ( closestTarget->center.x / abs(closestTarget->center.x));
		if (closestTarget->center.x > 0.0f)
		{
			lateralForce = Vector2(-agent.heading.y, agent.heading.x) * (closestTarget->radius - abs(closestTarget->center.x)) * lateralPower;
		}
		else
		{
			lateralForce = Vector2(agent.heading.y, -agent.heading.x) * (closestTarget->radius - abs(closestTarget->center.x)) * lateralPower;
		}
		lateralForce = TransformCoord(lateralForce, worldToLocal);

		// Compute braking force
		Vector2 brakingForce;
		if (closestTarget->center.y != 0.0f)
		{
			brakingForce = -Vector2::YAxis() * (brakingPower / (closestTarget->center.y / maxSight));
			//brakingForce = (-agent.heading) / closestTarget->center.y;
		}

		// Convert lateral and braking force to world space
		avoidanceForce = lateralForce + brakingForce;
		avoidanceForce = TransformCoord(avoidanceForce, localToWorld);
		//Coo::Graphics::SimpleDraw::AddScreenLine(agent.position, agent.position + avoidanceForce, Coo::Graphics::Colors::LightBlue);
	}

	return avoidanceForce * mWeight;
}

Vector2 FleeBehavior::Calculate(Agent & agent)
{
	// Check if the agent and fleeTarget is close enough
	float panicDistanceSqr = panicDistance * panicDistance;
	if (DistanceSqr(agent.position, fleeTarget) > panicDistanceSqr)
	{
		return { 0.0f,0.0f };
	}
	auto desiredVelocity = Normalize(agent.position - fleeTarget) * agent.maxSpeed;
	return (desiredVelocity - agent.velocity) * mWeight;
}

Coo::Math::Vector2 Coo::AI::SeparationBehavior::Calculate(Coo::AI::Agent & agent)
{
	Vector2 force;
	for (auto& neighbour : agent.neighbourhood)
	{
		if (neighbour == &agent) { continue; }
		Vector2 delta = agent.position - neighbour->position;
		float dist = Magnitude(delta);
		if (dist != 0.0f)
		{
			force += Normalize(delta) / dist;
		}
	}
	if (MagnitudeSqr(force) == 0.0f) { return { 0.0f,0.0f }; }
	return ((Normalize(force) * agent.maxSpeed) - agent.velocity) * mWeight;
}

Coo::Math::Vector2 Coo::AI::AlignmentBehavior::Calculate(Coo::AI::Agent & agent)
{
	Vector2 force = agent.heading;
	for (auto& neighbour : agent.neighbourhood)
	{
		force += neighbour->heading;
	}
	force /= static_cast<float>(agent.neighbourhood.size() + 1);
	return ((Normalize(force) * agent.maxSpeed) - agent.velocity)* mWeight;

}

Coo::Math::Vector2 Coo::AI::CohesionBehavior::Calculate(Coo::AI::Agent & agent)
{
	Vector2 position = agent.position;
	for (auto& neighbour : agent.neighbourhood)
	{
		position += neighbour->position;
	}
	if (agent.neighbourhood.size() >= 1)
	{
		position /= static_cast<float>(agent.neighbourhood.size() + 1);
		Math::Vector2 mDesiredVelocity = Math::Normalize(position - agent.position) * agent.maxSpeed;
		Math::Vector2 temp = mDesiredVelocity - agent.velocity;
		return temp * mWeight;
	}
	else
	{
		return{};
	}
}

Coo::Math::Vector2 Coo::AI::PursuitBehavior::Calculate(Coo::AI::Agent & agent)
{
	if (evader == nullptr) { return {}; } // Check if the agent has a target
	Math::Vector2 toEvader = evader->position - agent.position;
	float relativeHeading = Dot(evader->heading, agent.heading);
	if ((Dot(toEvader, agent.heading) > 0.0f)
		&& (relativeHeading < -0.95f))
	{
		destination = evader->position;
		return SeekBehavior::Calculate(agent);
	}

	float timeToTarget = Magnitude(toEvader) / (agent.maxSpeed + Magnitude(evader->velocity));
	destination = evader->position + (evader->velocity * timeToTarget);
	return SeekBehavior::Calculate(agent);
}

Coo::Math::Vector2 Coo::AI::EvadeBehavior::Calculate(Coo::AI::Agent & agent)
{
	if (pursuer == nullptr) { return {}; }
	Math::Vector2 toPursuer = pursuer->position - agent.position;
	float lookAheadTime = Magnitude(toPursuer) / (agent.maxSpeed + Magnitude(pursuer->velocity));
	fleeTarget = pursuer->position + (pursuer->velocity * lookAheadTime);
	return FleeBehavior::Calculate(agent);
}
