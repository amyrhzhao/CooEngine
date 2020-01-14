#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "TileMap.h"
#include "GraphUtil.h"

using namespace Coo::AI;

enum class Mode
{
	TileMapEdit,
	PathFinding
};

class GameState : public Coo::AppState 
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void BuildGraph();
	bool IsWall(size_t index);
	void CheckInput();
	void RenderGoal();

	NavGraph mGraph;
	AI::GraphSearch::Context context;
	Mode mCurrMode = Mode::TileMapEdit;
	SearchType mCurrSearchType = SearchType::BFS;
	bool mShowGraph = false;
	bool mShowSearch = false;
	Coo::Graphics::Camera mCamera;
	float fps;
	float lastSearchTime = 0.0f;
	size_t start;
	size_t end;	
};

#endif // !GAMESTATE_H
