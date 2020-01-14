#pragma once

#include <Coo/Inc/Coo.h>

class Sheep : public Coo::AI::Agent 
{
public:
	Sheep(Coo::AI::AIWorld& world) : Coo::AI::Agent(world) {}
	void Initialize(Coo::Math::Vector2 position);
	void Terminate();
	Coo::Math::Circle GetBoundingCircle() { return { position,radius }; }
	void Grow(float food);

	void Render() const;
	void Update(float deltaTime);
	void UpdateSteering(float deltaTime);

	std::unique_ptr<Coo::AI::SteeringModule> steeringModule;
	std::unique_ptr<Coo::AI::StateMachine<Sheep>> stateMachine;
	Coo::Graphics::Color renderColor = Coo::Graphics::Colors::White;
	Coo::Graphics::Color stateColor = Coo::Graphics::Colors::Orange;
};

//struct EvadeState : public Coo::AI::State<Sheep>
//{
//	static std::string GetName() { return "Evade"; }
//	
//	void Enter(Sheep& agent) override;
//	void Update(Sheep& agent, float deltaTime) override;
//	void Exit(Sheep& agent) override;
//};

struct EatState : public Coo::AI::State<Sheep> 
{
	static std::string GetName() { return "Eat"; }
	
	void Enter(Sheep& agent) override;
	void Update(Sheep& agent, float deltaTime) override;
	void Exit(Sheep& agent) override;
};

struct SeekState : public Coo::AI::State<Sheep>
{
	static std::string GetName() { return "Seek"; }

	void Enter(Sheep& agent) override;
	void Update(Sheep& agent, float deltaTime) override;
	void Exit(Sheep& agent) override;
};