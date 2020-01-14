#include "Precompiled.h"
#include "Animation.h"

using namespace Coo::Graphics;
using namespace Coo::Math;
using namespace Coo;

Math::Matrix4 Coo::Graphics::Animation::GetTransform(float time) const
{
	return GetScaleTransform(time) * GetRotationTransform(time) * GetPositionTransform(time);
}

Math::Matrix4 Coo::Graphics::Animation::GetPositionTransform(float time) const
{
	Matrix4 translation;
	for (size_t i = 0; i < mPositionKeyframes.size(); i++) 
	{
		if (i == mPositionKeyframes.size() - 1)
		{
			translation = Translate(mPositionKeyframes[i].value);
			break;
		}
		if(time >= mPositionKeyframes[i].time && time < mPositionKeyframes[i+1].time)
		{
			float timePass = time - mPositionKeyframes[i].time;
			float timeToEndFrame = mPositionKeyframes[i + 1].time - mPositionKeyframes[i].time;
			float t = timePass / timeToEndFrame;
			translation = Translate(Lerp(mPositionKeyframes[i].value, mPositionKeyframes[i + 1].value, t));
			break;
		}
	}
	return translation;
}

Math::Matrix4 Coo::Graphics::Animation::GetRotationTransform(float time) const
{
	Matrix4 rotation;
	for (size_t i = 0; i < mRotationKeyframes.size(); i++)
	{
		if (i == mRotationKeyframes.size() - 1)
		{
			rotation = MatrixRotationQuaternion(mRotationKeyframes[i].value);
			break;
		}
		if (time >= mRotationKeyframes[i].time && time < mRotationKeyframes[i + 1].time)
		{
			float timePass = time - mRotationKeyframes[i].time;
			float timeToEndFrame = mRotationKeyframes[i + 1].time - mRotationKeyframes[i].time;
			float t = timePass / timeToEndFrame;
			rotation = MatrixRotationQuaternion(Slerp(mRotationKeyframes[i].value, mRotationKeyframes[i + 1].value, t));
			break;
		}
	}
	return rotation;
}

Math::Matrix4 Coo::Graphics::Animation::GetScaleTransform(float time) const
{
	Matrix4 scaling;
	for (size_t i = 0; i < mScaleKeyframes.size(); i++)
	{
		if (i == mScaleKeyframes.size() - 1)
		{
			scaling = Scale(mScaleKeyframes[i].value);
			break;
		}
		if (time >= mScaleKeyframes[i].time && time < mScaleKeyframes[i + 1].time)
		{
			float timePass = time - mScaleKeyframes[i].time;
			float timeToEndFrame = mScaleKeyframes[i + 1].time - mScaleKeyframes[i].time;
			float t = timePass / timeToEndFrame;
			scaling = Scale(Lerp(mScaleKeyframes[i].value, mScaleKeyframes[i + 1].value, t));
			break;
		}
	}
	return scaling;
}
