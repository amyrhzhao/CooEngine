#include "Precompiled.h"
#include "VertexShader.h"
#include "D3DUtil.h"
#include "VertexType.h"

using namespace Coo;
using namespace Coo::Graphics;

namespace
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexLayout(uint32_t vertexFormat)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		if (vertexFormat & VE_Position)
			desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Normal)
			desc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Tangent)
			desc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Color)
			desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Texcoord)
			desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_BlendIndex)
			desc.push_back({ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_BlendWeight)
			desc.push_back({ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		
		return desc;
	}
}

void VertexShader::Initialize(const wchar_t* filename, uint32_t vertexFormat)
{
	auto elementDescs = GetVertexLayout(vertexFormat);
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	auto device = GetDevice();
	HRESULT hr;
	// Compile vertex shader code
	//DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	hr = D3DCompileFromFile(
		filename,
		nullptr, nullptr,
		"VS", "vs_5_0", 0, 0,
		&shaderBlob,
		&errorBlob);
	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	// Create vertex shader and input layout descriptor
	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	hr = device->CreateInputLayout(
		elementDescs.data(), static_cast<UINT>(std::size(elementDescs)),
		shaderBlob->GetBufferPointer(),
		static_cast<size_t>(shaderBlob->GetBufferSize()),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void VertexShader::Terminate()
{
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
}

void VertexShader::Bind()
{
	auto context = GetContext();
	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, nullptr, 0);
}
