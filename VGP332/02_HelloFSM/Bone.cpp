#include "Bone.h"

void Bone::Initialize()
{
	mTextureId = Coo::Graphics::TextureManager::Get()->LoadTexture("Food_27.png");
}

void Bone::Render()
{
	if (IsActive())
	{
		auto texture = Coo::Graphics::TextureManager::Get()->GetTexture(mTextureId);
		Coo::Graphics::SpriteRenderer::Get()->Draw(*texture, mPosition);
		Coo::Graphics::SimpleDraw::AddScreenCircle(mPosition, 100.0f, Coo::Graphics::Colors::SeaGreen);
	}
}

bool Bone::Spawn(const Coo::Math::Vector2 & pos)
{
	if (!IsActive())
	{
		mPosition = pos;
		mActive = true;
		return true;
	}
	return false;
}

void Bone::Kill()
{
	mActive = false;
}

void Bone::Move(const Coo::Math::Vector2 & pos)
{
	mPosition = pos;
}
