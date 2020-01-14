#include "GameState.h"

const int obstacleNum = 10;
const float obstacleMin = 20.0f;
const float obstacleMax = 40.0f;
Coo::Math::Vector2 destination = { 0.0f, 0.0f };
const int agentNum = 200;

using namespace Coo::Input;
void GameState::Initialize()
{
	Coo::Graphics::GraphicsSystem::Get()->SetClearColor(Coo::Graphics::Colors::Black);
	GenrateRandomObstacles();
	Coo::Graphics::SpriteRenderer::StaticInitialize();
	
	for (int i = 0; i < agentNum; ++i) 
	{
		mDudes.emplace_back(std::make_unique<Dude>(mAIWorld));
		mDudes[i]->Initialize();
	}
	for (auto& dude : mDudes)
	{
		dude->SetWeight("Seek", seekWeight);
		dude->SetWeight("Wander", wanderWeight);
		dude->SetWeight("ObsAvoid", ObsAvoidWeight);
		dude->SetWeight("Speration", SperationWeight);
		dude->SetWeight("Alignment", AlignmentWeight);
		dude->SetWeight("Cohesion", CohesionWeight);
	}
}

void GameState::Terminate() 
{
	Coo::Graphics::SpriteRenderer::StaticTerminate();
	Coo::Graphics::TextureManager::Get()->FlushInventory();
}

void GameState::Update(float deltaTime)
{
	fps = 1.0f / deltaTime;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::ESCAPE)) { CooApp::ShutDown(); }
	mAIWorld.Update();
	for (auto& dude : mDudes)
	{
		dude->Update(deltaTime);
	}
}

void GameState::Render() 
{
	auto sr = Coo::Graphics::SpriteRenderer::Get();
	Coo::Graphics::SimpleDraw::Render(mCamera);
	mAIWorld.DebugRender();
	sr->BeginRender();
	for (auto& dude : mDudes) 
	{
		dude->Render();
	}
	sr->EndRender();
}

void GameState::DebugUI()
{
	ImGui::Begin("Steering Behaviors");
	ImGui::Text("fps: %f", fps);
	GroupBehaviorDebugUI();
	for (auto& dude : mDudes)
	{
		dude->DebugUI();
	}
	ImGui::End();
}

void GameState::GenrateRandomObstacles()
{
	auto graphicSystem = Coo::Graphics::GraphicsSystem::Get();
	auto screenWidth = static_cast<float>(graphicSystem->GetBackBufferWidth());
	auto screenHeight = static_cast<float>(graphicSystem->GetBackBufferHeight());
	for (int i = 0; i < obstacleNum; ++i) 
	{
		mAIWorld.AddObstacle(Coo::Math::Circle{ {Coo::Math::RandomFloat(0.0f, screenWidth), Coo::Math::RandomFloat(0.0f,screenHeight)}, Coo::Math::RandomFloat(obstacleMin, obstacleMax) });
	}

}

void GameState::GroupBehaviorDebugUI()
{
	if (ImGui::CollapsingHeader("Behaviors Weight")) 
	{
		if (ImGui::DragFloat("Seek Behavior", &seekWeight, 0.1f, 0.0f, 10.0f)) 
		{
			for (auto& dude : mDudes)
			{
				dude->SetWeight("Seek", seekWeight);
			}
		}
		if (ImGui::DragFloat("Wander Behavior", &wanderWeight, 0.1f, 0.0f, 10.0f))
		{
			for (auto& dude : mDudes)
			{
				dude->SetWeight("Wander", wanderWeight);
			}
		}
		if (ImGui::DragFloat("ObsAvoid", &ObsAvoidWeight, 0.1f, 0.0f, 10.0f))
		{
			for (auto& dude : mDudes)
			{
				dude->SetWeight("ObsAvoid", ObsAvoidWeight);
			}
		}
		
	}
	if (ImGui::CollapsingHeader("Group Behaviors Weight")) 
	{
		if (ImGui::DragFloat("Speration", &SperationWeight, 0.1f, 0.0f, 10.0f))
		{
			for (auto& dude : mDudes)
			{
				dude->SetWeight("Speration", SperationWeight);
			}
		}
		if (ImGui::DragFloat("Alignment", &AlignmentWeight, 0.1f, 0.0f, 10.0f))
		{
			for (auto& dude : mDudes)
			{
				dude->SetWeight("Alignment", AlignmentWeight);
			}
		}
		if (ImGui::DragFloat("Cohesion", &CohesionWeight, 0.1f, 0.0f, 10.0f))
		{
			for (auto& dude : mDudes)
			{
				dude->SetWeight("Cohesion", CohesionWeight);
			}
		}
	}
}
