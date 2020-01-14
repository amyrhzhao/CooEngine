#include "GameState.h"

void GameState::Initialize()
{
	// make 10 random obstacles
	auto graphicSystem = Coo::Graphics::GraphicsSystem::Get();
	auto screenWidth = static_cast<float>(graphicSystem->GetBackBufferWidth());
	auto screenHeight = static_cast<float>(graphicSystem->GetBackBufferHeight());
	for (int i = 0; i < 10; ++i)
	{
		mAIWorld.AddObstacle(Coo::Math::Circle{ { Coo::Math::RandomFloat(0.0f, screenWidth), Coo::Math::RandomFloat(0.0f, screenHeight) }, Coo::Math::RandomFloat(10.0f, 40.0f) });
	}

	mDog = std::make_unique<Dog>(mAIWorld);
	mDog->Initialize();

	mOwner = std::make_unique<Owner>(mAIWorld);
	mOwner->Initialize();
	
	mBone = std::make_unique<Bone>();
	mBone->Initialize();
	mDog->owner = mOwner.get();

	Coo::Graphics::SpriteRenderer::StaticInitialize();
}

void GameState::Terminate()
{
	Coo::Graphics::SpriteRenderer::StaticTerminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = Coo::Input::InputSystem::Get();
	if (inputSystem->IsKeyPressed(Coo::Input::KeyCode::ESCAPE))
	{
		CooApp::ShutDown();
	}
	if (inputSystem->IsMousePressed(Coo::Input::MouseButton::LBUTTON))
	{
		auto screenX = static_cast<float>(inputSystem->GetMouseScreenX());
		auto screenY = static_cast<float>(inputSystem->GetMouseScreenY());
		if (mBone->Spawn({ screenX, screenY })) 
		{
			mDog->FetchBone(*(mBone.get()));
		}
	}
	mDog->Update(deltaTime);
	mOwner->Update(deltaTime);

}

void GameState::Render()
{
	Coo::Graphics::SpriteRenderer::Get()->BeginRender();
	mDog->Render();
	mOwner->Render();
	mBone->Render();
	auto mObstacles = mAIWorld.GetObstacles();
	for (auto& obstacle : mObstacles) 
	{
		Coo::Graphics::SimpleDraw::AddScreenCircle(obstacle.center, obstacle.radius, Coo::Graphics::Colors::AliceBlue);
	}
	Coo::Graphics::SimpleDraw::Render(mCamera);
	Coo::Graphics::SpriteRenderer::Get()->EndRender();
}

void GameState::DebugUI()
{

}