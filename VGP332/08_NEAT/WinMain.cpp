#include "Bird.h"
#include "Pipe.h"
#include <Coo/Inc/Coo.h>
#include <vector>

std::vector<Bird> birds;
std::vector<Pipe> pipes;
float spawnDelay = 0.0f;
bool run = false;
Coo::Graphics::Camera mCamera;

void Title(float deltaTime);
void Play(float deltaTime);
std::function<void(float)> Tick = Title;

class Game : public Coo::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;
};

void Title(float deltaTime)
{
	Coo::Graphics::SimpleDraw::AddScreenText("Hit [Space] to Flap", 10.0f, 10.0f, 20.0f, Coo::Graphics::Colors::White);
	Coo::Graphics::SimpleDraw::AddScreenText("Hit [N] to NEAT", 10.0f, 30.0f, 20.0f, Coo::Graphics::Colors::White);

	auto inputSys = Coo::Input::InputSystem::Get();
	if (inputSys->IsKeyPressed(Coo::Input::KeyCode::SPACE))
	{
		auto& bird = birds.emplace_back();
		bird.SetPosition({ 150.0f, 300.0f });

		Tick = Play;
	}
	else if (inputSys->IsKeyPressed(Coo::Input::KeyCode::N))
	{
		auto& bird = birds.emplace_back();
		bird.SetPosition({ 150.0f, 300.0f });

		auto& bird2 = birds.emplace_back();
		bird2.SetPosition({ 150.0f, 500.0f });

		Tick = Play;
	}
}

void Play(float deltaTime)
{
	spawnDelay -= deltaTime;
	if (spawnDelay <= 0.0f)
	{
		auto& pipe = pipes.emplace_back();
		pipe.Spawn(100.0f);
		spawnDelay = 5.0f;
	}

	for (auto& pipe : pipes)
		pipe.Update(deltaTime);

	auto IsOffScreen = [](const auto& pipe) { return pipe.GetTopRect().b < 0.0f; };
	pipes.erase(std::remove_if(pipes.begin(), pipes.end(), IsOffScreen), pipes.end());

	for (auto& bird : birds)
		bird.Update(deltaTime);

	auto inputSys = Coo::Input::InputSystem::Get();
	if (inputSys->IsKeyPressed(Coo::Input::KeyCode::SPACE))
	{
		birds[0].Flap();
	}

	for (auto& pipe : pipes)
		pipe.Render();
	for (auto& bird : birds)
		bird.Render();
}

size_t XORTest(Coo::AI::NEAT::NeuralNet& n, bool log)
{
	using namespace Coo::AI::NEAT;
	size_t fitness = 0;

	std::string message;
	if (log)
		message = "	> begin xor test\n		>";

	auto output = n.Evaluate({ 0.0, 0.0 });
	if (log)
		message += std::to_string(output[0]) + " ";
	fitness += static_cast<int>(std::min(1.0 / ((0.0 - output[0]) * (0.0f - output[0])), 50.0));

	output = n.Evaluate({ 0.0, 1.0 });
	if (log)
		message += std::to_string(output[0]) + " ";
	fitness += static_cast<int>(std::min(1.0 / ((1.0 - output[0]) * (1.0f - output[0])), 50.0));

	output = n.Evaluate({ 1.0, 0.0 });
	if (log)
		message += std::to_string(output[0]) + " ";
	fitness += static_cast<int>(std::min(1.0 / ((1.0 - output[0]) * (1.0f - output[0])), 50.0));

	output = n.Evaluate({ 1.0, 1.0 });
	if (log)
		message += std::to_string(output[0]) + " ";
	fitness += static_cast<int>(std::min(1.0 / ((0.0 - output[0]) * (0.0f - output[0])), 50.0));

	if (log)
	{
		message += ") fitness" + std::to_string(fitness);
		
		//LOG(message.c_str());
	}

	return fitness;
}

void Test()
{
	using namespace Coo::AI::NEAT;
	Population p(2, 1, 0);
	NeuralNet bestGuy;

	size_t max_fitness = 0;
	while (max_fitness < 150) {
		size_t current_fitness = 0;
		size_t min_fitness = 100000;
		for (auto& s : p.species)
		{
			for (auto& g : s.genomes)
			{
				NeuralNet n;
				n.Initialize(g, p.neuralNetConfig);
				current_fitness = XORTest(n, true);
				if (current_fitness < min_fitness)
					min_fitness = current_fitness;
				if (current_fitness > max_fitness)
				{
					max_fitness = current_fitness;
					bestGuy = n;
				}
				g.fitness = current_fitness;
			}
		}
		LOG("Generation %d successfully tested. Global min: %d, Global max: %d", p.Generation(), min_fitness, max_fitness);
		p.NewGeneration();
	}

}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Test();
	//CooApp::AddState<Game>("GameState");
	//CooApp::Run({ "Hello Path Finding","../../Assets/" ,500,720 });
}

void Game::Initialize()
{
	Coo::Graphics::GraphicsSystem::Get()->SetClearColor(Coo::Graphics::Colors::Black);
}

void Game::Terminate()
{
}

void Game::Update(float deltaTime)
{
	Tick(deltaTime);
}

void Game::Render()
{
	Coo::Graphics::SimpleDraw::Render(mCamera);
}

void Game::DebugUI()
{
}
