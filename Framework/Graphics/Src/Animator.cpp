#include "Precompiled.h"
#include "AnimationClip.h"
#include "AnimationBank.h"
#include "ImGui/Inc/imgui.h"
#include "Animator.h"

using namespace Coo::Graphics;
using namespace Coo::Math;

namespace
{
	int currAnim = -1;
}

const AnimationClip* Animator::GetCurrAnimationClip(bool playing) const
{
	int index;
	if (playing) { index = animationClip; }
	else { index = currAnim; }
	const AnimationClip* animClip = nullptr;
	if (index >= 0 && index < static_cast<int>(mAnimationBank->animationClips.size()))
	{
		animClip = mAnimationBank->animationClips[index].get();
	}
	return animClip;
}

void Animator::PlayAnimationClip(int clip)
{
	if (mAnimationBank && clip >= static_cast<int>(mAnimationBank->animationClips.size()))
	{
		animationClip = static_cast<int>(mAnimationBank->animationClips.size()) - 1;
	}
	else if (clip < 0)
	{
		animationClip = -1;
	}
	else
	{
		animationClip = clip;
	}
	timer = 0.0f;
	playSpeed = 1.0f;
}

void Animator::UpdateAnimation(float deltaTime)
{
	const AnimationClip* animClip = GetCurrAnimationClip();
	if (!paused)
	{
		timer += deltaTime * (animClip ? animClip->ticksPersecond : 1.0f) * playSpeed;
	}
	if (animClip && timer > animClip->duration)
	{
		if(loop)
		{
			timer = 0.0f;
		}
		else
		{
			timer = animClip->duration;
		}
	}

}

bool Coo::Graphics::Animator::GetBoneTransform(size_t index, Matrix4& transform) const
{
	const AnimationClip* animClip = GetCurrAnimationClip();
	return animClip && animClip->GetTransform(timer, static_cast<uint32_t>(index), transform);
}

void Coo::Graphics::Animator::Bind(AnimationBank * animationBank)
{
	mAnimationBank = animationBank;
}

void Coo::Graphics::Animator::DebugUI()
{
	const AnimationClip* selectClip = GetCurrAnimationClip(false);
	static const char* defaultBoneStr = "default bone transform";
	ImGui::Begin("Animator", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::DragInt("Animation Clip", &currAnim, 0.1f, -1, (mAnimationBank ? static_cast<int>(mAnimationBank->animationClips.size()) : 0) - 1);
	if (ImGui::CollapsingHeader("Current Selected")) 
	{
		if (selectClip)
		{
			selectClip->DuiShowInfo();
		}
		else
		{
			ImGui::Text(defaultBoneStr);
		}
	}
	if (ImGui::Button("Play selected"))
	{
		PlayAnimationClip(currAnim);
	}
	if (ImGui::CollapsingHeader("Current Playing")) 
	{
		const AnimationClip* animClip = GetCurrAnimationClip();
		if (animClip)
		{
			animClip->DuiShowInfo();
			ImGui::Checkbox("Loop", &loop);
			ImGui::DragFloat("Time", &timer, 0.1f, 0.0f, animClip->duration);
			ImGui::DragFloat("Play Speed", &playSpeed, 0.1f, 0.0f, 20.0f);
		}
		else
		{
			ImGui::Text("default bone transform");
		}
	}
	if (ImGui::Button(paused ? "Unpause" : "Pause")) 
	{
		paused = !paused;
	}
	ImGui::End();
}
