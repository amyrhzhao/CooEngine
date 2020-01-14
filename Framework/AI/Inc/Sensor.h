#ifndef INCLUDED_COOENGINE_AI_SENSOR_H
#define INCLUDED_COOENGINE_AI_SENSOR_H

#include "MemoryRecord.h"

namespace Coo::AI 
{
class Agent;

class Sensor 
{
public:
	virtual ~Sensor() = default;
	virtual void Update(Agent& agent, MemoryRecords& memory, float deltaTime) = 0;
};

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_SENSOR_H
