#include "GameState.h"
TileMap map;

struct GetGCost
{
	float operator()(AI::GraphSearch::Context context, size_t parentIndex, size_t nodeIndex)
	{
		if (parentIndex == InvalidNode)
		{
			return 0.0f;
		}
		int columns = map.GetColumns();
		size_t parentColumn = parentIndex % columns;
		size_t parentRow = parentIndex / columns;
		size_t nodeColumn = nodeIndex % columns;
		size_t nodeRow = nodeIndex / columns;
		float g = context.g[parentIndex] + ((parentColumn != nodeColumn && parentRow != nodeRow) ? 1.414f : 1.0f);
		return g;
	}
};

struct GetHCost
{
	float operator()(AI::GraphSearch::Context& context, size_t nodeIndex)
	{
		int columns = map.GetColumns();
		size_t currColumn = nodeIndex % columns;
		size_t currRow = nodeIndex / columns;
		size_t nodeColumn = context.end % columns;
		size_t nodeRow = context.end / columns;
		return sqrtf(static_cast<float>(((nodeColumn - currColumn) * (nodeColumn - currColumn)) + ((nodeRow - currRow) * (nodeRow - currRow))));
	}
};

struct CanOpenNode1
{
	bool operator() (GraphSearch::Context& context, size_t n)
	{
		if (context.open[n])
		{
			return false;
		}
		if (map.IsWall(n))
		{
			return false;
		}
		return true;
		//if (!map.IsWall(n) && !context.open[n])
		//{
		//	return true;
		//}
		//return false;
	}
};

struct CanOpenNode2
{
	bool operator() (GraphSearch::Context& context, size_t n)
	{
		if (!map.IsWall(n) && !context.closed[n])
		{
			return true;
		}
		return false;
	}
};

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,0.0f,-10.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });
	map.Load();
	BuildGraph();
	start = 0;
	//end = 45;
	end = (map.GetColumns() * map.GetColumns()) - 1;
}

void GameState::Terminate()
{
	map.Unload();
}

void GameState::Update(float deltaTime)
{
	fps = 1.0f / deltaTime;
	switch (mCurrMode)
	{
	case Mode::TileMapEdit:
	{
		map.Update(deltaTime);
		break;
	}
	case Mode::PathFinding:
	{
		CheckInput();
		break;
	}
	}
}

void GameState::Render()
{
	map.Render();
	if (mShowGraph)
	{
		using std::placeholders::_1;
		std::function<bool(size_t)> func = std::bind(&TileMap::IsWall, &map, _1);
		RenderGraph(mGraph, func);
	}
	if (mCurrMode == Mode::PathFinding)
	{
		if (mShowSearch)
		{
			RenderSearch(mGraph, context);
		}
		RenderGoal();
	}
	Coo::Graphics::SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Path Finding", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %f", fps);
	ImGui::Text("Search Time: %f", lastSearchTime);
	ImGui::Text("Mode");
	if (ImGui::RadioButton("Tile Map Editor", mCurrMode == Mode::TileMapEdit))
		mCurrMode = Mode::TileMapEdit;
	if (ImGui::RadioButton("Path Finding", mCurrMode == Mode::PathFinding))
		mCurrMode = Mode::PathFinding;
	ImGui::Checkbox("Show Graph", &mShowGraph);
	switch (mCurrMode)
	{
	case Mode::TileMapEdit:
	{
		map.DebugUI();
		break;
	}
	case Mode::PathFinding:
	{
		ImGui::Checkbox("Show Search", &mShowSearch);

		ImGui::Text("Path Finding");
		if (ImGui::RadioButton("BFS", mCurrSearchType == SearchType::BFS))
			mCurrSearchType = SearchType::BFS;
		if (ImGui::RadioButton("DFS", mCurrSearchType == SearchType::DFS))
			mCurrSearchType = SearchType::DFS;
		if (ImGui::RadioButton("Dijkstra", mCurrSearchType == SearchType::Dijkstra))
			mCurrSearchType = SearchType::Dijkstra;
		if (ImGui::RadioButton("A*", mCurrSearchType == SearchType::AStar))
			mCurrSearchType = SearchType::AStar;

		if (ImGui::Button("Execute"))
		{
			context.Init(mGraph.GetSize());
			context.Reset();
			auto startTime = std::chrono::high_resolution_clock::now();
			ExecuteSearch<CanOpenNode1,CanOpenNode2, GetGCost, GetHCost>(mGraph, context, start, end, mCurrSearchType);
			auto finishTime = std::chrono::high_resolution_clock::now();
			lastSearchTime = std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - startTime).count() / 1000.0f;
		}
		break;
	}
	}
	ImGui::End();
}

void GameState::BuildGraph()
{
	BuildGridBasedGraph(mGraph, map);
}

bool GameState::IsWall(size_t index)
{
	return map.IsWall(index);
}

void GameState::CheckInput()
{

	if (Coo::Input::InputSystem::Get()->IsMouseDown(Coo::Input::MouseButton::LBUTTON))
	{
		auto index = map.MousePosToIndex();
		if (index >= 0)
			start = index;
	}
	if (Coo::Input::InputSystem::Get()->IsMouseDown(Coo::Input::MouseButton::RBUTTON))
	{
		auto index = map.MousePosToIndex();
		if (index >= 0)
			end = index;
	}
}

void GameState::RenderGoal()
{
	auto max = mGraph.GetSize();
	if (start < max)
		Coo::Graphics::SimpleDraw::AddScreenCircle(mGraph.GetNode(start).position, 9.0f, Coo::Graphics::Colors::Violet);
	if (end < max)
		Coo::Graphics::SimpleDraw::AddScreenCircle(mGraph.GetNode(end).position, 9.0f, Coo::Graphics::Colors::Violet);
}
