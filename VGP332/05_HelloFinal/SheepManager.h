#pragma once

#include "Sheep.h"

class SheepManager 
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static SheepManager* Get();

	void Initialize(Coo::AI::AIWorld& world);
	void Terminate();
	void Render() const;
	void Update(float deltaTime);

private:
	std::vector<std::unique_ptr<Sheep>> sheeps;
	int sheepCount = 10;
};