#include "Precompiled.h"
#include "AIWorld.h"
#include "Entity.h"

static int gID = 0;

Coo::AI::Entity::Entity(AIWorld & world, int typeId) : world(world), id(gID++)
{
	world.RegisterEntity(this);
	Entity::typeId = typeId;
}

Coo::AI::Entity::~Entity()
{
	world.UnregisterEntity(this);
}
