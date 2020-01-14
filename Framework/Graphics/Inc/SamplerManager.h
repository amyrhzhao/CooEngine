#ifndef INCLUDED_COOENGINE_GRAPHICS_SAMPLERMANAGER_H
#define INCLUDED_COOENGINE_GRAPHICS_SAMPLERMANAGER_H

#include "Sampler.h"

namespace Coo::Graphics
{
class SamplerManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static SamplerManager* Get();

	SamplerManager() = default;
	~SamplerManager();

	void Initialize();
	void Terminate();

	bool AddSampler(std::string name, Sampler::Filter filter, Sampler::AddressMode addressMode);
	Sampler* GetSampler(std::string name);

private:
	std::unordered_map<std::string, std::unique_ptr<Sampler>> mInventory;
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_SAMPLERMANAGER_H
