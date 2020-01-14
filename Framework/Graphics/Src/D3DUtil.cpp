#include "Precompiled.h"
#include "D3DUtil.h"

#include "Texture.h"
#include "GraphicsSystem.h"

ID3D11ShaderResourceView* Coo::Graphics::GetShaderResourceView(const Texture& texture)
{
	return texture.mShaderResourceView;
}

ID3D11Device * Coo::Graphics::GetDevice()
{
	return GraphicsSystem::Get()->mD3DDevice;
}

ID3D11DeviceContext * Coo::Graphics::GetContext()
{
	return GraphicsSystem::Get()->mImmediateContext;
}
