#include "GameState.h"

using namespace Coo::Input;
using namespace Coo::Math;
using namespace Coo::Graphics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition({ 0.0f,0.0f,-10.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });
	mMesh = MeshBuilder::CreateSphere(5.0f,512,512);
	mSpaceMesh = MeshBuilder::CreateSpherePX(400.0f);
	mMeshBuffer.Initialize(mMesh);
	mSpaceMeshBuffer.Initialize(mSpaceMesh);

	mVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");
	mSpaceVertexShader.Initialize(L"../../Assets/Shaders/Texturing.fx", VertexPX::Format);
	mSpacePixelShader.Initialize(L"../../Assets/Shaders/Texturing.fx");
	auto tm = Coo::Graphics::TextureManager::Get();
	mDiffuseMap = tm->LoadTexture("EarthTexture.jpg");
	//mDiffuseMap = tm->LoadTexture("earth.jpg");
	mSpecularMap = tm->LoadTexture("EarthSpecularMap.jpg");
	mDisplacementMap = tm->LoadTexture("EarthDisplacementMap.jpg");
	mNormalMap = tm->LoadTexture("earth_normal.jpg");
	mSpaceTexture = tm->LoadTexture("StarUV.jpg");

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionsBuffer.Initialize();
	mSpaceConstantBuffer.Initialize(3, sizeof(Matrix4));

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
	mMeshBuffer.Terminate();
	mSpaceMeshBuffer.Terminate();
	mSpaceVertexShader.Terminate();
	mSpacePixelShader.Terminate();
	mSpaceConstantBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
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
}

void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	auto tm = Coo::Graphics::TextureManager::Get();
	tm->BindPS(mDiffuseMap,0);
	tm->BindPS(mSpecularMap,1);
	tm->BindVS(mDisplacementMap,2);
	tm->BindPS(mNormalMap, 3);
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

	mMeshBuffer.Render();

	{
		auto rs =  RasterizerStateManager::Get()->GetRasterizerState("CullFrontSolid");
		rs->Set();
		mSpaceVertexShader.Bind();
		mSpacePixelShader.Bind();
		Matrix4 matrix[3];
		matrix[0] = Coo::Math::Transpose(Translate(mCamera.GetPosition()));
		matrix[1] = Coo::Math::Transpose(mCamera.GetViewMatrix());
		matrix[2] = Coo::Math::Transpose(mCamera.GetPerspectiveMatrix());
		mSpaceConstantBuffer.Set(matrix);
		mSpaceConstantBuffer.BindVS();
		tm->BindPS(mSpaceTexture, 0);
		mSpaceMeshBuffer.Render();
		rs->Clear();
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	
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
		ImGui::DragFloat("Weight##Displacement", &mOptions.displacementWeight, 0.001f, 0.0f, 1.0f);
		bool useNormal = (mOptions.useNormal > 0.0f);
		if(ImGui::Checkbox("UseNormal##Options", &useNormal))
		{
			mOptions.useNormal = (useNormal ? 1.0f : 0.0f);
		}
	}
	if(ImGui::CollapsingHeader("Transform"))
	{
		// position + rotation + reset
	}
	ImGui::End();
}

