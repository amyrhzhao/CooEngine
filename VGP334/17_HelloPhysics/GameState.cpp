#include "GameState.h"

using namespace Coo;
using namespace Coo::Physics;
using namespace Coo::Input;
using namespace Coo::Math;
using namespace Coo::Graphics;

namespace
{
	int currAnimClip;
	float fps;

	void RandomParticle(Particle& particle, Vector3 pos)
	{
		Vector3 rad = { RandomFloat(-1.0f,1.0f) + 0.01f, RandomFloat(0.0f,1.0f),RandomFloat(-1.0f, 1.0f) };
		rad = Coo::Math::Normalize(rad) * 0.1f;
		particle.SetPosition(pos);
		particle.SetVelocity(rad);
		particle.radius = 0.05f;
		particle.invMass = 1.0f;
		return;
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition({ 0.0f,0.0f,-10.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });
}

void GameState::Terminate()
{
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
	physicsWorld.Update(deltaTime);
	fps = 1.0f / deltaTime;
}

void GameState::Render()
{
	for (float z = -50.0f; z < 50.0f; z += 1.0f)
	{
		Graphics::SimpleDraw::AddLine({ -50.0f, 0.0f, z }, { 50.0f, 0.0f, z }, Colors::Gray);
	}

	for (float x = -50.0f; x < 50.0f; x += 1.0f)
	{
		Graphics::SimpleDraw::AddLine({ x, 0.0f, -50.0f }, { x, 0.0f, 50.0f }, Colors::Gray);
	}

	physicsWorld.DebugDraw();
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Physics Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("FPS: %f", fps);
	// TODO 
	// Have cube, cloth, web

	// Point Particle
	if (ImGui::Button("Boom!"))
	{
		physicsWorld.ClearDynamic();
		for (int i = 0; i < 100; ++i)
		{
			Particle* particle = new Particle();
			RandomParticle(*particle, { 0.0f,0.0f,0.0f });
			physicsWorld.AddParticle(particle);
		}
	}

	// Stick
	if (ImGui::Button("Boom Sticks"))
	{
		physicsWorld.ClearDynamic();
		for (int i = 0; i < 100; ++i)
		{
			Particle* particles[2];
			for (int j = 0; j < 2; j++)
			{
				particles[j] = new Coo::Physics::Particle();
				Vector3 pos;
				if (j == 0) { pos = { 0.0f, 0.0f,0.0f }; }
				else { pos = { 0.0f, 0.5f, 0.0f }; }
				RandomParticle(*particles[j], pos);
				physicsWorld.AddParticle(particles[j]);
			}

			auto constrain = new Coo::Physics::Spring(particles[0], particles[1]);
			physicsWorld.AddConstraint(constrain);
		}
	}

	// Tetrahedron
	if (ImGui::Button("Boom Tetrahedrom"))
	{
		physicsWorld.ClearDynamic();
		for (int i = 0; i < 100; ++i)
		{
			Particle* particles[4];
			for (int j = 0; j < 4; j++)
			{
				particles[j] = new Coo::Physics::Particle();
				Vector3 pos;
				if (j == 0) { pos = { 0.94f, 0.0f, -0.33f }; }
				else if (j == 1) { pos = { -0.47f, 0.82f, -0.33f }; }
				else if (j == 2) { pos = { -0.47f, -0.82f, -0.33f }; }
				else if (j == 3) { pos = { 0.0f, 0.0f, 1.0f }; }
				pos /= 3.0f;
				RandomParticle(*particles[j], pos);
				physicsWorld.AddParticle(particles[j]);
			}

			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[0], particles[1]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[0], particles[2]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[0], particles[3]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[1], particles[2]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[1], particles[3]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[2], particles[3]));

		}
	}

	// Cube
	if (ImGui::Button("Boom Cube"))
	{
		physicsWorld.ClearDynamic();
		for (int i = 0; i < 100; ++i)
		{
			Particle* particles[8];
			for (int j = 0; j < 8; j++)
			{
				particles[j] = new Coo::Physics::Particle();
				Vector3 pos;
				if (j == 0) { pos = { 1.0f, 1.0f, 1.0f }; }
				else if (j == 1) { pos = { -1.0f, 1.0f, 1.0f }; }
				else if (j == 2) { pos = { 1.0f, -1.0f, 1.0f }; }
				else if (j == 3) { pos = { -1.0f, -1.0f, 1.0f }; }
				else if (j == 4) { pos = { 1.0f, 1.0f, -1.0f }; }
				else if (j == 5) { pos = { -1.0f, 1.0f, -1.0f }; }
				else if (j == 6) { pos = { 1.0f, -1.0f, -1.0f }; }
				else if (j == 7) { pos = { -1.0f, -1.0f, -1.0f }; }
				pos *= 0.2f;
				RandomParticle(*particles[j], pos);
				physicsWorld.AddParticle(particles[j]);
			}

			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[0], particles[1]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[0], particles[2]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[0], particles[3]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[0], particles[4]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[0], particles[5]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[0], particles[6]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[1], particles[2]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[1], particles[3]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[1], particles[4]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[1], particles[5]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[1], particles[7]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[2], particles[3]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[2], particles[4]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[2], particles[6]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[2], particles[7]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[3], particles[5]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[3], particles[6]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[3], particles[7]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[4], particles[5]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[4], particles[6]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[4], particles[7]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[5], particles[6]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[5], particles[7]));
			physicsWorld.AddConstraint(new Coo::Physics::Spring(particles[6], particles[7]));

		}
	}


	ImGui::End();

}

