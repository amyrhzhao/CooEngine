#include "Precompiled.h"
#include "TerrainService.h"
#include "CameraService.h"
#include "World.h"

using namespace Coo;

META_DERIVED_BEGIN(TerrainService, Service)
	META_FIELD_BEGIN
		META_FIELD(mNumRows, "NumRows")
		META_FIELD(mNumCols, "NumCols")
		META_FIELD(mCellSize, "CellSize")
	META_FIELD_END
META_CLASS_END

void Coo::TerrainService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();
	mTerrain.Initialize(static_cast<uint32_t>(mNumRows), static_cast<uint32_t>(mNumCols), mCellSize);
}

void Coo::TerrainService::Terminate()
{
	mTerrain.Terminate();
}

void Coo::TerrainService::Render()
{
	mTerrain.Render(mCameraService->GetActiveCamera());
}

void Coo::TerrainService::DebugUI()
{
	mTerrain.DebugUI();
}
