#include <Graphics/Inc/Graphics.h>
#include <Core/Inc/Core.h>
#include <Math/Inc/CooMath.h>
#include <Graphics/Src/D3DUtil.h>
using namespace Coo::Graphics;
using namespace Coo::Math;
using namespace Coo::Core;

// Moved to VertexType.h in Coo::Graphics
//struct Vertex
//{
//	Vector3 position;
//	float r, g, b, a;
//};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Set up application window
	Window myWindow;
	myWindow.Initialize(hInstance, "Hello Triangle", 1280, 720);

	// Initialize graphic system
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	// Define vertex data
	const VertexPC vertices[] =
	{
		{ {  0.0f, 0.5f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} },
		{ {  0.5f, 0.0f, 0.0f }, {0.0f, 1.0f, 0.0f, 1.0f} },
		{ { -0.5f, 0.0f, 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} }
	};
	const D3D11_INPUT_ELEMENT_DESC elementDescs[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	auto device = GetDevice();

	// Allocate memory in VRAM for vertex buffer and initialize
	// with vertex data
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(std::size(vertices) * sizeof(VertexPC));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer = nullptr;
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &initData, &vertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

	// Compile vertex shader code
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr, nullptr,
		"VS", "vs_5_0", 0, 0,
		&shaderBlob,
		&errorBlob);
	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	// Create vertex shader and input layout descriptor
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&vertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	hr = device->CreateInputLayout(
		elementDescs, static_cast<UINT>(std::size(elementDescs)),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&inputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	// Compile pixel shader
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr, nullptr,
		"PS", "ps_5_0", 0, 0,
		&shaderBlob,
		&errorBlob);
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader. Error: %s", (const char*)errorBlob->GetBufferPointer());

	// Create pixel shader
	ID3D11PixelShader* pixelShader = nullptr;
	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&pixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");

	// Release blobs
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();
		if (GetAsyncKeyState(VK_ESCAPE))
			done = true;

		// Run game logic
		GraphicsSystem::Get()->BeginRender();

		auto context = GetContext();

		context->IASetInputLayout(inputLayout);
		context->VSSetShader(vertexShader, nullptr, 0);
		context->PSSetShader(pixelShader, nullptr, 0);

		UINT stride = sizeof(VertexPC);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		// Draw stuff
		context->Draw(static_cast<UINT>(std::size(vertices)), 0);

		GraphicsSystem::Get()->EndRender();
	}

	SafeRelease(pixelShader);
	SafeRelease(inputLayout);
	SafeRelease(vertexShader);
	SafeRelease(vertexBuffer);

	GraphicsSystem::StaticTerminate();

	myWindow.Terminate();
	return 0;
}