#include "Precompiled.h"
#include "MetaRegistration.h"

#include "Service.h"
#include "GameObject.h"
#include "Component.h"

#include "ColliderComponent.h"
#include "TransformComponent.h"

#include "CameraService.h"
#include "TerrainService.h"

void Coo::StaticRegister()
{
	META_REGISTER(Coo::GameObject);
	META_REGISTER(Coo::Component);
	META_REGISTER(Coo::Service);

	META_REGISTER(Coo::ColliderComponent);
	META_REGISTER(Coo::TransformComponent);
	META_REGISTER(Coo::CameraService);
	META_REGISTER(Coo::TerrainService);
}
