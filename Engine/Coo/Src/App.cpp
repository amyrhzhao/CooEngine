#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace Coo;
using namespace Coo::Input;
using namespace Coo::Graphics;

void App::ChangeState(std::string name)
{
	if (auto iter = mAppStates.find(name); iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}

void App::Run(AppConfig appConfig)
{
	mAppConfig = appConfig;

	// Set up application window
	mWindow.Initialize(
		GetModuleHandle(NULL),
		mAppConfig.appName.c_str(),
		mAppConfig.windowWidth,
		mAppConfig.windowHeight);

	// Initialize the input system
	InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	// Initialize graphic system
	GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	SimpleDraw::StaticInitialize();
	Dui::StaticInitialize(mWindow.GetWindowHandle(),true,true);

	// Initialize the texture manager
	Graphics::TextureManager::StaticInitialize(appConfig.assetsDirectory / "Images");

	// Initialize the sampler manager
	Graphics::SamplerManager::StaticInitialize();

	// Initialize the blend state manager
	Graphics::BlendStateManager::StaticInitialize();

	// Initialize the rasterizer state manager
	Graphics::RasterizerStateManager::StaticInitialize();

	// Initialize the starting state
	mCurrentState = mAppStates.begin()->second.get();
	mCurrentState->Initialize();

	// Loop 
	bool done = false;
	while (!done)
	{
		done = mWindow.ProcessMessage();
		if (done) { break; }

		if (mNextState)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();			
		}

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
		lastTime = currentTime;

		mCurrentState->Update(deltaTime);

		GraphicsSystem::Get()->BeginRender();

		mCurrentState->Render();

		Dui::BeginRender();
		mCurrentState->DebugUI();
		Dui::EndRender();

		GraphicsSystem::Get()->EndRender();
	}

	// Shut
	mCurrentState->Terminate();

	RasterizerStateManager::StaticTerminate();
	BlendStateManager::StaticTerminate();
	SamplerManager::StaticTerminate();
	TextureManager::StaticTerminate();
	Dui::StaticTerminate();
	SimpleDraw::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();

	mWindow.Terminate();
}
