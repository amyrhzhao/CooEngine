#include "GameState.h"

using namespace Coo::Input;
using namespace Coo::Math;
using namespace Coo::Graphics;

namespace 
{
	int iteration;
	float rot;
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition({ 0.0f,0.0f,-10.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });
	mMesh = MeshBuilder::CreateSphere(5.0f, 512, 512);
	mSpaceMesh = MeshBuilder::CreateSpherePX(400.0f);
	mScreenMesh = MeshBuilder::CreateNDCQuad();
	mMeshBuffer.Initialize(mMesh);
	mSpaceMeshBuffer.Initialize(mSpaceMesh);
	mScreenMeshBuffer.Initialize(mScreenMesh);

	mVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");
	mSpaceVertexShader.Initialize(L"../../Assets/Shaders/Texturing.fx", VertexPX::Format);
	mSpacePixelShader.Initialize(L"../../Assets/Shaders/Texturing.fx");

	/// Post processing effect( negative, grey scale, pixel)
	mPostProcessingVertexShader.Initialize(L"../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize(L"../../Assets/Shaders/PostProcessing.fx");

	/// Crt effect(scan line)
	mCrtEffectVertexShader.Initialize(L"../../Assets/Shaders/CrtEffect.fx", VertexPX::Format);
	mCrtEffectPixelShader.Initialize(L"../../Assets/Shaders/CrtEffect.fx");

	/// Blur effect
	mBlurEffectVertexShader.Initialize(L"../../Assets/Shaders/BlurEffect.fx", VertexPX::Format);
	mBlurEffectPixelShader.Initialize(L"../../Assets/Shaders/BlurEffect.fx");

	/// Brightness and Contrast
	mFragmentVertexShader.Initialize(L"../../Assets/Shaders/Fragment.fx", VertexPX::Format);
	mFragmentPixelShader.Initialize(L"../../Assets/Shaders/Fragment.fx");

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
	mFragmentConstantBuffer.Initialize();
	mBlurConstantBuffer.Initialize();

	mLight.direction = Coo::Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 0.1f, 0.1f, 0.1f, 1.0f };

	mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mMaterial.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
	mMaterial.power = 10.0f;

	mOptions.displacementWeight = 0.3f;

	auto graphicsSystem = GraphicsSystem::Get();
	for (int i = 0; i < 2; ++i)
	{
		mRenderTargets[i].Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	}
}

void GameState::Terminate()
{
	mOptionsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mPostProcessingPixelShader.Terminate();
	mCrtEffectVertexShader.Terminate();
	mCrtEffectPixelShader.Terminate();
	mBlurEffectVertexShader.Terminate();
	mBlurEffectPixelShader.Terminate();
	mFragmentVertexShader.Terminate();
	mFragmentPixelShader.Terminate();
	mFragmentConstantBuffer.Terminate();
	mMeshBuffer.Terminate();
	mSpaceMeshBuffer.Terminate();
	mScreenMeshBuffer.Terminate();
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
	rot += 0.15f * deltaTime;
	auto gs = GraphicsSystem::Get();
	mBlurData.resolution = { static_cast<float>(gs->GetBackBufferWidth()),static_cast<float>( gs->GetBackBufferHeight()) };
}

void GameState::Render()
{
	iteration = 0;

	mRenderTargets[iteration].BeginRender();
	RenderScene();
	mRenderTargets[iteration].EndRender();
	iteration = (iteration + 1) % 2;

	if (applyPixel)
	{
		mRenderTargets[iteration].BeginRender();
		PostProcess(iteration, 1);
		mRenderTargets[iteration].EndRender();
		iteration = (iteration + 1) % 2;
	}

	if (applyBlur)
	{
		for (int i = 0; i < blurPower; ++i)
		{
			mRenderTargets[iteration].BeginRender();
			PostProcess(iteration, 3);
			mRenderTargets[iteration].EndRender();
			iteration = (iteration + 1) % 2;

			mRenderTargets[iteration].BeginRender();
			PostProcess(iteration, 4);
			mRenderTargets[iteration].EndRender();
			iteration = (iteration + 1) % 2;
		}

	}

	if (applyCrt)
	{
		mRenderTargets[iteration].BeginRender();
		PostProcess(iteration, 2);
		mRenderTargets[iteration].EndRender();
		iteration = (iteration + 1) % 2;
	}

	PostProcess(iteration, 0);
}

void GameState::DebugUI()
{
	//ImGui::Begin("Scene");
	//ImGui::Image(mRenderTargets[iteration % 2 ? 0: 1].GetShaderResourceView(), {1280.0f * 0.7f, 720.0f * 0.7f});
	//ImGui::End();

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
		if (ImGui::Checkbox("UseNormal##Options", &useNormal))
		{
			mOptions.useNormal = (useNormal ? 1.0f : 0.0f);
		}
	}
	if (ImGui::CollapsingHeader("Transform"))
	{
		// position + rotation + reset
	}
	if (ImGui::CollapsingHeader("Post Processing"))
	{
		ImGui::Checkbox("Pixel Effect", &applyPixel);
		ImGui::Checkbox("CRT Effect", &applyCrt);
		ImGui::Checkbox("Blur Effect", &applyBlur);
		ImGui::DragInt("Blur Power", &blurPower, 0.1f, 0, 10);
		ImGui::DragFloat("Brightness", &mFragmentData.brightness, 0.001f, -5.0f, 5.0f);
		ImGui::DragFloat("Contrast", &mFragmentData.contrast, 0.001f, -5.0f, 5.0f);
	}
	ImGui::End();
}

void GameState::RenderScene()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	auto tm = Coo::Graphics::TextureManager::Get();
	tm->BindPS(mDiffuseMap, 0);
	tm->BindPS(mSpecularMap, 1);
	tm->BindVS(mDisplacementMap, 2);
	tm->BindPS(mNormalMap, 3);
	Coo::Graphics::SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();

	auto world = Coo::Math::Translate(mPosition) * Coo::Math::RotationY(rot);
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
		auto rs = RasterizerStateManager::Get()->GetRasterizerState("CullFrontSolid");
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

void GameState::PostProcess(int iteration, int shader)
{
	if (shader == 1)
	{
		mPostProcessingVertexShader.Bind();
		mPostProcessingPixelShader.Bind();
	}
	else if (shader == 2)
	{
		mCrtEffectVertexShader.Bind();
		mCrtEffectPixelShader.Bind();
	}
	else if (shader == 3)
	{
		mBlurData.blurDir = { 1.0f,0.0f };
		mBlurConstantBuffer.Set(mBlurData);
		mBlurConstantBuffer.BindPS(0);
		mBlurEffectVertexShader.Bind();
		mBlurEffectPixelShader.Bind();
	}
	else if (shader == 4)
	{
		mBlurData.blurDir = { 0.0f,1.0f };
		mBlurConstantBuffer.Set(mBlurData);
		mBlurConstantBuffer.BindPS(0);
		mBlurEffectVertexShader.Bind();
		mBlurEffectPixelShader.Bind();
	}
	else
	{
		mFragmentConstantBuffer.Set(mFragmentData);
		mFragmentConstantBuffer.BindPS(0);
		mFragmentVertexShader.Bind();
		mFragmentPixelShader.Bind();
	}

	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);

	mRenderTargets[iteration % 2 ? 0 : 1].BindPS(0);
	mScreenMeshBuffer.Render();
	mRenderTargets[iteration % 2 ? 0 : 1].UnbindPS(0);
}

