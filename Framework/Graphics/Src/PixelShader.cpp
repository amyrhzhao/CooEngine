#include "Precompiled.h"
#include "PixelShader.h"
#include "D3DUtil.h"
using namespace Coo::Graphics;
void PixelShader::Initialize(const wchar_t* filename)
{
	auto device = GetDevice();
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr;
	// Compile pixel shader
	hr = D3DCompileFromFile(
		filename,
		nullptr, nullptr,
		"PS", "ps_5_0", 0, 0,
		&shaderBlob,
		&errorBlob);
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	// Create pixel shader
	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");
}

void PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}

void PixelShader::Bind()
{
	auto context = GetContext();
	context->PSSetShader(mPixelShader, nullptr, 0);
}
