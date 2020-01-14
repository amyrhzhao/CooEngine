#ifndef INCLUDED_COOENGINE_APP_H
#define INCLUDED_COOENGINE_APP_H

namespace Coo
{
class AppState;

struct AppConfig 
{
	AppConfig() = default;
	AppConfig(std::string _appName, std::filesystem::path _assetsDirectory, uint32_t _windowWidth, uint32_t _windowHeight) 
		: appName(_appName), assetsDirectory(_assetsDirectory), windowWidth(_windowWidth), windowHeight(_windowHeight) {}
	AppConfig(std::string _appName)
		: appName(_appName) {}

	std::string appName = "Coo";
	std::filesystem::path assetsDirectory = L"../../Assets";
	uint32_t windowWidth = 1280;
	uint32_t windowHeight = 720;
};

class App
{
public:
	template <typename StateType>
	void AddState(std::string name);
	void ChangeState(std::string name);

	void Run(AppConfig appConfig);

private:
	AppConfig mAppConfig;
	Core::Window mWindow;
	std::map<std::string, std::unique_ptr<AppState>> mAppStates;
	AppState* mCurrentState = nullptr;
	AppState* mNextState = nullptr;
};

template<typename StateType>
void App::AddState(std::string name)
{
	mAppStates.emplace(std::move(name), std::make_unique<StateType>());
}

} // namespace Coo

#endif // !INCLUDED_COOENGINE_APP_H
