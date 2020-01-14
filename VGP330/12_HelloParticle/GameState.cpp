#include "GmaeState.h"

using namespace Coo::Input;
using namespace Coo::Math;
using namespace Coo::Graphics;

namespace
{
	const char* blendItems[] = { "Opaque", "AlphaBlend", "AlphaPremultiplied", "Additive" };

	float maxLifeTime = 2.0f;

	struct Particle
	{
		Vector3 position;
		Vector3 velocity;
		Vector4 color;
		float lifeTime = 0.0f;
	};

	std::vector<Particle> particles(1000);
	int nextParticle = 0;

	void SpawnParticle()
	{
		auto& p = particles[nextParticle];
		p.position = Vector3{ 0.0f };
		p.velocity = Vector3{
			RandomFloat(-1.0f, 1.0f),
			RandomFloat(-1.0f, 1.0f),
			RandomFloat(-1.0f, 1.0f)
		};
		p.lifeTime = maxLifeTime;
		nextParticle = (nextParticle + 1) % particles.size();
	}

	void UpdateParticles(float deltaTime)
	{
		for (auto& p : particles)
		{
			if (p.lifeTime > 0.0f)
			{
				p.lifeTime -= deltaTime;
				p.velocity.y += deltaTime;
				p.position += p.velocity * deltaTime;
			}
		}
	}

	Vector4 startColor{ 1.0f, 0.0f, 0.0f, 1.0f };
	Vector4 endColor{ 0.8f, 0.8f, 0.0f, 1.0f };
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f, 5.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 2.0f, 0.0f });
	mConstantBuffer.Initialize();

	mVertexShader.Initialize(L"../../Assets/Shaders/Particle.fx", VertexPNC::Format);
	mGeometryShader.Initialize(L"../../Assets/Shaders/Particle.fx");
	mPixelShader.Initialize(L"../../Assets/Shaders/Particle.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mTexture.Initialize("../../Assets/Images/smoke-17.png");

	mMeshBuffer.Initialize(nullptr, static_cast<uint32_t>(sizeof(VertexPNC)), static_cast<uint32_t>(particles.size()), true);
}

void GameState::Terminate()
{
	mMeshBuffer.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();
	for (auto& b : mBlendStates)
		b.Terminate();
	mPixelShader.Terminate();
	mGeometryShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::ESCAPE)) { CooApp::ShutDown(); return; }

	auto moveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	auto turnSpeed = 0.2f;
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	else if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::SPACE) || true)
	{
		SpawnParticle();
	}

	UpdateParticles(deltaTime);
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

	// Draw Particles
	auto view = mCamera.GetViewMatrix();
	auto projection = mCamera.GetPerspectiveMatrix();
	
	ConstantData data;
	data.wvp = Transpose(view * projection);
	data.viewPosition = { mCamera.GetPosition(), 1.0f };
	data.viewUp = Vector3{ view._12, view._22, view._32 };
	
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindGS();
	mConstantBuffer.BindPS();
	mVertexShader.Bind();
	mGeometryShader.Bind();
	mPixelShader.Bind();
	
	BlendStateManager::Get()->GetBlendState(blendItems[mBlendStateSelected])->Set();
	
	mSampler.BindPS(0);
	mTexture.BindPS(0);
	
	std::vector<VertexPNC> vertices;
	vertices.reserve(particles.size());
	for (auto& p : particles)
	{
		if (p.lifeTime > 0.0f)
		{
			float size = 1.0f - p.lifeTime / maxLifeTime;
	
			VertexPNC vertex;
			vertex.position = p.position;
			vertex.normal.x = size;
			vertex.normal.y = size;
			vertex.color = Vector4{
				Lerp(startColor.x, endColor.x, size),
				Lerp(startColor.y, endColor.y, size),
				Lerp(startColor.z, endColor.z, size),
				1.0f
			};
			vertices.push_back(vertex);
		}
	}
	std::sort(vertices.begin(), vertices.end(), [&](auto& a, auto& b)
	{
		auto s = Dot(a.position, mCamera.GetDirection());
		auto t = Dot(b.position, mCamera.GetDirection());
		return s > t;
	});
	mMeshBuffer.Update(vertices.data(), static_cast<uint32_t>(vertices.size()));
	mMeshBuffer.SetTopology(MeshBuffer::Topology::Points);
	mMeshBuffer.Render();
	mGeometryShader.Unbind();
	BlendState::ClearState();


}

void GameState::DebugUI()
{
	ImGui::SetNextWindowPos({ 30.0f, 30.0f });
	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::Text("FPS: %f", 1.0f / deltaTime);
	ImGui::Text("Particles: %d", particles.size());
	
	ImGui::Combo("Blend Mode", &mBlendStateSelected, blendItems, IM_ARRAYSIZE(blendItems));
	ImGui::ColorEdit4("Start Color", (float*)&startColor);
	ImGui::ColorEdit4("End Color", (float*)&endColor);
	ImGui::DragFloat("Max Life Time", &maxLifeTime, 0.1f, 0.0f, 10.0f);
	ImGui::End();
}