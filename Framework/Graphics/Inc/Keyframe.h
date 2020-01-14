#ifndef INCLUDED_COOENGINE_GRAPHICS_KEYFRAME_H
#define INCLUDED_COOENGINE_GRAPHICS_KEYFRAME_H

namespace Coo::Graphics 
{

template <class T>
struct Keyframe
{
	T value;
	float time;
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_KEYFRAME_H
