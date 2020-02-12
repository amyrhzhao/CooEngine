#ifndef INCLUDED_COOENGINE_TRANSFORMCOMPONENT_H
#define INCLUDED_COOENGINE_TRANSFORMCOMPONENT_H

#include "Component.h"

namespace Coo 
{
	class TransformComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		Math::Vector3 position;
		Math::Quaternion rotation;
	};
} // namespace Coo

#endif // !INCLUDED_COOENGINE_TRANSFORMCOMPONENT_H
