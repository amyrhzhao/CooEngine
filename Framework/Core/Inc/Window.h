#ifndef INCLUDED_COOENGINE_CORE_WINDOW_H
#define INCLUDED_COOENGINE_CORE_WINDOW_H

namespace Coo::Core {

class Window
{
public:
	Window() = default;

	void Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height);
	void Terminate();

	bool ProcessMessage();

	HWND GetWindowHandle() const { return mWindow; }

private:
	HINSTANCE mInstance{ nullptr };
	HWND mWindow{ nullptr };
	std::string mAppName;
};

} //namespace Coo::Core


#endif // !INCLUDED_COOENGINE_CORE_WINDOW_H
