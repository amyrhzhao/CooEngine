#include "Bird.h"
#include "PipeManager.h"
#include <Coo/Inc/Coo.h>
#include <vector>

PipeManager pm;
std::vector<Bird> birds;
std::unique_ptr<Coo::AI::NEAT::Population> population;
size_t activeBirdCount = 0;
//std::vector<Pipe> pipes;
float spawnDelay = 0.0f;
bool run = false;
Coo::Graphics::Camera mCamera;

void Title(float deltaTime);
void Play(float deltaTime);
void RunNEAT(float deltaTime);
void Init();
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

void Init() 
{
	pm.Reset();
	birds.clear();
	population.reset();
	activeBirdCount = 0;
	spawnDelay = 0.0f;
}

void Title(float deltaTime)
{
	Coo::Graphics::SimpleDraw::AddScreenText("Hit [Space] to Flap", 10.0f, 10.0f, 20.0f, Coo::Graphics::Colors::White);
	Coo::Graphics::SimpleDraw::AddScreenText("Hit [N] to NEATy bird", 10.0f, 30.0f, 20.0f, Coo::Graphics::Colors::White);

	auto inputSys = Coo::Input::InputSystem::Get();
	if (inputSys->IsKeyPressed(Coo::Input::KeyCode::SPACE))
	{
		auto& bird = birds.emplace_back();
		bird.SetPosition({ 150.0f, 300.0f });

		Tick = Play;
	}
	else if (inputSys->IsKeyPressed(Coo::Input::KeyCode::N))
	{
		population = std::make_unique<Coo::AI::NEAT::Population>(4, 1);

		Coo::AI::NEAT::MutationConfig& mutationConfig = population->mutationConfig;
		mutationConfig.connection_mutate_chance = 0.65;
		mutationConfig.perturb_chance = 0.9;
		mutationConfig.crossover_chance = 0.75;
		mutationConfig.link_mutation_chance = 0.85;
		mutationConfig.node_mutation_chance = 0.5;
		mutationConfig.bias_mutation_chance = 0.2;
		mutationConfig.step_size = 0.1;
		mutationConfig.disable_mutation_chance = 0.2;
		mutationConfig.enable_mutation_chance = 0.2;

		for (auto& s : population->species)
		{
			for (auto& g : s.genomes)
			{
				auto& bird = birds.emplace_back();
				//bird.Load();
				bird.Spawn({ 150.0f, 300.0f });

				bird.brain = std::make_unique<Coo::AI::NEAT::NeuralNet>();
				bird.brain->Initialize(g, population->neuralNetConfig);
				bird.fitness = 0.0f;

				++activeBirdCount;
			}
		}

		Tick = RunNEAT;
	}
}

void Play(float deltaTime)
{
	pm.Update(deltaTime);

	for (auto& bird : birds)
		bird.Update(deltaTime, pm);

	if (Coo::Input::InputSystem::Get()->IsKeyPressed(Coo::Input::KeyCode::SPACE))
 		birds[0].Flap();

	//bg.Render();
	pm.Render();
	for (auto& bird : birds)
		bird.Render();

	//spawnDelay -= deltaTime;
	//if (spawnDelay <= 0.0f)
	//{
	//	auto& pipe = pipes.emplace_back();
	//	pipe.Spawn(100.0f);
	//	spawnDelay = 5.0f;
	//}

	//for (auto& pipe : pipes)
	//	pipe.Update(deltaTime);

	//auto IsOffScreen = [](const auto& pipe) { return pipe.GetTopRect().b < 0.0f; };
	//pipes.erase(std::remove_if(pipes.begin(), pipes.end(), IsOffScreen), pipes.end());

	/*for (auto& bird : birds)
		bird.Update(deltaTime);

	auto inputSys = Coo::Input::InputSystem::Get();
	if (inputSys->IsKeyPressed(Coo::Input::KeyCode::SPACE))
	{
		birds[0].Flap();
	}

	for (auto& pipe : pipes)
		pipe.Render();
	for (auto& bird : birds)
		bird.Render();*/

	//if (!birds[0].IsAlive()) 
	//{
	//	Init();
	//	auto& bird = birds.emplace_back();
	//	bird.SetPosition({ 150.0f, 300.0f });
	//}

	if (Coo::Input::InputSystem::Get()->IsKeyPressed(Coo::Input::KeyCode::ESCAPE)) 
	{
		Init();
		Tick = Title;
	}
}

