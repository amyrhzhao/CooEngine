#ifndef INCLUDED_COOENGINE_AI_PERCEPTIONMODULE_H
#define INCLUDED_COOENGINE_AI_PERCEPTIONMODULE_H

#include "MemoryRecord.h"
#include "Sensor.h"

namespace Coo::AI 
{
class Agent;

using ImportanceCalculator = std::function<void(MemoryRecord&)>;

class PerceptionModule 
{
public:
	PerceptionModule(Agent& agent, ImportanceCalculator importanceCalculator);
	~PerceptionModule();

	template <class SensorType>
	SensorType* AddSensor(std::string name);

	template <class SensorType>
	SensorType* GetSensor(const std::string& name);

	void Update(float deltaTime);

	const MemoryRecords& GetMemoryRecords() const { return mMemory; }

private:
	using SensorMap = std::unordered_map<std::string, std::unique_ptr<Sensor>>;

	Agent& mAgent;
	SensorMap mSensors;
	ImportanceCalculator mImportanceCalculator;
	MemoryRecords mMemory;
	float mMemorySpan = 2.0f; // how long does agent remember
};

template<class SensorType>
inline SensorType * PerceptionModule::AddSensor(std::string name)
{
	auto[iter, success] = mSensors.try_emplace(std::move(name), std::make_unique<SensorType>());
	return success ? static_cast<SensorType*>(iter->second.get()) : nullptr;
}

template<class SensorType>
inline SensorType * PerceptionModule::GetSensor(const std::string& name)
{
	auto iter = mSensors.find(name);
	return iter == mSensors.end() ? nullptr : static_cast<SensorType*>(iter->second.get());
}

} // namespace Coo::AI

#endif // !INCLUDED_COOENGINE_AI_PERCEPTIONMODULE_H
