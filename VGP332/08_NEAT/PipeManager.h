#pragma once

#include "Pipe.h"

class Bird;

class PipeManager
{
public:
	void Update(float deltaTime);
	void Render();

	void Reset();

	bool Intersect(const Bird& bird) const;
	const Pipe* GetClosestPipe(const Bird& bird) const;

private:
	std::vector<Pipe> mPipes;
	float mSpawnDelay = 0.0f;
};