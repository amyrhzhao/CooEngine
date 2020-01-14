#include "Dude.h"

using namespace Coo::Graphics;
using namespace Coo::Math;
using namespace Coo::AI;

class VisualSensor : public::Sensor
{
public:
	void Update(Agent& agent, MemoryRecords& memory, float deltaTime) override
	{
		auto viewRangeSqr = viewRange * viewRange;
		auto cosViewAngle = cosf(viewAngle * 0.5f);
		auto meats = agent.world.GetEntity(1);
		for (auto meat : meats)
		{
			// Check distance
			if (DistanceSqr(agent.position, meat->position) > viewRangeSqr)
			{
				continue;
			}
			// Check angle/fov
			if (Dot(agent.heading, Normalize(meat->position - agent.position)) < cosViewAngle)
			{
				continue;
			}
			// Raycast

			
			// find in memory
			
			// if found in memory, update
			// else, add to memory
		}
	}

	float viewRange;
	float viewAngle;
};

void Dude::Initialize()
{
	maxSpeed = 100.0f;
	position = { 640.0f, 360.0f };
	radius = 20.0f;

	// Initialize perception module
	perceptionModule = std::make_unique<PerceptionModule>(*this, [](Coo::AI::MemoryRecord&) {});
	auto visual = perceptionModule->AddSensor<VisualSensor>("Vision");

	// Initialize steering module
	steeringModule = std::make_unique<SteeringModule>(*this);
	steeringModule->AddBehavior<WanderBehavior>("Wander")->SetActive(true);
	steeringModule->AddBehavior<ObstacleAvoidanceBehavior>("ObsAvoid")->SetActive(true);

	// Initialize texture
	auto tm = TextureManager::Get();
	mTextureId = tm->LoadTexture("chara2_3/down_stand.png");
}

void Dude::Update(float deltaTime)
{
	// Move the character back when it goes out of the screen
	auto graphicSystem = Coo::Graphics::GraphicsSystem::Get();
	auto screenWidth = static_cast<float>(graphicSystem->GetBackBufferWidth());
	auto screenHeight = static_cast<float>(graphicSystem->GetBackBufferHeight());
	if (Agent::position.x < 0) { Agent::position.x = screenWidth - 10.0f; }
	if (Agent::position.x > screenWidth) { Agent::position.x = 10.0f; }
	if (Agent::position.y < 0) { Agent::position.y = screenHeight - 10.0f; }
	if (Agent::position.y > screenHeight) { Agent::position.y = 10.0f; }

	auto force = steeringModule->Calculate();
	auto acceleration = force / mass;
	Agent::velocity += acceleration * deltaTime;
	Agent::velocity = Coo::Math::Truncate(Agent::velocity, Agent::maxSpeed);
	Agent::position += Agent::velocity * deltaTime;
	if (Magnitude(velocity) > 1.0f)
	{
		heading = Normalize(velocity);
	}
}

void Dude::Render()
{
	auto sr = SpriteRenderer::Get();
	auto tm = TextureManager::Get();
	auto texture = tm->GetTexture(mTextureId);
	sr->Draw(*texture, { position.x - 9.0f , position.y - 15.0f });
	// Draw the direction and the size of the agent
	Coo::Graphics::SimpleDraw::AddScreenLine(Agent::position, Agent::position + Agent::velocity, Coo::Graphics::Colors::Cyan);
	Coo::Graphics::SimpleDraw::AddScreenCircle(Agent::position, Agent::radius, Coo::Graphics::Colors::Pink);
}

void Dude::DebugUI()
{

}

