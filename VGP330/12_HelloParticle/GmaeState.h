#ifndef HELLOPARTICLE_GAMESTATE_H
#define HELLOPARTICLE_GAMESTATE_H

#include <Coo/Inc/Coo.h>

class GameState : public Coo::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	struct ConstantData
	{
		Coo::Math::Matrix4 wvp;
		Coo::Math::Vector4 viewPosition;
		Coo::Math::Vector3 viewUp;
		float padding;
	};

	Coo::Graphics::Camera mCamera;
	Coo::Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;

	Coo::Graphics::VertexShader mVertexShader;
	Coo::Graphics::GeometryShader mGeometryShader;
	Coo::Graphics::PixelShader mPixelShader;

	std::array<Coo::Graphics::BlendState, 4> mBlendStates;
	Coo::Graphics::Sampler mSampler;
	Coo::Graphics::Texture mTexture;

	Coo::Graphics::MeshBuffer mMeshBuffer;
	int mBlendStateSelected = 1;
};

#endif // !HELLOPARTICLE_GAMESTATE_H
