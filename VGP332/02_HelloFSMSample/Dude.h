#ifndef HELLOFSMSAMPLE_DUDE_H
#define HELLOFSMSAMPLE_DUDE_H

#include <Coo/Inc/Coo.h>
#include <random>

using namespace Coo;

struct Dude
{
	Dude() {}
	void Initialize() 
	{
		stateMachine = std::make_unique<AI::StateMachine<Dude>>(*this);
	}

	std::unique_ptr<AI::StateMachine<Dude>> stateMachine;
	Math::Vector2 position;
};

struct DanceState : public AI::State<Dude>
{
	static std::string GetName() { return "Dance"; }

	void Enter(Dude& agent) override
	{
		position = agent.position;
		time = 0.0f;
	}

	void Update(Dude& agent, float deltaTime) override
	{
		time += deltaTime * 10.0f;
		if (time > Math::TwoPi)
		{
			agent.stateMachine->ChangeState("Run");
		}
		else
		{
			agent.position.x = position.x + sinf(time * 2.0f) * 30.0f;
			agent.position.y = position.y + cosf(time * 2.0f) * 10.0f;
		}
	}

	void Exit(Dude& agent) override
	{
		agent.position = position;
	}

	Math::Vector2 position;
	float time;
};

struct RunState : public AI::State<Dude>
{
	static std::string GetName() { return "Run"; }

	void Enter(Dude & agent) override;

	void Update(Dude& agent, float deltaTime) override
	{
		if (Math::DistanceSqr(agent.position, destination) < 10.0f)
		{
			agent.stateMachine->ChangeState("Dance");
		}
		else
		{
			agent.position += Math::Normalize(destination - agent.position) * 300.0f * deltaTime;
			Graphics::SimpleDraw::AddScreenLine(agent.position, destination, Graphics::Colors::Red);
			Graphics::SimpleDraw::AddScreenCircle(destination, 10.0f, Graphics::Colors::Green);
		}
	}

	void Exit(Dude& agent) override {}

	Math::Vector2 destination;
};

#endif // !HELLOFSMSAMPLE_DUDE_H
