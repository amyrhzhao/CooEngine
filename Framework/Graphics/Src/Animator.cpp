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
	if (playing) { index = mCurrAnimationClip; }
	else { index = currAnim; }
	const AnimationClip* animClip = nullptr;
	if (index >= 0 && index < static_cast<int>(mAnimationBank->animationClips.size()))
	{
		animClip = mAnimationBank->animationClips[index].get();
	}
	return animClip;
}

void Animator::PlayAnimationClip(int clip, bool playFromBegining)
{
	if (mAnimationBank && clip >= static_cast<int>(mAnimationBank->animationClips.size()))
	{
		mCurrAnimationClip = static_cast<int>(mAnimationBank->animationClips.size()) - 1;
	}
	else if (clip < 0)
	{
		mCurrAnimationClip = -1;
	}
	else
	{
		mCurrAnimationClip = clip;
		if (playFromBegining)
		{
			mTimers[mCurrAnimationClip] = 0.0f;
		}
		//playSpeed[animationClip] = 1.0f;
	}
}

void Coo::Graphics::Animator::AddTransition(int clip, float exitTime, float transitionDuration)
{
	mTransiting = true;
	mInTransition = false;
	mNextAnimationClip = clip;
	mExitTime = exitTime;
	mTransitionDuration = transitionDuration;
	mTransitionDurationPassed = 0.0f;
}

void Animator::UpdateAnimation(float deltaTime)
{
	const AnimationClip* animClip = GetCurrAnimationClip();
	if (animClip)
	{
		float lastTimer = mTimers[mCurrAnimationClip];
		if (!mPaused)
		{
			mTimers[mCurrAnimationClip] += deltaTime * (animClip ? animClip->ticksPersecond : 1.0f) * mPlaySpeed[mCurrAnimationClip];
		}
		if (animClip && mTimers[mCurrAnimationClip] > animClip->duration)
		{
			if (mIsLoop[mCurrAnimationClip])
			{
				mTimers[mCurrAnimationClip] = 0.0f;
			}
			else
			{
				mTimers[mCurrAnimationClip] = animClip->duration;
			}
		}
		if (mTransiting && lastTimer / animClip->duration < mExitTime && mTimers[mCurrAnimationClip] / animClip->duration > mExitTime)
		{
			mInTransition = true;
			mTransiting = false;
			mTimers[mNextAnimationClip] = 0.0f;
		}
		if (mInTransition)
		{
			mTransitionDurationPassed += deltaTime / animClip->duration * animClip->ticksPersecond;
			mTimers[mNextAnimationClip] += deltaTime * mAnimationBank->animationClips[mNextAnimationClip]->ticksPersecond;
			if (mTimers[mNextAnimationClip] > mAnimationBank->animationClips[mNextAnimationClip]->duration)
			{
				if (mIsLoop[mCurrAnimationClip])
				{
					mTimers[mNextAnimationClip] = 0.0f; 
				}
				else
				{
					mTimers[mNextAnimationClip] = mAnimationBank->animationClips[mNextAnimationClip]->duration;
				}
			}
			if (mTransitionDurationPassed > mTransitionDuration)
			{
				mInTransition = false;
				mCurrAnimationClip = mNextAnimationClip;
			}
		}
	}

}

bool Coo::Graphics::Animator::GetBoneTransform(size_t index, Matrix4& transform) const
{
	const AnimationClip* animClip = GetCurrAnimationClip();
	if (!mInTransition)
	{
		return animClip && animClip->GetTransform(mTimers[mCurrAnimationClip], static_cast<uint32_t>(index), transform);
	}
	else
	{
		Vector3 currPos;
		Quaternion currRot;
		Vector3 currScale;
		if (animClip && animClip->boneAnimations[index])
		{
			const Animation& anim = animClip->boneAnimations[index]->animation;
			currPos = anim.GetPosition(mTimers[mCurrAnimationClip]);
			currRot = anim.GetRotation(mTimers[mCurrAnimationClip]);
			currScale = anim.GetScale(mTimers[mCurrAnimationClip]);
		}
		const AnimationClip* nextAnimClip = mAnimationBank->animationClips[mNextAnimationClip].get();
		Vector3 nextPos;
		Quaternion nextRot;
		Vector3 nextScale;
		if (nextAnimClip && nextAnimClip->boneAnimations[index])
		{
			const Animation& nextAnim = nextAnimClip->boneAnimations[index]->animation;
			nextPos = nextAnim.GetPosition(mTimers[mNextAnimationClip]);
			nextRot = nextAnim.GetRotation(mTimers[mNextAnimationClip]);
			nextScale = nextAnim.GetScale(mTimers[mNextAnimationClip]);
		}
		float t = mTransitionDurationPassed / mTransitionDuration;
		Vector3 pos = Lerp(currPos, nextPos, t);
		Quaternion rot = Slerp(currRot, nextRot, t);
		Vector3 scaling = Lerp(currScale, nextScale, t);
		transform = Scale(scaling) * MatrixRotationQuaternion(rot) * Translate(pos);
		return true;
	}
}

void Coo::Graphics::Animator::Bind(AnimationBank * animationBank)
{
	mAnimationBank = animationBank;
	mTimers.resize(animationBank->animationClips.size(), 0.0f);
	mPlaySpeed.resize(animationBank->animationClips.size(), 1.0f);
	mIsLoop.resize(animationBank->animationClips.size(), true);
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
	if (ImGui::Button("Resume selected"))
	{
		PlayAnimationClip(currAnim, false);
	}
	if (ImGui::CollapsingHeader("Transition"))
	{
		ImGui::DragInt("Transit to", &mNextAnimationClip, 0.1f, 0, (mAnimationBank ? static_cast<int>(mAnimationBank->animationClips.size()) : 0) - 1);
		ImGui::DragFloat("Exit Time", &mExitTime, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("Transition Duration", &mTransitionDuration, 0.1f, 0.0f, 1.0f);
		if (ImGui::Button("Add Transition"))
		{
			AddTransition(mNextAnimationClip, mExitTime, mTransitionDuration);
		}
	}
	if (ImGui::CollapsingHeader("Current Playing"))
	{
		const AnimationClip* animClip = GetCurrAnimationClip();
		if (animClip)
		{
			animClip->DuiShowInfo();
			bool isLoop = mIsLoop[mCurrAnimationClip];
			ImGui::Checkbox("Loop", &isLoop);
			mIsLoop[mCurrAnimationClip] = isLoop;
			ImGui::DragFloat("Time", &mTimers[mCurrAnimationClip], 0.01f, 0.0f, animClip->duration);
			ImGui::DragFloat("Play Speed", &mPlaySpeed[mCurrAnimationClip], 0.01f, 0.0f, 20.0f);
		}
		else
		{
			ImGui::Text("default bone transform");
		}
	}
	if (ImGui::Button(mPaused ? "Unpause" : "Pause"))
	{
		mPaused = !mPaused;
	}
	ImGui::End();
}
