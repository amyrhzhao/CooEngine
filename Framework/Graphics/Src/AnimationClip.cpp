#include "Precompiled.h"
#include "AnimationClip.h"
#include "ImGui/Inc/imgui.h"

using namespace Coo::Graphics;
using namespace Coo::Math;

bool AnimationClip::GetTransform(float time, uint32_t boneIndex, Matrix4 & transform) const
{
	auto& boneAnim = boneAnimations[boneIndex];
	if (boneAnim) 
	{
		transform = boneAnim->animation.GetTransform(time);
		return true;
	}
	return false;
}

void AnimationClip::DuiShowInfo() const
{
	ImGui::Text("name: %s", name.data());
	ImGui::Text("duration: %f",duration);
	ImGui::Text("ticksPersecond: %f", ticksPersecond);
}
