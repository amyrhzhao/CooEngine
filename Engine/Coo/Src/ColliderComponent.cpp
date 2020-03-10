#include "Precompiled.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
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
	auto transform = mOwner->GetComponent<Coo::TransformComponent>();
	if (transform) 
	{
		Graphics::SimpleDraw::AddAABB(transform->position + center, extend, Graphics::Colors::Cyan);
	}
}
