#include "Dude.h"

float screenWidth;
float screenHeight;
float edgeWidth = 10.0f;
using namespace Coo::Graphics;
using namespace Coo::Math;
using namespace Coo::AI;
void Dude::Initialize()
{
	auto graphicSystem = Coo::Graphics::GraphicsSystem::Get();
	screenWidth = static_cast<float>(graphicSystem->GetBackBufferWidth());
	screenHeight = static_cast<float>(graphicSystem->GetBackBufferHeight());
	maxSpeed = 300.0f;
	neighbourhoodRadius = 50.0f;
	position = { RandomFloat(edgeWidth, screenWidth),RandomFloat(edgeWidth, screenHeight) };
	radius = 5.0f;
	steeringModule = std::make_unique<SteeringModule>(*this);
	auto tm = TextureManager::Get();
	mTextureId = tm->LoadTexture("chara2_3/down_stand.png");
	steeringModule->AddBehavior<SeekBehavior>("Seek")->SetActive(false);
	steeringModule->AddBehavior<WanderBehavior>("Wander")->SetActive(true);
	steeringModule->AddBehavior<ObstacleAvoidanceBehavior>("ObsAvoid")->SetActive(true);
	steeringModule->AddBehavior<SeparationBehavior>("Speration")->SetActive(true);
	steeringModule->AddBehavior<AlignmentBehavior>("Alignment")->SetActive(true);
	steeringModule->AddBehavior<CohesionBehavior>("Cohesion")->SetActive(true);
}

void Dude::Update(float deltaTime)
{
	// Move the character back when it goes out of the screen
	if (position.x < 0) { position.x = screenWidth - edgeWidth; }
	if (position.x > screenWidth) { position.x = edgeWidth; }
	if (position.y < 0) { position.y = screenHeight - edgeWidth; }
	if (position.y > screenHeight) { position.y = edgeWidth; }

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

void Dude::Render()
{
	auto sr = SpriteRenderer::Get();
	auto tm = TextureManager::Get();
	auto texture = tm->GetTexture(mTextureId);
	//sr->Draw(*texture, { position.x - 9.0f , position.y - 15.0f });
	// Draw the direction and the size of the agent
	Coo::Graphics::SimpleDraw::AddScreenLine(Agent::position, Agent::position + (Agent::velocity * 0.2f), Coo::Graphics::Colors::Cyan);
	Coo::Graphics::SimpleDraw::AddScreenCircle(Agent::position, Agent::radius, Coo::Graphics::Colors::Pink);
}

void Dude::DebugUI()
{
	//std::string name = "Agent" + std::to_string(id);
	//if (ImGui::CollapsingHeader(name.c_str()))
	//{
	//	auto seekBehavior = steeringModule->GetBehavior<SeekBehavior>("Seek");
	//	useSeek = seekBehavior->IsActive();
	//	if (ImGui::Checkbox("Seek", &useSeek))
	//	{
	//		seekBehavior->SetActive(useSeek);
	//	}
	//	auto wanderBehavior = steeringModule->GetBehavior<WanderBehavior>("Wander");
	//	useWander = wanderBehavior->IsActive();
	//	if (ImGui::Checkbox("Wander", &useWander))
	//	{
	//		wanderBehavior->SetActive(useWander);
	//	}
	//	auto ObsAvoidBehavior = steeringModule->GetBehavior<ObstacleAvoidanceBehavior>("ObsAvoid");
	//	useObstacleAvoidance = ObsAvoidBehavior->IsActive();
	//	if (ImGui::Checkbox("Obstacle Avoidance", &useObstacleAvoidance))
	//	{
	//		ObsAvoidBehavior->SetActive(useObstacleAvoidance);
	//	}
	//}
}

void Dude::SetWeight(std::string behaviorName, float weight)
{
	steeringModule->GetBehavior<SteeringBehavior>(behaviorName)->SetWeight(weight);
}
