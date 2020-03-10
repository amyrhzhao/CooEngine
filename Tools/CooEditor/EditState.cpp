#include "EditState.h"

using namespace Coo::Input;
using namespace Coo::Math;
using namespace Coo::Graphics;

namespace 
{
	int iteration;
}

static void ShowExampleMenuFile()
{
	ImGui::MenuItem("(dummy menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		static bool b = true;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::Checkbox("Check", &b);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Colors"))
	{
		float sz = ImGui::GetTextLineHeight();
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
			ImGui::Dummy(ImVec2(sz, sz));
			ImGui::SameLine();
			ImGui::MenuItem(name);
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

void EditState::Initialize()
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

	mWorld.Initialize(10000);
	mWorld.Create("../../Assets/Template/test.json", "TestObject");
}

void EditState::Terminate()
{
	mWorld.Terminate();
	mOptionsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mFragmentConstantBuffer.Terminate();
	mMeshBuffer.Terminate();
	mSpaceMeshBuffer.Terminate();
	mScreenMeshBuffer.Terminate();
	mSpaceVertexShader.Terminate();
	mSpacePixelShader.Terminate();
	mSpaceConstantBuffer.Terminate();
}

void EditState::Update(float deltaTime)
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
	auto gs = GraphicsSystem::Get();
	mBlurData.resolution = { static_cast<float>(gs->GetBackBufferWidth()),static_cast<float>(gs->GetBackBufferHeight()) };
}

void EditState::Render()
{
	iteration = 0;

	mRenderTargets[iteration].BeginRender();
	RenderScene();
	mWorld.Render();
	SimpleDraw::Render(mCamera);
	mRenderTargets[iteration].EndRender();
	iteration = (iteration + 1) % 2;

	PostProcess(iteration, 0);
}

void EditState::DebugUI()
{
	//ImGui::Begin("Scene");
	//ImGui::Image(mRenderTargets[iteration % 2 ? 0: 1].GetShaderResourceView(), {1280.0f * 0.7f, 720.0f * 0.7f});
	//ImGui::End();
	DrawMenuBar();
	ShowMainWindowWithDockSpace();
	ShowSceneView();
	mEditor.ShowHierarchyView();
	mEditor.ShowInspectorView();
	//ShowSettings();
}

void EditState::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void EditState::ShowMainWindowWithDockSpace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Main", nullptr, window_flags);
	ImGui::DockSpace(ImGui::GetID("MyDockingSpace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	ImGui::End();

	ImGui::PopStyleVar(3);
}

void EditState::ShowSceneView()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene");
	ImGui::PopStyleVar(1);
	
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();
	vMin.x += ImGui::GetWindowPos().x;
	vMin.y += ImGui::GetWindowPos().y;
	vMax.x += ImGui::GetWindowPos().x;
	vMax.y += ImGui::GetWindowPos().y;

	ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));
	ImGui::Image(mRenderTargets[iteration % 2 ? 0 : 1].GetShaderResourceView(), { vMax.x - vMin.x, vMax.y - vMin.y });
	ImGui::CaptureMouseFromApp(!ImGui::IsItemHovered());
	ImGui::End();
}

void EditState::ShowSettings()
{
	//ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Begin("Setting");
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

void EditState::RenderScene()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	auto tm = Coo::Graphics::TextureManager::Get();
	tm->BindPS(mDiffuseMap, 0);
	tm->BindPS(mSpecularMap, 1);
	tm->BindVS(mDisplacementMap, 2);
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

void EditState::PostProcess(int iteration, int shader)
{
	SamplerManager::Get()->GetSampler("PointWrap")->BindPS(0);

	mRenderTargets[iteration % 2 ? 0 : 1].BindPS(0);
	mScreenMeshBuffer.Render();
	mRenderTargets[iteration % 2 ? 0 : 1].UnbindPS(0);
}

