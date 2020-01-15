#include "Precompiled.h"
#include "Animation.h"

using namespace Coo::Graphics;
using namespace Coo::Math;
using namespace Coo;

Math::Matrix4 Coo::Graphics::Animation::GetTransform(float time) const
{
	return GetScaleTransform(time) * GetRotationTransform(time) * GetPositionTransform(time);
}

Math::Vector3 Coo::Graphics::Animation::GetPosition(float time) const
{
	Vector3 position;
	for (size_t i = 0; i < mPositionKeyframes.size(); i++)
	{
		if (i == mPositionKeyframes.size() - 1)
		{
			position = mPositionKeyframes[i].value;
			break;
		}
		if (time >= mPositionKeyframes[i].time && time < mPositionKeyframes[i + 1].time)
		{
			float timePass = time - mPositionKeyframes[i].time;
			float timeToEndFrame = mPositionKeyframes[i + 1].time - mPositionKeyframes[i].time;
			float t = timePass / timeToEndFrame;
			position = Lerp(mPositionKeyframes[i].value, mPositionKeyframes[i + 1].value, t);
			break;
		}
	}
	return position;
}

Math::Matrix4 Coo::Graphics::Animation::GetPositionTransform(float time) const
{
	Matrix4 translation;
	translation = Translate(GetPosition(time));
	return translation;
}

Math::Quaternion Coo::Graphics::Animation::GetRotation(float time) const
{
	Quaternion rotation;
	for (size_t i = 0; i < mRotationKeyframes.size(); i++)
	{
		if (i == mRotationKeyframes.size() - 1)
		{
			rotation = mRotationKeyframes[i].value;
			break;
		}
		if (time >= mRotationKeyframes[i].time && time < mRotationKeyframes[i + 1].time)
		{
			float timePass = time - mRotationKeyframes[i].time;
			float timeToEndFrame = mRotationKeyframes[i + 1].time - mRotationKeyframes[i].time;
			float t = timePass / timeToEndFrame;
			rotation = Slerp(mRotationKeyframes[i].value, mRotationKeyframes[i + 1].value, t);
			break;
		}
	}
	return rotation;
}

Math::Matrix4 Coo::Graphics::Animation::GetRotationTransform(float time) const
{
	Matrix4 rotation;
	rotation = MatrixRotationQuaternion(GetRotation(time));
	return rotation;
}

Math::Vector3 Coo::Graphics::Animation::GetScale(float time) const
{
	Vector3 scaling;
	for (size_t i = 0; i < mScaleKeyframes.size(); i++)
	{
		if (i == mScaleKeyframes.size() - 1)
		{
			scaling = mScaleKeyframes[i].value;
			break;
		}
		if (time >= mScaleKeyframes[i].time && time < mScaleKeyframes[i + 1].time)
		{
			float timePass = time - mScaleKeyframes[i].time;
			float timeToEndFrame = mScaleKeyframes[i + 1].time - mScaleKeyframes[i].time;
			float t = timePass / timeToEndFrame;
			scaling = Lerp(mScaleKeyframes[i].value, mScaleKeyframes[i + 1].value, t);
			break;
		}
	}
	return scaling;
}

Math::Matrix4 Coo::Graphics::Animation::GetScaleTransform(float time) const
{
	Matrix4 scaling;
	scaling = Scale(GetScale(time));
	return scaling;
}
