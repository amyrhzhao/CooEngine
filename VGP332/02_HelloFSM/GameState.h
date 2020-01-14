#ifndef INCLUDED_FSM_GAMESTATE_H
#define INCLUDED_FSM_GAMESTATE_H

#include "Bone.h"
#include "Dog.h"
#include "Owner.h"

class GameState : public Coo::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	std::unique_ptr<Dog> mDog;
	std::unique_ptr<Owner> mOwner;
	std::unique_ptr<Bone> mBone;
	Coo::AI::AIWorld mAIWorld;
	Coo::Graphics::Camera mCamera;
};

#endif // !INCLUDED_FSM_GAMESTATE_H
