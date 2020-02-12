#ifndef INCLUDED_COO_COLLIDER_COMPONENT_H
#define INCLUDED_COO_COLLIDER_COMPONENT_H

#include "Component.h"

namespace Coo
{
	class ColliderComponent : public Component
	{
	public:
		META_CLASS_DECLARE

		void Initialize();
		void Render();

		Math::Vector3 center;
		Math::Vector3 extend;
	};
}

#endif // !INCLUDED_COO_COLLIDER_COMPONENT_H
