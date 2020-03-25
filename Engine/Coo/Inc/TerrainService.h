#ifndef INCLUDED_COOENGINE_TERRAINSERVICE_H
#define INCLUDED_COOENGINE_TERRAINSERVICE_H

#include "Terrain.h"
#include "Service.h"

namespace Coo 
{
	class CameraService;
	class TerrainService : public Service
	{
	public:
		META_CLASS_DECLARE

		void Initialize() override;
		void Terminate() override;

		void Render() override;
		void DebugUI() override;

	private:
		const CameraService* mCameraService = nullptr;
		Terrain mTerrain;
		int mNumRows = 200;
		int mNumCols = 200;
		float mCellSize = 1.0f;
	};


} // namespace Coo

#endif // !INCLUDED_COOENGINE_TERRAINSERVICE_H
