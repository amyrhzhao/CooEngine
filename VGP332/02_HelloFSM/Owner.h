#ifndef HELLOFSM_OWNER_H
#define HELLOFSM_OWNER_H

#include <Coo/Inc/Coo.h>

struct Owner : public Coo::AI::Agent 
{
	Owner(Coo::AI::AIWorld& world) :Agent(world) {}
	void Initialize();
	void Update(float deltaTime);
	void Render() const;

	std::unique_ptr<Coo::AI::SteeringModule> steeringModule;
	std::vector<Coo::Math::Circle> obstacles;
	Coo::Graphics::TextureId textureId;
	Coo::Math::Vector2 destination;
};

#endif // !HELLOFSM_OWNER_H
