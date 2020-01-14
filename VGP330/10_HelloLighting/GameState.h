#ifndef HELLOLIGHTING_GAMESTATE_H
#define HELLOLIGHTING_GAMESTATE_H

#include <Coo/Inc/Coo.h>

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
	Mesh mMesh;
	MeshBuffer mMeshBuffer;
	MeshPX mSpaceMesh;
	MeshBuffer mSpaceMeshBuffer;

	VertexShader mVertexShader;
	PixelShader mPixelShader;
	VertexShader mSpaceVertexShader;
	PixelShader mSpacePixelShader;

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

	using TransformBuffer = TypedConstantBuffer<TransformData>;	
	using LightBuffer = TypedConstantBuffer<DirectionalLight>;
	using MaterialBuffer = TypedConstantBuffer<Material>;
	using OptionsBuffer = TypedConstantBuffer<OptionsData>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionsBuffer mOptionsBuffer;
	ConstantBuffer mSpaceConstantBuffer;

	float mTurnSpeed = 0.1f;
	float mMoveSpeed = 10.0f;

	DirectionalLight mLight;
	Material mMaterial;
	OptionsData mOptions;
	Coo::Math::Vector3 mPosition;
	TextureId mDiffuseMap;
	TextureId mSpecularMap;
	TextureId mDisplacementMap;
	TextureId mNormalMap;
	TextureId mSpaceTexture;
};


#endif // !HELLOLIGHTING_GAMESTATE_H