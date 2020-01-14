#ifndef INCLUDED_COOENGINE_RASTERIZERSTATEMANAGER_H
#define INCLUDED_COOENGINE_RASTERIZERSTATEMANAGER_H

#include "RasterizerState.h"

namespace Coo::Graphics 
{
class RasterizerStateManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static RasterizerStateManager* Get();

	RasterizerStateManager() = default;
	~RasterizerStateManager();

	void Initialize();
	void Terminate();

	bool AddRasterizerState(std::string name, RasterizerState::CullMode cullMode, RasterizerState::FillMode fillMode);
	RasterizerState* GetRasterizerState(std::string name);

private:
	std::unordered_map<std::string, std::unique_ptr<RasterizerState>> mInventory;

};
} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_RASTERIZERSTATEMANAGER_H
