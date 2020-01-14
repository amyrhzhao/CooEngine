#ifndef INCLUDED_COOENGINE_BLENDSTATEMANAGER_H
#define INCLUDED_COOENGINE_BLENDSTATEMANAGER_H

#include "BlendState.h"

namespace Coo::Graphics 
{

namespace Blend 
{
	const std::string Opaque = "Opaque";
	const std::string AlphaBlend = "AlphaBlend";
	const std::string AlphaPremultiplied = "AlphaPremultiplied";
	const std::string Additive = "Additive";
}

class BlendStateManager 
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static BlendStateManager* Get();

	BlendStateManager() = default;
	~BlendStateManager();

	void Initialize();
	void Terminate();

	bool AddBlendState(std::string name, BlendState::Mode mode);
	BlendState* GetBlendState(std::string name);

private:
	std::unordered_map<std::string, std::unique_ptr<BlendState>> mInventory;

};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_BLENDSTATEMANAGER_H
