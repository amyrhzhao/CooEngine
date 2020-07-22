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

	DirectX::XMFLOAT2 GetOrigin(uint32_t width, uint32_t height, Pivot pivot)
	{
		DirectX::XMFLOAT2 offset = { 0.0f, 0.0f };
		switch (pivot)
		{
		case Coo::Graphics::Pivot::TopLeft:
			offset = { 0.0f, 0.0f };
			break;
		case Coo::Graphics::Pivot::Top:
			offset = { 0.5f, 0.0f };
			break;
		case Coo::Graphics::Pivot::TopRight:
			offset = { 1.0f, 0.0f };
			break;
		case Coo::Graphics::Pivot::Left:
			offset = { 0.0f, 0.5f };
			break;
		case Coo::Graphics::Pivot::Center:
			offset = { 0.5f, 0.5f };
			break;
		case Coo::Graphics::Pivot::Right:
			offset = { 0.5f, 1.0f };
			break;
		case Coo::Graphics::Pivot::BottomLeft:
			offset = { 0.0f, 1.0f };
			break;
		case Coo::Graphics::Pivot::Bottom:
			offset = { 0.5f, 1.0f };
			break;
		case Coo::Graphics::Pivot::BottomRight:
			offset = { 1.0f, 1.0f };
			break;
		}
		return { width * offset.x, height * offset.y };
	}

	DirectX::XMFLOAT2 Convert(Coo::Math::Vector2 vec)
	{
		return { vec.x, vec.y };
	}
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
	mSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, mCommonStates->NonPremultiplied());
}

void SpriteRenderer::EndRender()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized!");
	mSpriteBatch->End();
}

void SpriteRenderer::Draw(const Texture& texture, const Math::Vector2& pos, Pivot pivot)
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized!");
	mSpriteBatch->Draw(GetShaderResourceView(texture), DirectX::XMFLOAT2{ pos.x,pos.y });
}

void SpriteRenderer::Draw(const Texture& texture, const Math::AABB& rec, const Math::Vector2& pos, float rotation = 0.0f, const Math::Vector2& scale = { 1.0f, 1.0f }, Pivot pivot = Pivot::TopLeft)
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized!");
	RECT rect;
	rect.left = static_cast<LONG>(rec.center.x - rec.extend.x);
	rect.top = static_cast<LONG>(rec.center.y - rec.extend.y);
	rect.right = static_cast<LONG>(rec.center.x + rec.extend.x);
	rect.bottom = static_cast<LONG>(rec.center.y + rec.extend.y);
	auto origin = GetOrigin(static_cast<uint32_t>(rec.extend.x), static_cast<uint32_t>(rec.extend.y), pivot);
	mSpriteBatch->Draw(GetShaderResourceView(texture), Convert(pos), &rect, DirectX::Colors::White, rotation, origin, Convert(scale));
}