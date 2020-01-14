#ifndef HELLOPERCEPTION_GAMESTATE_H
#define HELLOPERCEPTION_GAMESTATE_H

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

	std::unique_ptr<Dude> mDude;
	Coo::AI::AIWorld mAIWorld;
	Coo::Graphics::Camera mCamera;
};

#endif // !HELLOPERCEPTION_GAMESTATE_H
