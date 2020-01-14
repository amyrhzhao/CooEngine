#ifndef INCLUDED_COOENGINE_GRAPHICS_GRAPHICSYSTEM_H
#define INCLUDED_COOENGINE_GRAPHICS_GRAPHICSYSTEM_H

#include "Colors.h"

namespace Coo::Graphics {

class GraphicsSystem 
{
public://static functions 
	static void StaticInitialize(HWND window, bool fullscreen);
	static void StaticTerminate();
	static GraphicsSystem* Get();

public://non-static functions
	GraphicsSystem() = default;
	~GraphicsSystem();
	
	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator=(const GraphicsSystem&) = delete;

	void Initialize(HWND window, bool fullscreen);
	void Terminate();

	void BeginRender();
	void EndRender();

	void SetVSync(bool vSync) { mVSync = vSync ? 1 : 0; }
	void SetClearColor(Color clearColor) { mClearColor = clearColor; }

	void ToggleFullScreen();
	void Resize(uint32_t width, uint32_t height);

	void ResetRenderTarget();
	void ResetViewport();

	uint32_t GetBackBufferWidth() const;
	uint32_t GetBackBufferHeight() const;

private:
	friend LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	
	friend ID3D11Device* GetDevice();
	friend ID3D11DeviceContext* GetContext();
	
	ID3D11Device* mD3DDevice{ nullptr };
	ID3D11DeviceContext* mImmediateContext{ nullptr };

	IDXGISwapChain* mSwapChain{ nullptr };
	ID3D11RenderTargetView* mRenderTargetView{ nullptr };

	ID3D11Texture2D* mDepthStencilBuffer{ nullptr };
	ID3D11DepthStencilView* mDepthStencilView{ nullptr };

	DXGI_SWAP_CHAIN_DESC mSwapChainDesc{};
	D3D11_VIEWPORT mViewport{};

	UINT mVSync = 1;
	Color mClearColor = Colors::White;
};

} // namespace Coo::Graphics


#endif // !INCLUDED_COOENGINE_GRAPHICS_GRAPHICSYSTEM_H
