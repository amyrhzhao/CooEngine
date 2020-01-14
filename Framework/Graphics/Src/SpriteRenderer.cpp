#include "Precompiled.h"
#include "SpriteRenderer.h"
#include "D3DUtil.h"
#include <DirectXTK/Inc/CommonStates.h>
#include <DirectXTK/Inc/SpriteBatch.h>

using namespace Coo;
using namespace Coo::Graphics;

namespace
{
	std::unique_ptr<SpriteRenderer> sSpriteRenderer = nullptr;
}

void SpriteRenderer::StaticInitialize()
{
	ASSERT(sSpriteRenderer == nullptr, "[SpriteRenderer] System already initialized!");
	sSpriteRenderer = std::make_unique<SpriteRenderer>();
	sSpriteRenderer->Initialize();
}

void SpriteRenderer::StaticTerminate()
{
	if (sSpriteRenderer != nullptr) 
	{
		sSpriteRenderer->Terminate();
		sSpriteRenderer.reset();
	}
}

SpriteRenderer* SpriteRenderer::Get()
{
	ASSERT(sSpriteRenderer != nullptr, "[SpriteRenderer] No instance registed.");
	return sSpriteRenderer.get();
}

SpriteRenderer::~SpriteRenderer()
{
	ASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] Renderer not freed.");
}

void SpriteRenderer::Initialize()
{
	ASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] Already initialized!");
	ID3D11DeviceContext* context = GetContext();
	ID3D11Device* device = GetDevice();
	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	mCommonStates = std::make_unique<DirectX::CommonStates>(device);
}

void SpriteRenderer::Terminate()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Already terminated!");
	mSpriteBatch.reset();
}

void SpriteRenderer::BeginRender()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized!");
	mSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,mCommonStates->NonPremultiplied());
}

void SpriteRenderer::EndRender()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized!");
	mSpriteBatch->End();
}

void SpriteRenderer::Draw(const Texture& texture, const Math::Vector2& pos)
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized!");
	mSpriteBatch->Draw(GetShaderResourceView(texture), DirectX::XMFLOAT2{ pos.x,pos.y });
}






