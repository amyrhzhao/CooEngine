#ifndef HELLOFSM_DOG_H
#define HELLOFSM_DOG_H

#include <Coo/Inc/Coo.h>

class Bone;
struct Owner;

struct Dog : public Coo::AI::Agent
{
	Dog(Coo::AI::AIWorld& world) : Agent(world) {}
	void Initialize();
	void Update(float deltaTime);
	void Render() const;
	void FetchBone(Bone& bone);
	
	std::unique_ptr<Coo::AI::StateMachine<Dog>> stateMachine;
	std::unique_ptr<Coo::AI::SteeringModule> steeringModule;
	Coo::Graphics::TextureId textureId;
	Owner* owner = nullptr;
	Bone* target = nullptr;
	bool isFetching = false;
};


struct WhirlState : public Coo::AI::State<Dog>
{
	static std::string GetName() { return "Whirl"; }

	void Enter(Dog& agent) override;
	void Update(Dog& agent, float deltaTime) override;
	void Exit(Dog& agent) override;

	Coo::Math::Vector2 position;
	float time;
};

struct RunState : public Coo::AI::State<Dog> 
{
	static std::string GetName() { return "Run"; }

	void Enter(Dog& agent) override;
	void Update(Dog& agent, float deltaTime) override;
	void Exit(Dog& agent) override;
	Coo::Math::Vector2 target;
};

struct FetchState :public Coo::AI::State<Dog> 
{
	static std::string GetName() { return "Fetch"; }
	void Enter(Dog& agent) override;
	void Update(Dog& agent, float deltaTime) override;
	void Exit(Dog& agent) override;
	Coo::Math::Vector2 target;
};

#endif // !HELLOFSM_DOG_H
