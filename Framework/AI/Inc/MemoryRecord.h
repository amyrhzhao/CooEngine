#ifndef INCLUDED_COOENGINE_AI_MEMORYRECORD_H
#define INCLUDED_COOENGINE_AI_MEMORYRECORD_H

#include "Common.h"

namespace Coo::AI 
{
	using Property = std::variant<int, float, Coo::Math::Vector2>;

struct MemoryRecord
{
	std::unordered_map<std::string, Property> properties;
	float lastRecordedTime = 0.0f;
	float importance = 0.0f;
};

using MemoryRecords = std::list<MemoryRecord>;
} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_MEMORYRECORD_H
