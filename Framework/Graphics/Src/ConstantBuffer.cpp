#include "Precompiled.h"
#include "ConstantBuffer.h"
#include "D3DUtil.h"
using namespace Coo::Graphics;

void ConstantBuffer::Initialize(uint32_t constantSize, uint32_t numConstants)
{
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.ByteWidth = numConstants * constantSize;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffer);
}

void Coo::Graphics::ConstantBuffer::Initialize(uint32_t bufferSize, const void* initData)
{
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.ByteWidth = bufferSize;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA subresourceData{};
	subresourceData.pSysMem = initData;

	GetDevice()->CreateBuffer(&constantBufferDesc, initData ? &subresourceData : nullptr, &mConstantBuffer);
}

void Coo::Graphics::ConstantBuffer::Set(const void* constants) const
{
	GetContext()->UpdateSubresource(mConstantBuffer, 0, nullptr, constants, 0, 0);
}

void Coo::Graphics::ConstantBuffer::BindVS(uint32_t slot) const
{
	GetContext()->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void Coo::Graphics::ConstantBuffer::BindGS(uint32_t slot) const
{
	GetContext()->GSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void Coo::Graphics::ConstantBuffer::BindPS(uint32_t slot) const 
{
	GetContext()->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}
