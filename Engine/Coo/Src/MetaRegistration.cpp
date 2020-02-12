#include "Precompiled.h"
#include "MetaRegistration.h"

#include "GameObject.h"
#include "Component.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"

void Coo::StaticRegister()
{
	META_REGISTER(Coo::GameObject);
	META_REGISTER(Coo::Component);

	META_REGISTER(Coo::ColliderComponent);
	META_REGISTER(Coo::TransformComponent);
}
