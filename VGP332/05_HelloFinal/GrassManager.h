#pragma once

#include "Grass.h"

class GrassManager 
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static GrassManager* Get();

	void Initialize(Coo::AI::AIWorld& world);
	void Terminate();
	void Update();
	void Render() const;

	int GrassCount() const { return static_cast<int>(grasses.size()); }
	Grass* GetGrass(int index) { return grasses[index].get(); }

private:
	std::vector<std::unique_ptr<Grass>> grasses;
	int grassCount = 10;
};