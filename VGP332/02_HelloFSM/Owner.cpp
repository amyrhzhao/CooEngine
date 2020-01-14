#include "Owner.h"

using namespace Coo::AI;
void Owner::Initialize()
{
	Agent::position = { 640.0f, 360.0f };
	textureId = Coo::Graphics::TextureManager::Get()->LoadTexture("chara2_3/down_stand.png");
	steeringModule = std::make_unique<SteeringModule>(*this);
	//steeringModule->AddBehavior<SeekBehavior>("Seek")->SetActive(true);
	steeringModule->AddBehavior<WanderBehavior>("Wander")->SetActive(false);
	steeringModule->AddBehavior<ArriveBehavior>("Arrive")->SetActive(true);
	//steeringModule->GetBehavior<ArriveBehavior>("Arrive")->SetSlowingRadius(10.0f);
	steeringModule->AddBehavior<ObstacleAvoidanceBehavior>("ObsAvoid")->SetActive(true);
	Agent::maxSpeed = 100.0f;
}

void Owner::Update(float deltaTime)
{
	// Move the character back when it goes out of the screen
	auto graphicSystem = Coo::Graphics::GraphicsSystem::Get();
	auto screenWidth = static_cast<float>(graphicSystem->GetBackBufferWidth());
	auto screenHeight = static_cast<float>(graphicSystem->GetBackBufferHeight());
	if (Agent::position.x < 0) { Agent::position.x = screenWidth - 10.0f; }
	if (Agent::position.x > screenWidth) { Agent::position.x = 10.0f; }
	if (Agent::position.y < 0) { Agent::position.y = screenHeight - 10.0f; }
	if (Agent::position.y > screenHeight) { Agent::position.y = 10.0f; }

	// MousePosition
	auto inputSystem = Coo::Input::InputSystem::Get();
	auto mouseScreenX = static_cast<float>(inputSystem->GetMouseScreenX());
	auto mouseScreenY = static_cast<float>(inputSystem->GetMouseScreenY());
	destination = {mouseScreenX, mouseScreenY};
	steeringModule->GetBehavior<ArriveBehavior>("Arrive")->SetTarget(destination);

	// Moving calculation
	auto force = steeringModule->Calculate();
	auto acceleration = force / Agent::mass;
	Agent::velocity += acceleration * deltaTime;
	Agent::velocity = Coo::Math::Truncate(Agent::velocity, Agent::maxSpeed);
	Agent::position += Agent::velocity * deltaTime;
	if (Coo::Math::Magnitude(Agent::velocity) > 1.0f)
	{
		Agent::heading = Coo::Math::Normalize(Agent::velocity);
	}

}

void Owner::Render() const
{
	auto texture = Coo::Graphics::TextureManager::Get()->GetTexture(textureId);
	Coo::Graphics::SpriteRenderer::Get()->Draw(*texture, {position.x - 9.0f , position.y - 15.0f });
	// Draw the direction of the agent
	Coo::Graphics::SimpleDraw::AddScreenLine(Agent::position, Agent::position + Agent::velocity, Coo::Graphics::Colors::Cyan);
	Coo::Graphics::SimpleDraw::AddScreenCircle(Agent::position, 20.0f, Coo::Graphics::Colors::Pink);
}
