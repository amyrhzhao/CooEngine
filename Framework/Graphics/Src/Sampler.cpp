#include "Precompiled.h"
#include "Sampler.h"
#include "D3DUtil.h"

using namespace Coo;
using namespace Coo::Graphics;

namespace
{
	D3D11_FILTER GetFilter(Sampler::Filter filter)
	{
		switch (filter)
		{
		case Coo::Graphics::Sampler::Filter::Point:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case Coo::Graphics::Sampler::Filter::Linear:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case Coo::Graphics::Sampler::Filter::Anisotropic:
			return D3D11_FILTER_ANISOTROPIC;
			break;
		default:
			return D3D11_FILTER_ANISOTROPIC;
			break;
		}
	}

	D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(Sampler::AddressMode addressMode) 
	{
		switch (addressMode)
		{
		case Coo::Graphics::Sampler::AddressMode::Border:
			return D3D11_TEXTURE_ADDRESS_BORDER;
			break;
		case Coo::Graphics::Sampler::AddressMode::Clamp:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
			break;
		case Coo::Graphics::Sampler::AddressMode::Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
			break;
		case Coo::Graphics::Sampler::AddressMode::Wrap:
			return D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		default:
			return D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		}
	}
}

Sampler::~Sampler()
{
	ASSERT(mSampler == nullptr,
		"[Sampler] Terminate must be called before destruction.");
}

void Sampler::Initialize(Filter filter, AddressMode addressMode)
{
	D3D11_FILTER d3dFilter = GetFilter(filter);
	D3D11_TEXTURE_ADDRESS_MODE d3dAddressMode = GetAddressMode(addressMode);

	D3D11_SAMPLER_DESC sampDesc{};
	sampDesc.Filter = d3dFilter;
	sampDesc.AddressU = d3dAddressMode;
	sampDesc.AddressV = d3dAddressMode;
	sampDesc.AddressW = d3dAddressMode;
	sampDesc.BorderColor[0] = 1.0f;
	sampDesc.BorderColor[1] = 0.0f;
	sampDesc.BorderColor[2] = 1.0f;
	sampDesc.BorderColor[3] = 1.0f;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto device = GetDevice();
	auto hr = device->CreateSamplerState(&sampDesc, &mSampler);
	ASSERT(SUCCEEDED(hr), "[Sampler] Failed to create sampler state.");
}

void Sampler::Terminate()
{
	SafeRelease(mSampler);
}

void Sampler::BindPS(uint32_t slot) const
{
	auto context = GetContext();
	context->PSSetSamplers(slot, 1, &mSampler);
}
