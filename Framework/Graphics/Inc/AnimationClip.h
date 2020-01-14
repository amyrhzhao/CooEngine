#ifndef INCLUDED_COOENGINE_GRAPHICS_ANIMATIONCLIP_H
#define INCLUDED_COOENGINE_GRAPHICS_ANIMATIONCLIP_H

#include "Animation.h"

namespace Coo::Graphics
{
	class AnimationClip
	{
	public:
		bool GetTransform(float time, uint32_t boneIndex, Math::Matrix4& transform) const;
		void DuiShowInfo() const;

		std::string name;
		float duration = 0.0f;
		float ticksPersecond = 0.0f;
		BoneAnimations boneAnimations;
	};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_ANIMATIONCLIP_H
