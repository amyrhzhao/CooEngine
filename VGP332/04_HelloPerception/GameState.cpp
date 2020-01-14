#include "GameState.h"

static int obstacleNum = 20;
static float obstacleMin = 20.0f;
static float obstacleMax = 40.0f;

using namespace Coo::Input;

void GameState::Initialize()
{
	Coo::Graphics::GraphicsSystem::Get()->SetClearColor(Coo::Graphics::Colors::Black);
	GenrateRandomObstacles();
	Coo::Graphics::SpriteRenderer::StaticInitialize();
	mDude = std::make_unique<Dude>(mAIWorld);
	mDude->Initialize();
	// Initialize meat
}

void GameState::Terminate()
{
	Coo::Graphics::SpriteRenderer::StaticTerminate();
	Coo::Graphics::TextureManager::Get()->FlushInventory();
}


void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::ESCAPE)) { CooApp::ShutDown(); }
	mAIWorld.Update();
	mDude->Update(deltaTime);
}

void GameState::Render()
{
	auto sr = Coo::Graphics::SpriteRenderer::Get();
	Coo::Graphics::SimpleDraw::Render(mCamera);
	mAIWorld.DebugRender();
	sr->BeginRender();
	mDude->Render();
	sr->EndRender();
}

void GameState::DebugUI()
{
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