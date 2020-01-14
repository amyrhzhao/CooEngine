#include "Precompiled.h"
#include "GeometryShader.h"
#include "D3DUtil.h"

using namespace Coo::Graphics;

void Coo::Graphics::GeometryShader::Initialize(const wchar_t * filename)
{
	auto device = GetDevice();
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr;
	// Compile geometry shader
	hr = D3DCompileFromFile(
		filename,
		nullptr, nullptr,
		"GS", "gs_5_0", 0, 0,
		&shaderBlob,
		&errorBlob);
	ASSERT(SUCCEEDED(hr), "Failed to compile geometry shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	// Create geometry shader
	hr = device->CreateGeometryShader(
		shaderBlob->GetBufferPointer(), 
		shaderBlob->GetBufferSize(),
		nullptr,
		&mGeometryShader);
	ASSERT(SUCCEEDED(hr), "Failed to create geometry shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void Coo::Graphics::GeometryShader::Terminate()
{
	SafeRelease(mGeometryShader);
}

void Coo::Graphics::GeometryShader::Bind()
{
	auto context = GetContext();
	context->GSSetShader(mGeometryShader, nullptr, 0);
}

void Coo::Graphics::GeometryShader::Unbind()
{
	auto context = GetContext();
	context->GSSetShader(nullptr, nullptr, 0);
}