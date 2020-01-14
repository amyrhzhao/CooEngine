#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace Coo::AI;

PerceptionModule::PerceptionModule(Agent & agent, ImportanceCalculator importanceCalculator)
	: mAgent(agent)
	, mImportanceCalculator(importanceCalculator)
{

}

PerceptionModule::~PerceptionModule()
{
}

void PerceptionModule::Update(float deltaTime)
{
	// Update sensors and refresh/add memory records
	for (auto&[name, sensor] : mSensors)
	{
		sensor->Update(mAgent, mMemory, deltaTime);
	}

	// Remove any record older than memory span
	for (auto iter = mMemory.begin(); iter != mMemory.end();)
	{
		iter->lastRecordedTime += deltaTime;
		if (iter->lastRecordedTime > mMemorySpan)
		{
			iter = mMemory.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// Calculator importance on new records
	for (auto m : mMemory)
	{
		mImportanceCalculator(m);
	}

	// Sort records by importance
	mMemory.sort([](MemoryRecord& a, MemoryRecord& b)
	{
		return a.importance > b.importance;
	});
}
