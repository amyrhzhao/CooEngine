#ifndef HELLOSTEERING_GAMESTATE_H
#define HELLOSTEERING_GAMESTATE_H

#include "Dude.h"

class GameState : public Coo::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void GenrateRandomObstacles();
	void GroupBehaviorDebugUI();

	std::vector<std::unique_ptr<Dude>> mDudes;
	Coo::AI::AIWorld mAIWorld;
	Coo::Graphics::Camera mCamera;

	float fps = 60.0f;
	float seekWeight = 0.0f;
	float wanderWeight = 5.0f;
	float ObsAvoidWeight = 5.0f;
	float SperationWeight = 3.0f;
	float AlignmentWeight = 1.8f;
	float CohesionWeight = 3.0f;

};

#endif // !HELLOSTEERING_GAMESTATE_H
