#include "Precompiled.h"
#include "GraphicsSystem.h"

using namespace Coo;
using namespace Coo::Graphics;

namespace
{
	std::unique_ptr<GraphicsSystem> sGraphicSystem;
	Core::WindowMessageHandler sWindowMessageHandler;
}

LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (sGraphicSystem)
	{
		switch (message)
		{
			case WM_SIZE:
			{
				const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
				const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
				sGraphicSystem->Resize(width, height);
				break;
			}
		}
	}

	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(sGraphicSystem == nullptr, "[Graphics::GraphicsSystem] System already initialized.");
	sGraphicSystem = std::make_unique<GraphicsSystem>();
	sGraphicSystem->Initialize(window, fullscreen);
}

void GraphicsSystem::StaticTerminate()
{
	if (sGraphicSystem != nullptr)
	{
		sGraphicSystem->Terminate();
		sGraphicSystem.reset();
	}
}

GraphicsSystem* GraphicsSystem::Get()
{
	ASSERT(sGraphicSystem != nullptr, "[Graphics::GraphicsSystem] No system registered.");
	return sGraphicSystem.get();
}

GraphicsSystem::~GraphicsSystem()
{
	ASSERT(sGraphicSystem == nullptr, "[Graphics::GraphicsSystem] Terminate() must be called to clean up!");
}

void GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
	RECT clientRect = {};
	GetClientRect(window, &clientRect);
	UINT width = (UINT)(clientRect.right - clientRect.left);
	UINT height = (UINT)(clientRect.bottom - clientRect.top);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;///1
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;///?
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;///?

	UINT createDeviceFlags = 0;///?
	//const D3D_FEATURE_LEVEL featureLevel = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,///?
		featureLevelArray,///&featureLevel,
		2,///1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mD3DDevice,
		&featureLevel,///nullptr,
		&mImmediateContext
	);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to create device or swap chain.");

	// Cache swap chain description
	mSwapChain->GetDesc(&mSwapChainDesc);

	// Initialize render target and depth stencil views
	Resize(GetBackBufferWidth(), GetBackBufferHeight());

	// Hook application to window's procedure
	sWindowMessageHandler.Hook(window, GraphicsSystemMessageHandler);
}

void GraphicsSystem::Terminate()
{
	// Restore original window's procedure
	sWindowMessageHandler.Unhook();

	SafeRelease(mDepthStencilView);
	SafeRelease(mDepthStencilBuffer);
	SafeRelease(mRenderTargetView);
	SafeRelease(mSwapChain);
	SafeRelease(mImmediateContext);
	SafeRelease(mD3DDevice);
}

void GraphicsSystem::BeginRender()
{
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, (FLOAT*)(&mClearColor));
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsSystem::EndRender()
{
	mSwapChain->Present(mVSync, 0);
}

void GraphicsSystem::ToggleFullScreen()
{
	BOOL fullscreen;
	mSwapChain->GetFullscreenState(&fullscreen, nullptr);
	mSwapChain->SetFullscreenState(!fullscreen, nullptr);
}

void GraphicsSystem::Resize(uint32_t width, uint32_t height)
{
	mImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilBuffer);

	HRESULT hr;
	if (width != GetBackBufferWidth() || height != GetBackBufferHeight())
	{
		hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to resize swap chain buffer.");

		// Cache swap chain description
		mSwapChain->GetDesc(&mSwapChainDesc);
	}

	// Create a render target view
	ID3D11Texture2D* backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to access swap chain buffer.");

	hr = mD3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	SafeRelease(backBuffer);
	ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to access render target view.");

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	depthBufferDesc.Width = GetBackBufferWidth();
	depthBufferDesc.Height = GetBackBufferHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	hr = mD3DDevice->CreateTexture2D(&depthBufferDesc, nullptr, &mDepthStencilBuffer);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to create depth stencil buffer.");

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = depthBufferDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, &dsvDesc, &mDepthStencilView);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to create depth stencil view.");

	// Set the render target view and depth stencil view to Ouput Merger
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// Setup the viewport
	mViewport.Width = (FLOAT)width;
	mViewport.Height = (FLOAT)height;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicsSystem::ResetRenderTarget()
{
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void GraphicsSystem::ResetViewport() 
{
	mImmediateContext->RSSetViewports(1, &mViewport);
}

uint32_t Coo::Graphics::GraphicsSystem::GetBackBufferWidth() const
{
	return mSwapChainDesc.BufferDesc.Width;
}

uint32_t Coo::Graphics::GraphicsSystem::GetBackBufferHeight() const
{
	return mSwapChainDesc.BufferDesc.Height;
}
