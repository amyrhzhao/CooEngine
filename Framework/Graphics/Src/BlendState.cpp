#include "Precompiled.h"
#include "BlendState.h"
#include "D3DUtil.h"

using namespace Coo::Graphics;

namespace
{
	D3D11_BLEND GetSrcBlend(BlendState::Mode mode)
	{
		switch (mode)
		{
		case Coo::Graphics::BlendState::Mode::Opaque:
			return D3D11_BLEND_ONE;
			break;
		case Coo::Graphics::BlendState::Mode::AlphaBlend:
			return D3D11_BLEND_SRC_ALPHA;
			break;
		case Coo::Graphics::BlendState::Mode::AlphaPremultiplied:
			return D3D11_BLEND_ONE;
			break;
		case Coo::Graphics::BlendState::Mode::Additive:
			return D3D11_BLEND_SRC_ALPHA;
			break;
		}
		return D3D11_BLEND_ONE;
	}
	D3D11_BLEND GetDestBlend(BlendState::Mode mode)
	{
		switch (mode)
		{
		case Coo::Graphics::BlendState::Mode::Opaque:
			return D3D11_BLEND_ZERO;
			break;
		case Coo::Graphics::BlendState::Mode::AlphaBlend:
			return D3D11_BLEND_INV_SRC_ALPHA;
			break;
		case Coo::Graphics::BlendState::Mode::AlphaPremultiplied:
			return D3D11_BLEND_INV_SRC_ALPHA;
			break;
		case Coo::Graphics::BlendState::Mode::Additive:
			return D3D11_BLEND_ONE;
			break;
		}
		return D3D11_BLEND_ZERO;
	}
}

BlendState::~BlendState()
{
	ASSERT(mBlendState == nullptr, "[BlendState] Blend State not released.");
}

void BlendState::Initialize(Mode mode)
{
	D3D11_BLEND srcBlend = GetSrcBlend(mode);
	D3D11_BLEND destBlend = GetDestBlend(mode);

	D3D11_BLEND_DESC desc{};
	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);
	desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = GetDevice()->CreateBlendState(&desc, &mBlendState);
	ASSERT(SUCCEEDED(hr), "[BlendState] Failed to create Blend state.");
}

void BlendState::Terminate()
{
	SafeRelease(mBlendState);
}

void BlendState::Set()
{
	GetContext()->OMSetBlendState(mBlendState, nullptr, UINT_MAX);
}

void BlendState::ClearState()
{
	GetContext()->OMSetBlendState(nullptr, nullptr, UINT_MAX);
}