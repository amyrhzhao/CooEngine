#ifndef TILEMAP_H
#define TILEMAP_H

#include "Coo/Inc/Coo.h"

class TileMap 
{
public:
	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render() const;
	void DebugUI();
	
	int GetTileWidth() const { return tileWidth; }
	int GetTileHeight() const { return tileHeight; }
	int GetColumns() const { return mColumns; }
	int GetRows() const { return mRows; }
	inline bool IsWall(size_t index) const { return (mMap[index] == 2); }
	int GetIndex(int x, int y) const;
	int MousePosToIndex();

private:
	const int tileWidth = 32;
	const int tileHeight = 32;
	int mRows = 0;
	int mColumns = 0;
	int mTileCount = 0;
	int mCurrTileSelect = 0;
	std::unique_ptr<int[]>mMap = nullptr;
	std::unique_ptr<Coo::Graphics::TextureId[]>mTiles = nullptr;
};

#endif // !TILEMAP_H
