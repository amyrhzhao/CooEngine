#include "GameState.h"

using namespace Coo::Input;
using namespace Coo::Math;
using namespace Coo::Graphics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::ForestGreen);
	mTankPosition = { 0.0f, 3.5f, 0.0f };

	mDefaultCamera.SetPosition({ 0.0f,50.0f,-200.0f });
	mDefaultCamera.SetDirection({ 0.0f,0.0f,1.0f });
	
	mLightCamera.SetDirection(Normalize({ 1.0f, -1.0f, 1.0f }));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetFOV(1.0f);
	mLightCamera.SetAspectRatio(1.0f);
	mActiveCamera = &mDefaultCamera;

	ObjLoader::Load("../../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(mTankMesh);
	
	mPlaneMesh = MeshBuilder::CreatePlane(100.0f, 100.0f);
	mPlaneMeshBuffer.Initialize(mPlaneMesh);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionsBuffer.Initialize();

	mLight.direction = Coo::Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mLight.ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mMaterial.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
	mMaterial.power = 10.0f;
	
	mOptions.displacementWeight = 0.0f;
	mPosition = Coo::Math::Vector3::Zero();
	
	auto tm = TextureManager::Get();
	mVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");
	diffuseMap = tm->LoadTexture("../../Assets/Models/Tank/tank_diffuse.jpg",false);
	specularMap = tm->LoadTexture("../../Assets/Models/Tank/tank_specular.jpg",false);
	normalMap = tm->LoadTexture("../../Assets/Models/Tank/tank_normal.jpg",false);
	aoMap = tm->LoadTexture("../../Assets/Models/Tank/tank_ao.jpg", false);
	mGroundDiffuseMap = tm->LoadTexture("grass_2048.jpg");

	constexpr uint32_t depthMapSize = 1024;
	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, RenderTarget::Format::RGBA_U32);
	mDepthMapVertexShader.Initialize(L"../../Assets/Shaders/DepthMap.fx", Vertex::Format);
	mDepthMapPixelShader.Initialize(L"../../Assets/Shaders/DepthMap.fx");
	mDepthMapConstantBuffer.Initialize();
	mShadowConstantBuffer.Initialize();

}

void GameState::Terminate()
{
	mShadowConstantBuffer.Terminate();
	mDepthMapConstantBuffer.Terminate();
	mDepthMapPixelShader.Terminate();
	mDepthMapVertexShader.Terminate();
	mDepthMapRenderTarget.Terminate();
	mOptionsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mPlaneMeshBuffer.Terminate();
	mTankMeshBuffer.Terminate();
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
	if (inputSystem->IsKeyDown(KeyCode::W)) { mDefaultCamera.Walk(mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::S)) { mDefaultCamera.Walk(-mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::A)) { mDefaultCamera.Strafe(mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::D)) { mDefaultCamera.Strafe(-mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::Q)) { mDefaultCamera.Rise(mMoveSpeed * deltaTime); }
	if (inputSystem->IsKeyDown(KeyCode::E)) { mDefaultCamera.Rise(-mMoveSpeed * deltaTime); }
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mDefaultCamera.Yaw(inputSystem->GetMouseMoveX() * mTurnSpeed * deltaTime);
		mDefaultCamera.Pitch(inputSystem->GetMouseMoveY() * mTurnSpeed * deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::UP))
		mTankPosition.x += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
		mTankPosition.x -= deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
		mTankRotation.y += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		mTankRotation.y -= deltaTime;

	mLightCamera.SetPosition(mTankPosition - mLightCamera.GetDirection() * 50.0f);
}

void GameState::Render()
{
	mDepthMapRenderTarget.BeginRender();
	DrawDepthMap();
	mDepthMapRenderTarget.EndRender();

	DrawScene();
}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool lightCamera = mActiveCamera == &mLightCamera;
		if (ImGui::Checkbox("Use Light Camera", &lightCamera))
		{
			mActiveCamera = lightCamera ? &mLightCamera : &mDefaultCamera;
		}

		ImGui::Image(
			mDepthMapRenderTarget.GetShaderResourceView(),
			{ 150.0f, 150.0f },
			{ 0.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mLight.direction.x, 0.1f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mLight.direction.y, 0.1f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mLight.direction.z, 0.1f, -1.0f, 1.0f);
		if (directionChanged)
		{
			mLight.direction = Coo::Math::Normalize(mLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 0.1f, 0.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Options"))
	{
		ImGui::SliderFloat("Weight##Displacement", &mOptions.displacementWeight, 0.0f, 1.0f);
		bool useNormal = (mOptions.useNormal > 0.0f);
		if (ImGui::Checkbox("UseNormal##Options", &useNormal))
		{
			mOptions.useNormal = (useNormal ? 1.0f : 0.0f);
		}
		ImGui::SliderFloat("AmbientOcclusionPower", &mOptions.aoPower, 0.0f, 10.0f);
		bool useShadow = (mOptions.useShadow > 0.0f);
		if (ImGui::Checkbox("UseShadow##Options", &useShadow)) 
		{
			mOptions.useShadow = (useShadow ? 1.0f : 0.0f);
		}
		ImGui::SliderFloat("DepthBias", &mOptions.depthBias, 0.0f, 0.01f, "%.4f");
	}
	if (ImGui::CollapsingHeader("Transform"))
	{
		// position + rotation + reset
	}
	ImGui::End();
}

void GameState::DrawScene()
{
	auto matView = mActiveCamera->GetViewMatrix();
	auto matProj = mActiveCamera->GetPerspectiveMatrix();
	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();

	mLightBuffer.Set(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Set(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mOptionsBuffer.Set(mOptions);
	mOptionsBuffer.BindVS(3);
	mOptionsBuffer.BindPS(3);

	Coo::Graphics::SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();

	auto tm = TextureManager::Get();
	tm->BindPS(diffuseMap, 0);
	tm->BindPS(specularMap, 1);
	tm->BindPS(normalMap, 3);
	tm->BindPS(aoMap, 4);
	mDepthMapRenderTarget.BindPS(5);

	Matrix4 matTrans = Translate(mTankPosition);
	Matrix4 matRot = RotationX(mTankRotation.x) * RotationY(mTankRotation.y);
	Matrix4 matWorld = matRot * matTrans;

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mActiveCamera->GetPosition();
	mTransformBuffer.Set(transformData);
	mTransformBuffer.BindVS(0);

	auto wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Set(wvpLight);
	mShadowConstantBuffer.BindVS(4);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTankMeshBuffer.Render();

	matWorld = Matrix4::Identity();
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Set(transformData);

	wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Set(wvpLight);
	mShadowConstantBuffer.BindVS(4);

	tm->BindPS(mGroundDiffuseMap, 0);

	OptionsData options;
	options.displacementWeight = 0.0f;
	options.useNormal = 0.0f;
	options.aoPower = 0.0f;
	options.useShadow = 1.0f;
	options.depthBias = 0.0f;
	mOptionsBuffer.Set(options);

	mPlaneMeshBuffer.Render();

	SimpleDraw::Render(*mActiveCamera);
}

void GameState::DrawDepthMap()
{
	mDepthMapVertexShader.Bind();
	mDepthMapPixelShader.Bind();

	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();

	Matrix4 matTrans = Translate(mTankPosition);
	Matrix4 matRot = RotationX(mTankRotation.x) * RotationY(mTankRotation.y);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 wvp = Transpose(matWorld * matViewLight * matProjLight);
	mDepthMapConstantBuffer.Set(wvp);
	mDepthMapConstantBuffer.BindVS(0);

	mTankMeshBuffer.Render();
}