void RunNEAT(float deltaTime)
{
	if (activeBirdCount == 0)
	{
		// Feed bird fitness back into the genome
		for (auto& s : population->species)
			for (auto& g : s.genomes)
				g.fitness = static_cast<size_t>(birds[activeBirdCount++].fitness);

		population->NewGeneration();

		// Use new species/genomes to respawn birds with new brains
		activeBirdCount = 0;
		for (auto& s : population->species)
		{
			for (auto& g : s.genomes)
			{
				auto& bird = birds[activeBirdCount++];
				bird.Spawn({ 150.0f, 300.0f });

				bird.brain = std::make_unique<Coo::AI::NEAT::NeuralNet>();
				bird.brain->Initialize(g, population->neuralNetConfig);
				bird.fitness = 0.0f;
			}
		}

		pm.Reset();
	}

	//bg.Update(deltaTime);
	pm.Update(deltaTime);

	activeBirdCount = 0;
	for (auto& bird : birds)
	{
		bird.Update(deltaTime, pm);
		if (bird.IsAlive())
			++activeBirdCount;
	}

	//bg.Render();
	pm.Render();
	for (auto& bird : birds)
		bird.Render();

	std::string txt;
	txt += "Generation: " + std::to_string(population->Generation());
	Coo::Graphics::SimpleDraw::AddScreenText(txt.c_str(), 10.0f, 10.0f, 20.0f, Coo::Graphics::Colors::White);

	if (Coo::Input::InputSystem::Get()->IsKeyPressed(Coo::Input::KeyCode::ESCAPE)) 
	{
		Init();
		Tick = Title;
	}
}

//size_t XORTest(Coo::AI::NEAT::NeuralNet& n, bool log)
//{
//	using namespace Coo::AI::NEAT;
//	size_t fitness = 0;
//
//	std::string message;
//	if (log)
//		message = "	> begin xor test\n		>";
//
//	auto output = n.Evaluate({ 0.0, 0.0 });
//	if (log)
//		message += std::to_string(output[0]) + " ";
//	fitness += static_cast<int>(std::min(1.0 / ((0.0 - output[0]) * (0.0f - output[0])), 50.0));
//
//	output = n.Evaluate({ 0.0, 1.0 });
//	if (log)
//		message += std::to_string(output[0]) + " ";
//	fitness += static_cast<int>(std::min(1.0 / ((1.0 - output[0]) * (1.0f - output[0])), 50.0));
//
//	output = n.Evaluate({ 1.0, 0.0 });
//	if (log)
//		message += std::to_string(output[0]) + " ";
//	fitness += static_cast<int>(std::min(1.0 / ((1.0 - output[0]) * (1.0f - output[0])), 50.0));
//
//	output = n.Evaluate({ 1.0, 1.0 });
//	if (log)
//		message += std::to_string(output[0]) + " ";
//	fitness += static_cast<int>(std::min(1.0 / ((0.0 - output[0]) * (0.0f - output[0])), 50.0));
//
//	if (log)
//	{
//		message += ") fitness" + std::to_string(fitness);
//		
//		//LOG(message.c_str());
//	}
//
//	return fitness;
//}

//void Test()
//{
//	using namespace Coo::AI::NEAT;
//	Population p(2, 1, 0);
//	NeuralNet bestGuy;
//
//	size_t max_fitness = 0;
//	while (max_fitness < 150) {
//		size_t current_fitness = 0;
//		size_t min_fitness = 100000;
//		for (auto& s : p.species)
//		{
//			for (auto& g : s.genomes)
//			{
//				NeuralNet n;
//				n.Initialize(g, p.neuralNetConfig);
//				current_fitness = XORTest(n, true);
//				if (current_fitness < min_fitness)
//					min_fitness = current_fitness;
//				if (current_fitness > max_fitness)
//				{
//					max_fitness = current_fitness;
//					bestGuy = n;
//				}
//				g.fitness = current_fitness;
//			}
//		}
//		LOG("Generation %d successfully tested. Global min: %d, Global max: %d", p.Generation(), min_fitness, max_fitness);
//		p.NewGeneration();
//	}
//
//}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//Test();
	CooApp::AddState<Game>("GameState");
	CooApp::Run({ "Hello Bird","../../Assets/" ,500,720 });
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
