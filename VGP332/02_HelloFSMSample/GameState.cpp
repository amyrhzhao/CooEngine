#include "GameState.h"

using namespace Coo::Graphics;
void GameState::Initialize() 
{
	mDude.Initialize();
	mDude.stateMachine->AddState<RunState>();
	mDude.stateMachine->AddState<DanceState>();
	auto tm = TextureManager::Get();
	//tm->SetRootPath(L"../../Assets/Images/");
	mTextureId = tm->LoadTexture(L"stone.png");
	SpriteRenderer::StaticInitialize();
}

void GameState::Terminate() 
{
	SpriteRenderer::StaticTerminate();
}

void GameState::Update(float deltaTime) 
{
	mDude.stateMachine->Update(deltaTime);
}

void GameState::Render() 
{
	SimpleDraw::Render(mCamera);
	auto sr = SpriteRenderer::Get();
	auto tm = TextureManager::Get();
	sr->BeginRender();
	auto texture = tm->GetTexture(mTextureId);
	sr->Draw(*texture, mDude.position);
	sr->EndRender();
}

void GameState::DebugUI() 
{

}