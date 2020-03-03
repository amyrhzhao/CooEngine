#ifndef INCLUDED_HELLOMODEL_GAMESTATE_H
#define INCLUDED_HELLOMODEL_GAMESTATE_H

#include "Coo/Inc/Coo.h"

using namespace Coo;
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
	Camera mDefaultCamera;
	Camera mLightCamera;
	Camera* mActiveCamera = nullptr;

	Mesh mTankMesh;
	MeshBuffer mTankMeshBuffer;
	
	Mesh mPlaneMesh;
	MeshBuffer mPlaneMeshBuffer;

	TextureId diffuseMap;
	TextureId specularMap;
	TextureId normalMap;
	TextureId aoMap;

	TextureId mGroundDiffuseMap;

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
		float useShadow = 0.0f;
		float depthBias = 0.0f;
		float padding[3];
	};

	using TransformBuffer = TypedConstantBuffer<TransformData>;
	using LightBuffer = TypedConstantBuffer<DirectionalLight>;
	using MaterialBuffer = TypedConstantBuffer<Material>;
	using OptionsBuffer = TypedConstantBuffer<OptionsData>;
	using DepthMapConstantBuffer = TypedConstantBuffer<Math::Matrix4>;
	using ShadowConstantBuffer = TypedConstantBuffer<Math::Matrix4>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionsBuffer mOptionsBuffer;

	float mTurnSpeed = 0.1f;
	float mMoveSpeed = 10.0f;

	DirectionalLight mLight;
	Material mMaterial;
	OptionsData mOptions;
	Math::Vector3 mPosition;

	Math::Vector3 mTankPosition = Math::Vector3::Zero();
	Math::Vector3 mTankRotation = Math::Vector3::Zero();

	RenderTarget mDepthMapRenderTarget;
	VertexShader mDepthMapVertexShader;
	PixelShader mDepthMapPixelShader;
	DepthMapConstantBuffer mDepthMapConstantBuffer;
	ShadowConstantBuffer mShadowConstantBuffer;

	void DrawScene();
	void DrawDepthMap();
};

#endif // !INCLUDED_HELLOMODEL_GAMESTATE_H
