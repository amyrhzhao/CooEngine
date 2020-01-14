#ifndef INCLUDED_COOENGINE_GRAPHICS_ANIMATION_H
#define INCLUDED_COOENGINE_GRAPHICS_ANIMATION_H

#include "Math/Inc/CooMath.h"
#include "Keyframe.h"
namespace Coo::Graphics
{

	class Animation
	{
	public:
		Math::Matrix4 GetTransform(float time) const;
		Math::Matrix4 GetPositionTransform(float time) const;
		Math::Matrix4 GetRotationTransform(float time) const;
		Math::Matrix4 GetScaleTransform(float time) const;

	private:
		friend class AnimationBuilder;
		friend class AnimationBank;
		friend class Animator;

		std::vector<Keyframe<Math::Vector3>> mPositionKeyframes;
		std::vector<Keyframe<Math::Quaternion>> mRotationKeyframes;
		std::vector<Keyframe<Math::Vector3>> mScaleKeyframes;
	};

	class BoneAnimation
	{
	public:
		std::string boneName;
		Animation animation;
	};

	using BoneAnimations = std::vector<std::unique_ptr<BoneAnimation>>;

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_ANIMATION_H
