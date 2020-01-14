#include "Precompiled.h"
#include "Animation.h"
#include "AnimationBuilder.h"

using namespace Coo::Graphics;
using namespace Coo::Math;
AnimationBuilder& Coo::Graphics::AnimationBuilder::AddPositionKey(const Coo::Math::Vector3& position, float time)
{
	mAnimation.mPositionKeyframes.emplace_back(Keyframe<Vector3>{position, time});
	return *this;
}

AnimationBuilder & Coo::Graphics::AnimationBuilder::AddRotationKey(const Coo::Math::Quaternion& rotation, float time)
{
	mAnimation.mRotationKeyframes.emplace_back(Keyframe<Quaternion>{rotation, time});
	return *this;
}

AnimationBuilder & Coo::Graphics::AnimationBuilder::AddScaleKey(const Coo::Math::Vector3& scale, float time)
{
	mAnimation.mScaleKeyframes.emplace_back(Keyframe<Vector3>{scale, time});
	return *this;
}

AnimationBuilder & Coo::Graphics::AnimationBuilder::AddKey(const Coo::Math::Vector3& position, const Coo::Math::Quaternion& rotation, const Coo::Math::Vector3& scale, float time)
{
	mAnimation.mPositionKeyframes.emplace_back(Keyframe<Vector3>{position, time});
	mAnimation.mRotationKeyframes.emplace_back(Keyframe<Quaternion>{rotation, time});
	mAnimation.mScaleKeyframes.emplace_back(Keyframe<Vector3>{scale, time});
	return *this;
}

Animation Coo::Graphics::AnimationBuilder::Build()
{
	return mAnimation;
}
