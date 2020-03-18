#ifndef INCLUDED_COOENGINE_TERRAIN_H
#define INCLUDED_COOENGINE_TERRAIN_H

// https://www.redblobgames.com/maps/terrain-from-noise/
#include "Quadtree.h"

namespace Coo
{
	class Terrain 
	{
	public:
		void Initialize(uint32_t numRows, uint32_t numCols, float scale);
		void Terminate();

		bool Intersect(Coo::Math::Ray ray, float& distance) const;

		void Render(const Graphics::Camera& camera);

		void DebugUI();

	private:
		void GenerateVertices();
		void GenerateIndices();

		struct ConstantData
		{
			Math::Matrix4 world;
			Math::Matrix4 wvp;
			Math::Vector4 viewPosition;
			Graphics::DirectionalLight directionalLight;
		};

		Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
		Graphics::VertexShader mTerrainVertexShader;
		Graphics::PixelShader mTerrainPixelShader;

		Graphics::Sampler mSampler;

		Graphics::Texture mGrassTexture;

		Graphics::Mesh mMesh;
		Graphics::MeshBuffer mMeshBuffer;

		Quadtree mQuadtree;

		uint32_t mNumRows{ 0 };
		uint32_t mNumCols{ 0 };
		uint32_t mNumCellsInCol{ 0 };
		uint32_t mNumCellsInRow{ 0 };
		uint32_t mNumCells{ 0 };

		float mScale{ 1.0f };
		float mFrequency0{ 5.0f };
		float mFrequency1{ 40.0f };
		float mWeight0{ 25.0f };
		float mWeight1{ 1.0f };
	};
} // namespace Coo

#endif // !INCLUDED_COOENGINE_TERRAIN_H
