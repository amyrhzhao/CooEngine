#ifndef INCLUDED_HELLOPHYSICS_GAMESTATE_H
#define INCLUDED_HELLOPHYSICS_GAMESTATE_H

#include "Coo/Inc/Coo.h"

using namespace Coo::Graphics;

class GameState : public Coo::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	Camera mCamera;

	float mTurnSpeed = 0.1f;
	float mMoveSpeed = 10.0f;

	Coo::Math::Vector3 mPosition;

	Coo::Physics::PhysicsWorld physicsWorld;
};

#endif // !INCLUDED_HELLOPHYSICS_GAMESTATE_H
