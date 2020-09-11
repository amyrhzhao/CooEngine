#include "Bird.h"
#include "Pipe.h"
#include <Coo/Inc/Coo.h>

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

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	CooApp::AddState<Game>("GameState");
	CooApp::Run({ "Hello Path Finding","../../Assets/" ,500,720 });
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
