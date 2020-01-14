#include "Sheep.h"
#include "GrassManager.h"

const float growSpeed = 0.2f;
const float maxSize = 15.0f;
const float eatingSpeed = 10.0f;

using namespace Coo::Math;
using namespace Coo::AI;
using namespace Coo::Graphics;

void Sheep::Initialize(Vector2 pos)
{
	position = pos;
	maxSpeed = 150.0f;
	radius = 7.0f;
	typeId = 2;

	steeringModule = std::make_unique<SteeringModule>(*this);
	auto arrive = steeringModule->AddBehavior<ArriveBehavior>("Arrive");
	arrive->SetActive(false);
	arrive->SetSlowingRadius(50.0f);
	auto wander = steeringModule->AddBehavior<WanderBehavior>("Wander");
	wander->SetActive(false);
	wander->SetWeight(0.7f);
	//steeringModule->AddBehavior<EvadeBehavior>("Flee")->SetActive(false);
	stateMachine = std::make_unique<StateMachine<Sheep>>(*this);
	stateMachine->AddState<SeekState>();
	stateMachine->AddState<EatState>();
	stateMachine->ChangeState("Seek");
}

void Sheep::Terminate()
{
}

void Sheep::Render() const
{
	Coo::Graphics::SimpleDraw::AddScreenCircle(position, radius, renderColor);
	Coo::Graphics::SimpleDraw::AddScreenCircle(position, radius - 1.5f, stateColor);
	Coo::Graphics::SimpleDraw::AddScreenCircle(position, radius - 2.0f, stateColor);
}

void Sheep::Update(float deltaTime)
{
	stateMachine->Update(deltaTime);
	UpdateSteering(deltaTime);
	// Move the character back when it goes out of the screen
	auto graphicSystem = Coo::Graphics::GraphicsSystem::Get();
	auto screenWidth = static_cast<float>(graphicSystem->GetBackBufferWidth());
	auto screenHeight = static_cast<float>(graphicSystem->GetBackBufferHeight());
	if (position.x < 0) { position.x = screenWidth - radius * 0.5f; }
	if (position.x > screenWidth) { position.x = radius * 0.5f; }
	if (position.y < 0) { position.y = screenHeight - radius * 0.5f; }
	if (position.y > screenHeight) { position.y = radius * 0.5f; }
}

void Sheep::Grow(float food)
{
	float sizeWantsToGrow = radius + food * growSpeed;
	if (sizeWantsToGrow > maxSize) 
	{
		radius = maxSize;
	}
	else
	{
		radius = sizeWantsToGrow;
	}
}

void Sheep::UpdateSteering(float deltaTime)
{
	auto force = steeringModule->Calculate();
	auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	velocity = Coo::Math::Truncate(velocity, maxSpeed);
	position += velocity * deltaTime;
	if (Magnitude(velocity) > 1.0f)
	{
		heading = Normalize(velocity);
	}
}

void EatState::Enter(Sheep& agent)
{
	agent.velocity = {};
	agent.steeringModule->GetBehavior<ArriveBehavior>("Arrive")->SetActive(false);
	agent.steeringModule->GetBehavior<WanderBehavior>("Wander")->SetActive(false);
	agent.stateColor = Colors::Purple;
}

void EatState::Update(Sheep& agent, float deltaTime)
{
	bool isEating = false;
	auto gm = GrassManager::Get();
	for (int i = 0; i < gm->GrassCount(); ++i) 
	{
		auto grass = gm->GetGrass(i);
		if (!grass->IsActive()) { continue; }
		if (Intersect(grass->GetBoundingCircle(), agent.GetBoundingCircle())) 
		{
			auto eat = grass->Cost(eatingSpeed * deltaTime);
			agent.Grow(eat);
			break;
		}
	}
	if (!isEating) 
	{
		agent.stateMachine->ChangeState("Seek");
	}
}

void EatState::Exit(Sheep& agent)
{
}

void SeekState::Enter(Sheep& agent)
{
	agent.steeringModule->GetBehavior<ArriveBehavior>("Arrive")->SetActive(true);
	agent.steeringModule->GetBehavior<WanderBehavior>("Wander")->SetActive(true);
	agent.stateColor = Colors::AliceBlue;
}

void SeekState::Update(Sheep& agent, float deltaTime)
{
	auto gm = GrassManager::Get();
	float minDistSqr = std::numeric_limits<float>::max();
	Vector2 min = {};
	for (int i = 0; i < gm->GrassCount(); ++i)
	{
		auto grass = gm->GetGrass(i);
		float distSqr = DistanceSqr(agent.position, grass->position);
		if (distSqr < minDistSqr) 
		{
			minDistSqr = distSqr;
			min = grass->position;
		}
	}
	agent.steeringModule->GetBehavior<ArriveBehavior>("Arrive")->SetTarget(min);
	if (minDistSqr < 300.0f) 
	{
		agent.stateMachine->ChangeState("Eat");
	}
}

void SeekState::Exit(Sheep& agent)
{
}
