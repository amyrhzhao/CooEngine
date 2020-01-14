#ifndef INCLUDED_COOENGINE_APPSTATE_H
#define INCLUDED_COOENGINE_APPSTATE_H

namespace Coo
{
class AppState
{
public:
	virtual void Initialize() = 0;
	virtual void Terminate() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void DebugUI() = 0;
};

} // namespace Coo

#endif // !INCLUDED_COOENGINE_APPSTATE_H
