#include "GameState.h"

using namespace Coo::Input;
using namespace Coo::Math;
using namespace Coo::Graphics;

namespace
{
	int currAnimClip;
}

void GameState::Initialize()
{
	//load model
	model.Load("../../Assets/Models/ybot/ybot.model");
	model.mMaterialData[0].textureData[0].textureName = "../../Assets/Images/UranusUV.jpg";
	model.mMaterialData[1].textureData[0].textureName = "../../Assets/Images/VenusUV.jpg";
	model.Initialize();
	skeleton.Load("../../Assets/Models/ybot/ybot.skeleton");
	skeleton.Initialize();
	animationBank.Load("../../Assets/Models/ybot/bboyUprock.animation", true);
	animationBank.Load("../../Assets/Models/ybot/dancing.animation", true);
	animationBank.Load("../../Assets/Models/ybot/hipHopDancing.animation", true);
	animationBank.Load("../../Assets/Models/ybot/walking.animation", true);
	animationBank.Load("../../Assets/Models/ybot/running.animation", true);
	animator.Bind(&animationBank);

	GraphicsSystem::Get()->SetClearColor(Colors::Green);
	mCamera.SetPosition({ 0.0f,100.0f,-200.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	auto tm = TextureManager::Get();
	mVertexShader.Initialize(L"../../Assets/Shaders/Skinning.fx", BoneVertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Skinning.fx");

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionsBuffer.Initialize();

	mLight.direction = Coo::Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 0.1f, 0.1f, 0.1f, 1.0f };

	mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mMaterial.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
	mMaterial.power = 10.0f;

	mOptions.displacementWeight = 0.3f;
}

void GameState::Terminate()
{
	mOptionsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	skeleton.Terminate();
	model.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::LSHIFT)) 
	{
		mMoveSpeed = 100.0f;
	}
	else
	{
		mMoveSpeed = 10.0f;
	}
	if (inputSystem->IsKeyDown(KeyCode::ESCAPE)) { CooApp::ShutDown(); }
	// Camera transation
	if (inputSystem->IsKeyDown(KeyCode::W)) { mCamera.Walk(mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::S)) { mCamera.Walk(-mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::A)) { mCamera.Strafe(mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::D)) { mCamera.Strafe(-mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::Q)) { mCamera.Rise(mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::E)) { mCamera.Rise(-mMoveSpeed * deltaTime); }
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * mTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * mTurnSpeed * deltaTime);
	}
	animator.UpdateAnimation(deltaTime);
}

void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	Coo::Graphics::SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();

	auto world = Coo::Math::Translate(mPosition);
	auto view = mCamera.GetViewMatrix();
	auto proj = mCamera.GetPerspectiveMatrix();
	TransformData transformData;
	transformData.wvp = Coo::Math::Transpose(world*view*proj);
	transformData.world = Coo::Math::Transpose(world);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Set(transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mLightBuffer.Set(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Set(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mOptionsBuffer.Set(mOptions);
	mOptionsBuffer.BindVS(3);
	mOptionsBuffer.BindPS(3);

	skeleton.Bind(&animator);

	auto rs = RasterizerStateManager::Get()->GetRasterizerState("CullNoneSolid");
	//auto rs = RasterizerStateManager::Get()->GetRasterizerState("CullFrontSolid");
	//auto rs = RasterizerStateManager::Get()->GetRasterizerState("Wireframe");
	rs->Set();
	model.Render();
	rs->Clear();
}

void GameState::DebugUI()
{
	//ImGui::Begin("Scene Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//if (ImGui::CollapsingHeader("Light"))
	//{
	//	bool directionChanged = false;
	//	directionChanged |= ImGui::DragFloat("Direction X##Light", &mLight.direction.x, 0.1f, -1.0f, 1.0f);
	//	directionChanged |= ImGui::DragFloat("Direction Y##Light", &mLight.direction.y, 0.1f, -1.0f, 1.0f);
	//	directionChanged |= ImGui::DragFloat("Direction Z##Light", &mLight.direction.z, 0.1f, -1.0f, 1.0f);
	//	if (directionChanged)
	//	{
	//		mLight.direction = Coo::Math::Normalize(mLight.direction);
	//	}
	//	ImGui::ColorEdit4("Ambient##Light", &mLight.ambient.r);
	//	ImGui::ColorEdit4("Diffuse##Light", &mLight.diffuse.r);
	//	ImGui::ColorEdit4("Specular##Light", &mLight.specular.r);
	//}
	//if (ImGui::CollapsingHeader("Material"))
	//{
	//	ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
	//	ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
	//	ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
	//	ImGui::DragFloat("Power##Material", &mMaterial.power, 0.1f, 0.0f, 100.0f);
	//}
	//if (ImGui::CollapsingHeader("Options"))
	//{
	//	ImGui::DragFloat("Weight##Displacement", &mOptions.displacementWeight, 0.001f, 0.0f, 1.0f);
	//	bool useNormal = (mOptions.useNormal > 0.0f);
	//	if (ImGui::Checkbox("UseNormal##Options", &useNormal))
	//	{
	//		mOptions.useNormal = (useNormal ? 1.0f : 0.0f);
	//	}
	//}

	//ImGui::End();

	// Animator
	animator.DebugUI();
	
	// Skeleton
	//skeleton.DebugUI();
}

