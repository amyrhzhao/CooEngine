#ifndef INCLUDED_COOENGINE_GRAPHICS_ANIMATOR
#define INCLUDED_COOENGINE_GRAPHICS_ANIMATOR

namespace Coo::Graphics
{
	class AnimationClip;
	class AnimationBank;

	class Animator
	{
	public:
		void PlayAnimationClip(int clip, bool playFromBegining = true);
		void AddTransition(int clip, float exitTime, float transitionDuration);
		void UpdateAnimation(float deltaTime);
		bool GetBoneTransform(size_t index, Coo::Math::Matrix4& transform) const;
		void Bind(AnimationBank* animationBank);
		void DebugUI();

	private:
		int mCurrAnimationClip = -1;
		AnimationBank* mAnimationBank = nullptr;
		float mPaused = false;
		
		std::vector<bool> mIsLoop;
		std::vector<float> mTimers;
		std::vector<float> mPlaySpeed;
		
		const AnimationClip* GetCurrAnimationClip(bool playing = true) const;
	
		// Transition
		bool mTransiting = false;
		bool mInTransition = false;
		int mNextAnimationClip = 0;
		float mExitTime = 0.9f;
		float mTransitionDuration = 0.1f;
		float mTransitionDurationPassed = 0.0f;
		float mCurrTransitionDuration;
	};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_ANIMATOR
