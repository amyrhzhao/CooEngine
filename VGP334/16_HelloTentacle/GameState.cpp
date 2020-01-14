#include "GameState.h"

const int initArmNum = 50;
const float height = 10.0f;
const float rotateSpeed = 0.2f;

using namespace Coo::Input;
using namespace Coo::Math;
using namespace Coo::Graphics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Cyan);
	mCamera.SetPosition({ 0.0f,5.0f,-10.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	auto tm = TextureManager::Get();
	mVertexShader.Initialize(L"../../Assets/Shaders/Skinning.fx", BoneVertex::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Skinning.fx");
	mTextureId = tm->LoadTexture("sponge_bob.png");

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionsBuffer.Initialize();
	mBoneTransformBuffer.Initialize();

	mLight.direction = Coo::Math::Normalize({ 0.0f, 0.0f, 1.0f });
	mLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	mLight.specular = { 0.1f, 0.1f, 0.1f, 1.0f };

	mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mMaterial.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
	mMaterial.power = 10.0f;

	mOptions.displacementWeight = 0.3f;

	rootPosition = { 0.0f,0.0f,0.0f };
	mMesh = MeshBuilder::CreateTentacle(initArmNum, 0.1f, height, 32);
	mMeshBuffer.Initialize(mMesh);
	mBones.resize(initArmNum);
	mBoneRotations.resize(initArmNum);
	for (size_t i = 0; i < mBones.size(); ++i)
	{
		mBones[i].offsetTransform = Inverse(Coo::Math::Translate({ 0.0f,static_cast<float>(height / initArmNum) * i,0.0f }));
		mBoneRotations[i] = Quaternion::Identity();
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

	// Tentacle
	size_t select = selectedBone;

	if (inputSystem->IsKeyPressed(KeyCode::UP)) { select++; }
	else if (inputSystem->IsKeyPressed(KeyCode::DOWN)) { select--; }

	if (select < 0) { select = 0; }
	else if (select >= mBones.size()) { select = mBones.size() - 1; }
	else { selectedBone = select; }

	if (inputSystem->IsKeyDown(KeyCode::LEFT)) {
		mBoneRotations[selectedBone] =
			mBoneRotations[selectedBone]
			* QuaternionRotationAxis(Coo::Math::Vector3::XAxis(), rotateSpeed * deltaTime);
	}
	else if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mBoneRotations[selectedBone] =
			mBoneRotations[selectedBone]
			* QuaternionRotationAxis(Coo::Math::Vector3::XAxis(), -rotateSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::MINUS))
	{
		for (size_t i = 1; i < mBoneRotations.size(); ++i)
		{
			mBoneRotations[i] = mBoneRotations[i] * QuaternionRotationAxis(Coo::Math::Vector3::ZAxis(), rotateSpeed * deltaTime);
			mBoneRotations[i] = mBoneRotations[i] * QuaternionRotationAxis(Coo::Math::Vector3::XAxis(), rotateSpeed * deltaTime);
			mBoneRotations[i] = mBoneRotations[i] * QuaternionRotationAxis(Coo::Math::Vector3::YAxis(), rotateSpeed * deltaTime);
		}
	}
	else if (inputSystem->IsKeyDown(KeyCode::EQUALS))
	{
		for (size_t i = 1; i < mBoneRotations.size(); ++i)
		{
			mBoneRotations[i] = mBoneRotations[i] * QuaternionRotationAxis(-Coo::Math::Vector3::ZAxis(), rotateSpeed * deltaTime);
			mBoneRotations[i] = mBoneRotations[i] * QuaternionRotationAxis(-Coo::Math::Vector3::XAxis(), rotateSpeed * deltaTime);
			mBoneRotations[i] = mBoneRotations[i] * QuaternionRotationAxis(-Coo::Math::Vector3::YAxis(), rotateSpeed * deltaTime);
		}
	}

	mBones[0].toParentTransform = Coo::Math::MatrixRotationQuaternion(mBoneRotations[0]);
	for (size_t i = 1; i < mBones.size(); ++i)
	{
		mBones[i].toParentTransform =
			//mBones[i - 1].toParentTransform *
			//Coo::Math::Translate({ 0.0f, (static_cast<float>(height / initArmNum) * (i - 1)),0.0f });
			Coo::Math::MatrixRotationQuaternion(mBoneRotations[i]);
	}

}

void GameState::Render()
{
	// Draw ground plane
	const float planeSize = 30.0f;
	for (float i = -planeSize; i <= planeSize; i += 1.0f)
	{
		SimpleDraw::AddLine({ -planeSize, 0.0f, i }, { planeSize, 0.0f, i }, Colors::Gray);
		SimpleDraw::AddLine({ i, 0.0f, -planeSize }, { i, 0.0f, planeSize }, Colors::Gray);
	}
	SimpleDraw::Render(mCamera);

	mVertexShader.Bind();
	mPixelShader.Bind();
	Coo::Graphics::SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();

	mLightBuffer.Set(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Set(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mOptionsBuffer.Set(mOptions);
	mOptionsBuffer.BindVS(3);
	mOptionsBuffer.BindPS(3);

	mBoneTransform.boneTransforms[0] = mBones[0].toParentTransform;
	for (size_t i = 1; i < mBones.size(); ++i)
	{
		mBoneTransform.boneTransforms[i] =
			(mBones[i].offsetTransform)
			* mBones[i].toParentTransform
			* Inverse(mBones[i].offsetTransform)
			//* Coo::Math::Translate({ 0.0f, (static_cast<float>(height / initArmNum) * (i - 1)),0.0f })
			* mBoneTransform.boneTransforms[i - 1];
	}
	for (size_t i = 0; i < mBones.size(); ++i)
	{
		mBoneTransform.boneTransforms[i] = Transpose(mBoneTransform.boneTransforms[i]);
	}

	mBoneTransformBuffer.Set(mBoneTransform);
	mBoneTransformBuffer.BindVS(4);

	auto tm = TextureManager::Get();
	tm->BindPS(mTextureId, 0);

	// Render tentacle
	//RasterizerStateManager::Get()->GetRasterizerState("Wireframe")->Set();
	auto world = Coo::Math::Translate(rootPosition);
	auto view = mCamera.GetViewMatrix();
	auto proj = mCamera.GetPerspectiveMatrix();
	TransformData transformData;
	transformData.wvp = Coo::Math::Transpose(world*view*proj);
	transformData.world = Coo::Math::Transpose(world);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Set(transformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);
	mMeshBuffer.Render();
	//RasterizerStateManager::Get()->GetRasterizerState("Wireframe")->Clear();
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
		if (ImGui::Checkbox("UseNormal##Options", &useNormal))
		{
			mOptions.useNormal = (useNormal ? 1.0f : 0.0f);
		}
	}
	if (ImGui::CollapsingHeader("Transform"))
	{
		// position + rotation + reset
	}
	ImGui::End();
}

