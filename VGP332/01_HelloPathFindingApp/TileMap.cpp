#include "TileMap.h"
using namespace Coo::Graphics;

void TileMap::Load()
{
	// Initialize Tile Map
	mRows = 20;
	mColumns = 20;
	mMap = std::make_unique<int[]>(mRows* mColumns);

	// Initialize Tiles
	auto tm = TextureManager::Get();
	tm->SetRootPath(L"../../Assets/Images/");
	mTileCount = 3;
	mTiles = std::make_unique<Coo::Graphics::TextureId[]>(mTileCount);
	mTiles[0] = tm->LoadTexture(L"grass.png");
	mTiles[1] = tm->LoadTexture(L"grass_2.png");
	mTiles[2] = tm->LoadTexture(L"flower.png");

	SpriteRenderer::StaticInitialize();
}

void TileMap::Unload()
{
	mMap.reset();
	mTiles.reset();
	mTileCount = 0;
	mRows = 0;
	mColumns = 0;
	SpriteRenderer::StaticTerminate();
}

void TileMap::Update(float deltaTime)
{
	if (Coo::Input::InputSystem::Get()->IsMouseDown(Coo::Input::MouseButton::LBUTTON))
	{
		auto index = MousePosToIndex();
		if (index >= 0)
			mMap[index] = mCurrTileSelect;
	}
}

void TileMap::Render() const
{
	auto sr = SpriteRenderer::Get();
	auto tm = TextureManager::Get();
	sr->BeginRender();
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int index = GetIndex(x, y);
			auto texture = tm->GetTexture(mTiles[mMap[index]]);
			float posX = static_cast<float>(x * tileWidth);
			float posY = static_cast<float>(y * tileHeight);
			sr->Draw(*texture, { posX,posY });
		}
	}
	sr->EndRender();
}

void TileMap::DebugUI()
{
	// todo: Columns, Rows, LoadMap, SaveMap
	auto tm = TextureManager::Get();
	for (int i = 0; i < mTileCount; ++i)
	{
		auto texture = tm->GetTexture(mTiles[i]);
		if (Dui::ImageButton(*texture, { 32.0f,32.0f }))
		{
			mCurrTileSelect = i;
		}
		ImGui::SameLine();
	}
	ImGui::NewLine();
}

int TileMap::GetIndex(int x, int y) const
{
	return x + (y * mColumns);
}

int TileMap::MousePosToIndex()
{
	int index = -1;
	auto inputSystem = Coo::Input::InputSystem::Get();
	Coo::Math::Vector2 temp = { static_cast<float>(inputSystem->GetMouseScreenX()), static_cast<float>(inputSystem->GetMouseScreenY()) };
	int column = static_cast<int>(temp.x / tileWidth);
	int row = static_cast<int>(temp.y / tileHeight);
	if (column >= 0 && column < mColumns && row < mRows && row >= 0)
	{
		index = GetIndex(column, row);
	}
	return index;
}
