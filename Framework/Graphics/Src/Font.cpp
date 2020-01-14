#include "Precompiled.h"
#include "Font.h"

#include "D3DUtil.h"
#include <FW1FontWrapper_1_1/FW1FontWrapper.h>

#pragma comment(lib, "FW1FontWrapper.lib")

using namespace Coo::Graphics;

void Font::Initialize()
{
	ID3D11Device* device = GetDevice();
	FW1CreateFactory(FW1_VERSION, &mFW1Factory);
	mFW1Factory->CreateFontWrapper(device, L"Consolas", &mFontWrapper);
}

void Font::Terminate()
{
	SafeRelease(mFontWrapper);
	SafeRelease(mFW1Factory);
}

void Font::Draw(const wchar_t* str, float x, float y, float size, uint32_t color)
{
	ID3D11DeviceContext* context = GetContext();
	mFontWrapper->DrawString(context, str, x, y, size, color, FW1_RESTORESTATE);
}