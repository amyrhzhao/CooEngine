#ifndef INCLUDED_COOENGINE_GRAPHICS_ANIMATIONBANK_H
#define INCLUDED_COOENGINE_GRAPHICS_ANIMATIONBANK_H

namespace Coo::Graphics 
{
	class AnimationClip;

	class AnimationBank 
	{
	public:
		bool Load(std::string fileName, bool useFileName = false);
		bool Save(std::string fileName);

		//void Initialize();
		//void Terminate();

		//void Append(std::vector<std::unique_ptr<Coo::Graphics::AnimationClip>>& anim);

	public:
		std::vector<std::unique_ptr<Coo::Graphics::AnimationClip>> animationClips;
	};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_ANIMATIONBANK_H

