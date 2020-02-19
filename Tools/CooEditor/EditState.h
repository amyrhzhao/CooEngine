#ifndef EDITSTATE_H
#define EDITSTATE_H

#include "Coo/Inc/Coo.h"

using namespace Coo::Graphics;

class EditState : public Coo::AppState 
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void DrawMenuBar();
	void ShowMainWindowWithDockSpace();
	void ShowSceneView();
	void ShowSettings();

	void RenderScene();
	void PostProcess(int iteration, int shader);

	// TODO: Add fishbowl distortion for crt

	Camera mCamera;
	Mesh mMesh;
	MeshBuffer mMeshBuffer;
	MeshPX mSpaceMesh;
	MeshBuffer mSpaceMeshBuffer;
	MeshPX mScreenMesh;
	MeshBuffer mScreenMeshBuffer;

	VertexShader mVertexShader;
	PixelShader mPixelShader;
	VertexShader mSpaceVertexShader;
	PixelShader mSpacePixelShader;
	VertexShader mPostProcessingVertexShader;
	PixelShader mPostProcessingPixelShader;
	VertexShader mCrtEffectVertexShader;
	PixelShader mCrtEffectPixelShader;
	VertexShader mBlurEffectVertexShader;
	PixelShader mBlurEffectPixelShader;
	VertexShader mFragmentVertexShader;
	PixelShader mFragmentPixelShader;


	bool applyPixel = true;
	bool applyCrt = true;
	bool applyBlur = true;

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

	struct FragmentData
	{
		float brightness = 0.0f;
		float contrast = 1.0f;
		float padding[2];
	};

	struct BlurData
	{
		Coo::Math::Vector2 blurDir;
		Coo::Math::Vector2 resolution;
	};

	using TransformBuffer = TypedConstantBuffer<TransformData>;
	using LightBuffer = TypedConstantBuffer<DirectionalLight>;
	using MaterialBuffer = TypedConstantBuffer<Material>;
	using OptionsBuffer = TypedConstantBuffer<OptionsData>;
	using FragmentBuffer = TypedConstantBuffer<FragmentData>;
	using BlurBuffer = TypedConstantBuffer<BlurData>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionsBuffer mOptionsBuffer;
	ConstantBuffer mSpaceConstantBuffer;
	FragmentBuffer mFragmentConstantBuffer;
	BlurBuffer mBlurConstantBuffer;

	float mTurnSpeed = 0.1f;
	float mMoveSpeed = 10.0f;
	int blurPower = 1;

	DirectionalLight mLight;
	Material mMaterial;
	OptionsData mOptions;
	FragmentData mFragmentData;
	BlurData mBlurData;
	Coo::Math::Vector3 mPosition;
	TextureId mDiffuseMap;
	TextureId mSpecularMap;
	TextureId mDisplacementMap;
	TextureId mNormalMap;
	TextureId mSpaceTexture;

	RenderTarget mRenderTargets[2];
};
#endif // !EDITSTATE_H