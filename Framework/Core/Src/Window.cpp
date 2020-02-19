#include "Precompiled.h"
#include "Window.h"

using namespace Coo;
using namespace Coo::Core;

LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProcA(handle, message, wParam, lParam);
}

void Window::Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height, bool maximize)
{
	mInstance = instance;
	mAppName = appName;

	// Every Windows Window requires at least one window object, Three things are involved:
	// 1)	Register a window class (where we can specify the style of window we want)
	// 2)	Create a window object (spawning the actual window)
	// 3)	Retrieve and dispatch messages for this window.

	// Register class
	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = appName;
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	RegisterClassExA(&wcex);

	// Compute the correct window dimension
	RECT rc = { 0,0,(LONG)width,(LONG)height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = rc.right - rc.left;
	const int winHeight = rc.bottom - rc.top;
	const int left = (screenWidth - winWidth) / 2;
	const int top = (screenHeight - winHeight) / 2;

	// Create window
	mWindow = CreateWindowA
	(
		appName, appName,
		WS_OVERLAPPEDWINDOW,
		left, top,
		winWidth, winHeight,
		nullptr, nullptr,
		instance, nullptr
	);

	ShowWindow(mWindow, maximize ? SW_MAXIMIZE : SW_SHOWNORMAL);

	SetCursorPos(screenWidth / 2, screenHeight / 2);
}

void Window::Terminate()
{
	DestroyWindow(mWindow);

	UnregisterClassA(mAppName.c_str(), mInstance);

	mWindow = nullptr;
	mInstance = nullptr;
}

bool Window::ProcessMessage()
{
	MSG msg = { 0 };

	bool quit = false;
	while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);

		if (WM_QUIT == msg.message)
		{
			quit = true;
		}
	}

	return quit;
}