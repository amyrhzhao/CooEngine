#ifndef INCLUDED_HELLOMODEL_GAMESTATE_H
#define INCLUDED_HELLOMODEL_GAMESTATE_H

#include "Coo/Inc/Coo.h"

using namespace Coo::Graphics;

class GameState : public Coo::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	Camera mCamera;
	Mesh mTankMesh;
	MeshBuffer mTankMeshBuffer;
	Mesh mPlaneMesh;
	MeshBuffer mPlaneMeshBuffer;

	TextureId diffuseMap;
	TextureId specularMap;
	TextureId normalMap;
	TextureId aoMap;

	VertexShader mVertexShader;
	PixelShader mPixelShader;

	struct TransformData
	{
		Coo::Math::Matrix4 world;
		Coo::Math::Matrix4 wvp;
		Coo::Math::Vector3 viewPosition;
		float padding;
	};

	struct OptionsData
	{
		float displacementWeight;
		float useNormal = 1.0f;
		float aoPower = 1.0f;
		float padding;
	};

	using TransformBuffer = TypedConstantBuffer<TransformData>;
	using LightBuffer = TypedConstantBuffer<DirectionalLight>;
	using MaterialBuffer = TypedConstantBuffer<Material>;
	using OptionsBuffer = TypedConstantBuffer<OptionsData>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionsBuffer mOptionsBuffer;

	float mTurnSpeed = 0.1f;
	float mMoveSpeed = 10.0f;

	DirectionalLight mLight;
	Material mMaterial;
	OptionsData mOptions;
	Coo::Math::Vector3 mPosition;

	void RenderTank();
	void RenderPlane();
};

#endif // !INCLUDED_HELLOMODEL_GAMESTATE_H
