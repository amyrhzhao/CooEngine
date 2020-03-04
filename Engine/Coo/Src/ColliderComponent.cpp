#include "Precompiled.h"
#include "ColliderComponent.h"

using namespace Coo;

META_DERIVED_BEGIN(Coo::ColliderComponent, Coo::Component)
	META_FIELD_BEGIN
	META_FIELD(center, "Center")
	META_FIELD(extend, "Extend")
	META_FIELD_END
META_CLASS_END

void Coo::ColliderComponent::Initialize()
{
}

void Coo::ColliderComponent::Render()
{
	
}
