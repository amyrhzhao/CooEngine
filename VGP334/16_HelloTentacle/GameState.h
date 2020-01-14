#ifndef INCLUDED_HELLOMODEL_GAMESTATE_H
#define INCLUDED_HELLOMODEL_GAMESTATE_H

#include "Coo/Inc/Coo.h"

using namespace Coo::Graphics;
using namespace Coo::Math;

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

	VertexShader mVertexShader;
	PixelShader mPixelShader;

	BoneMesh mMesh;
	MeshBuffer mMeshBuffer;
	TextureId mTextureId;
	std::vector<Bone> mBones;
	std::vector<Quaternion> mBoneRotations;
	size_t selectedBone = 0;

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
		float padding[2];
	};

	struct BoneTransformData 
	{
		Coo::Math::Matrix4 boneTransforms[128];
	};

	using TransformBuffer = TypedConstantBuffer<TransformData>;
	using LightBuffer = TypedConstantBuffer<DirectionalLight>;
	using MaterialBuffer = TypedConstantBuffer<Material>;
	using OptionsBuffer = TypedConstantBuffer<OptionsData>;
	using BoneTransformBuffer = TypedConstantBuffer<BoneTransformData>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionsBuffer mOptionsBuffer;
	BoneTransformBuffer mBoneTransformBuffer;

	float mTurnSpeed = 0.1f;
	float mMoveSpeed = 10.0f;

	DirectionalLight mLight;
	Material mMaterial;
	OptionsData mOptions;
	BoneTransformData mBoneTransform;
	Coo::Math::Vector3 rootPosition;
};

#endif // !INCLUDED_HELLOMODEL_GAMESTATE_H
