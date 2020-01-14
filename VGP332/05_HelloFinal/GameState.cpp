#include "GameState.h"
#include "SheepManager.h"
#include "GrassManager.h"

using namespace Coo;
using namespace Coo::Graphics;
using namespace Coo::Input;

void GameState::Initialize() 
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	SheepManager::StaticInitialize();
	SheepManager::Get()->Initialize(mAIWorld);
	GrassManager::StaticInitialize();
	GrassManager::Get()->Initialize(mAIWorld);
}

void GameState::Terminate() 
{
	SheepManager::StaticTerminate();
	GrassManager::StaticTerminate();
}

void GameState::Update(float deltaTime) 
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::ESCAPE)) { CooApp::ShutDown(); }
	SheepManager::Get()->Update(deltaTime);
	GrassManager::Get()->Update();
}

void GameState::Render() 
{
	SheepManager::Get()->Render();
	GrassManager::Get()->Render();
	Coo::Graphics::SimpleDraw::AddScreenText("Hello", 100.0f, 100.0f, 14.0f, Colors::Cyan);
	Coo::Graphics::SimpleDraw::Render(mCamera);
	
}

void GameState::DebugUI() 
{
}