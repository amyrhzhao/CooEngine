#pragma once
#include "Coo/Inc/Coo.h"

class GameState : public Coo::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	Coo::AI::AIWorld mAIWorld;
	Coo::Graphics::Camera mCamera;
};