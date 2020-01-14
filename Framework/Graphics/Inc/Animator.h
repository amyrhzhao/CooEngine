#ifndef INCLUDED_COOENGINE_GRAPHICS_ANIMATOR
#define INCLUDED_COOENGINE_GRAPHICS_ANIMATOR

namespace Coo::Graphics
{
	class AnimationClip;
	class AnimationBank;

	class Animator
	{
	public:
		void PlayAnimationClip(int clip);
		void UpdateAnimation(float deltaTime);
		bool GetBoneTransform(size_t index, Coo::Math::Matrix4& transform) const;
		void Bind(AnimationBank* animationBank);
		void DebugUI();

	private:
		int animationClip = -1;
		AnimationBank* mAnimationBank = nullptr;
		bool loop = true;
		float timer = 0.0f;
		float paused = false;
		float playSpeed = 1.0f;
		const AnimationClip* GetCurrAnimationClip(bool playing = true) const;
	};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_ANIMATOR
