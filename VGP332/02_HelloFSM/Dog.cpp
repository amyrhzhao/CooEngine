#include "Dog.h"
#include <random>
#include "Bone.h"
#include "Owner.h"

using namespace Coo::AI;
using namespace Coo;

std::random_device myRandomDevice{};
std::mt19937 myRandomEngine{ myRandomDevice() };

float RandomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>{ min, max }(myRandomEngine);
}

Math::Vector2 RandomUnitCircle(bool normalized = true)
{
	Math::Vector2 randVal{
		RandomFloat(-1.0f, 1.0f) + 0.001f,
		RandomFloat(-1.0f, 1.0f)
	};
	if (normalized)
		return Math::Normalize(randVal);
	return randVal;
}

void Dog::Initialize()
{
	stateMachine = std::make_unique<StateMachine<Dog>>(*this);
	steeringModule = std::make_unique<SteeringModule>(*this);
	stateMachine->AddState<WhirlState>();
	stateMachine->AddState<RunState>();
	stateMachine->AddState<FetchState>();
	steeringModule->AddBehavior<SeekBehavior>("Seek");
	steeringModule->AddBehavior<ArriveBehavior>("Arrive");
	steeringModule->AddBehavior<PursuitBehavior>("Pursuit")->SetActive(false);
	Agent::maxSpeed = 200.0f;
	textureId = Coo::Graphics::TextureManager::Get()->LoadTexture("dog2/down_stand.png");
	stateMachine->ChangeState("Run");
}

void Dog::Update(float deltaTime)
{
	stateMachine->Update(deltaTime);
	// Move the character back when it goes out of the screen
	auto graphicSystem = Coo::Graphics::GraphicsSystem::Get();
	auto screenWidth = static_cast<float>(graphicSystem->GetBackBufferWidth());
	auto screenHeight = static_cast<float>(graphicSystem->GetBackBufferHeight());
	if (Agent::position.x < 0) { Agent::position.x = screenWidth - 10.0f; }
	if (Agent::position.x > screenWidth) { Agent::position.x = 10.0f; }
	if (Agent::position.y < 0) { Agent::position.y = screenHeight - 10.0f; }
	if (Agent::position.y > screenHeight) { Agent::position.y = 10.0f; }
}

void Dog::Render() const
{
	auto texture = Coo::Graphics::TextureManager::Get()->GetTexture(textureId);
	Coo::Graphics::SpriteRenderer::Get()->Draw(*texture, Agent::position);
}

void Dog::FetchBone(Bone& bone)
{
	target = &bone;
}

void WhirlState::Enter(Dog& agent)
{
	position = agent.position;
	time = 0.0f;
}

void WhirlState::Update(Dog& agent, float deltaTime)
{
	time += deltaTime * 10.0f;
	if (time > Coo::Math::TwoPi)
	{
		agent.stateMachine->ChangeState("Run");
	}
	else
	{
		agent.position.x = position.x + sinf(time * 1.0f) * 30.0f;
		agent.position.y = position.y + cosf(time * 1.0f) * 10.0f;
	}
	if (agent.target)
	{
		agent.stateMachine->ChangeState("Fetch");
	}
}

void WhirlState::Exit(Dog& agent)
{
	agent.position = position;
}

void RunState::Enter(Dog& agent)
{
	target = RandomUnitCircle() * 250.0f + Math::Vector2{ 600.0f, 350.0f };
	auto seek = agent.steeringModule->GetBehavior<SeekBehavior>("Seek");
	seek->SetActive(true);
	seek->SetTarget(target);
}

void RunState::Update(Dog& agent, float deltaTime)
{
	if (Coo::Math::DistanceSqr(agent.position, target) < 10.0f)
	{
		agent.stateMachine->ChangeState("Whirl");
	}
	else
	{
		auto force = agent.steeringModule->Calculate();
		auto acceleration = force / agent.mass;
		agent.velocity += acceleration * deltaTime;
		agent.position += agent.velocity * deltaTime;

		if (Coo::Math::Magnitude(agent.velocity) > 1.0f)
		{
			agent.heading = Coo::Math::Normalize(agent.velocity);
		}
	}
	if (agent.target)
	{
		agent.stateMachine->ChangeState("Fetch");
	}
}

void RunState::Exit(Dog& agent)
{
	agent.steeringModule->GetBehavior<SteeringBehavior>("Seek")->SetActive(false);
}

void FetchState::Enter(Dog & agent)
{
	auto arrive = agent.steeringModule->GetBehavior<ArriveBehavior>("Arrive");
	arrive->SetActive(true);
	target = agent.target->GetPosition();
	arrive->SetTarget(target);
	auto pursuit = agent.steeringModule->GetBehavior<PursuitBehavior>("Pursuit");
	pursuit->SetTarget(agent.owner);
	agent.isFetching = false;
}

void FetchState::Update(Dog & agent, float deltaTime)
{

	if (agent.isFetching)
	{
		agent.target->Move(agent.position);
		// if the dog get the bone and is near enough to the owner
		if (Coo::Math::DistanceSqr(agent.position, agent.owner->position) < 150.0f)
		{
			agent.target->Kill();
			agent.target = nullptr;
			agent.stateMachine->ChangeState("Run");
		}
	}
	else
	{
		// if the dog is close enough to the bone
		if (Coo::Math::DistanceSqr(agent.position, target) < 10.0f)
		{
			agent.isFetching = true;
			agent.steeringModule->GetBehavior<SteeringBehavior>("Arrive")->SetActive(false);
			agent.steeringModule->GetBehavior<SteeringBehavior>("Pursuit")->SetActive(true);
		}
	}
	auto force = agent.steeringModule->Calculate();
	auto acceleration = force / agent.mass;
	agent.velocity += acceleration * deltaTime;
	agent.velocity = Coo::Math::Truncate(agent.velocity, agent.maxSpeed);
	agent.position += agent.velocity * deltaTime;

	if (Coo::Math::Magnitude(agent.velocity) > 1.0f)
	{
		agent.heading = Coo::Math::Normalize(agent.velocity);
	}
}

void FetchState::Exit(Dog & agent)
{
	agent.steeringModule->GetBehavior<SteeringBehavior>("Seek")->SetActive(false);
	agent.steeringModule->GetBehavior<SteeringBehavior>("Pursuit")->SetActive(false);
}
