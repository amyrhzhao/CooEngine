#ifndef INCLUDED_HELLOFSM_BONE_H
#define INCLUDED_HELLOFSM_BONE_H

#include "Coo/Inc/Coo.h"

class Bone 
{
public:
	Bone() = default;

	void Initialize();
	void Render();

	bool Spawn(const Coo::Math::Vector2& pos);
	void Kill();
	void Move(const Coo::Math::Vector2& pos);
	bool IsActive() { return mActive; }
	Coo::Math::Vector2 GetPosition() { return mPosition; }

private:
	Coo::Graphics::TextureId mTextureId;
	Coo::Math::Vector2 mPosition;
	bool mActive = false;
};

#endif // !INCLUDED_HELLOFSM_BONE_H
