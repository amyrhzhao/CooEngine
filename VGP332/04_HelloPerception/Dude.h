#pragma once
#include "Coo/Inc/Coo.h"

class Dude : public Coo::AI::Agent
{
public:
	Dude(Coo::AI::AIWorld& world) : Coo::AI::Agent(world) {}
	void Initialize();

	void Update(float deltaTime);
	void Render();

	void DebugUI();

private:
	std::unique_ptr<Coo::AI::SteeringModule> steeringModule;
	std::unique_ptr<Coo::AI::PerceptionModule> perceptionModule;
	Coo::Graphics::TextureId mTextureId;
};