#include "Precompiled.h"
#include "Texture.h"
#include "D3DUtil.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace Coo;
using namespace Coo::Graphics;

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr,
		"[Texture] Terminate must be called before destruction.");
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
	auto device = GetDevice();
	auto context = GetContext();
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		device, context, fileName.c_str(), nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to load texture %ls.", fileName.c_str());

	// Cache the texture dimensions
	ID3D11Resource* resource = nullptr;
	mShaderResourceView->GetResource(&resource);

	ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(resource);
	D3D11_TEXTURE2D_DESC desc = {};
	texture->GetDesc(&desc);

	mWidth = desc.Width;
	mHeight = desc.Height;
}

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
	ASSERT(mShaderResourceView != nullptr,"[Texture] Texture not initialized yet.");
	auto context = GetContext();
	context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Coo::Graphics::Texture::BindVS(uint32_t slot) const
{
	ASSERT(mShaderResourceView != nullptr, "[Texture] Texture not initialized yet.");
	auto context = GetContext();
	context->VSSetShaderResources(slot, 1, &mShaderResourceView);
}
