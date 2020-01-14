#ifndef INCLUDED_COOENGINE_GRAPHICS_ANIMATIONBUILDER_H
#define INCLUDED_COOENGINE_GRAPHICS_ANIMATIONBUILDER_H

namespace Coo::Graphics 
{

class AnimationBuilder 
{
public:
	AnimationBuilder& AddPositionKey(const Coo::Math::Vector3& position, float time);
	AnimationBuilder& AddRotationKey(const Coo::Math::Quaternion& rotation, float time);
	AnimationBuilder& AddScaleKey(const Coo::Math::Vector3& scale, float time);
	AnimationBuilder& AddKey(const Coo::Math::Vector3& position, const Coo::Math::Quaternion& rotation, const Coo::Math::Vector3& scale, float time);
	
	Animation Build();

private:
	Animation mAnimation;
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_ANIMATIONBUILDER_H
