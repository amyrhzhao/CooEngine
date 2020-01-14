#ifndef HELLOFSMSAMPLE_GAMESTATE_H
#define HELLOFSMSAMPLE_GAMESTATE_H

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
	Dude mDude;
	Graphics::TextureId mTextureId;
	Coo::Graphics::Camera mCamera;
};

#endif // !HELLOFSMSAMPLE_GAMESTATE_H
