#ifndef HELLOSTEERING_DUDE_H
#define HELLOSTEERING_DUDE_H

#include <Coo/Inc/Coo.h>

class Dude: public Coo::AI::Agent
{
public:
	Dude(Coo::AI::AIWorld& world) : Coo::AI::Agent(world) {}
	void Initialize();

	void Update(float deltaTime);
	void Render();

	void DebugUI();
	void SetWeight(std::string behaviorName, float weight);

private:
	std::unique_ptr<Coo::AI::SteeringModule> steeringModule;
	Coo::Graphics::TextureId mTextureId;

	bool useSeek;
	bool useWander;
	bool useObstacleAvoidance;
};

#endif // !HELLOSTEERING_DUDE_H