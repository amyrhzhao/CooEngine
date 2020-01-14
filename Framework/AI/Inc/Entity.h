#ifndef INCLUDED_COOENGINE_AI_ENTITY_H
#define INCLUDED_COOENGINE_AI_ENTITY_H

namespace Coo::AI 
{
class AIWorld;

class Entity;
using EntityList = std::vector<Entity*>;

class Entity
{
public:
	Entity(AIWorld& world, int typeId = 0);
	~Entity();

	Math::Vector2 position = {};
	float radius = 1.0f;
	int id;
	int typeId;
	AIWorld& world;
};

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_ENTITY_H
