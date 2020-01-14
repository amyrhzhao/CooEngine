#ifndef INCLUDED_COOENGINE_CORE_WINDOWMESSAGEHANDLER_H
#define INCLUDED_COOENGINE_CORE_WINDOWMESSAGEHANDLER_H

namespace Coo::Core {

class WindowMessageHandler 
{
public:
	using Callback = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

	void Hook(HWND window, Callback cb);
	void Unhook();

	LRESULT ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND mWindow{ nullptr };
	Callback mPreviousCallback{ nullptr };
};

} // namespace Coo::Core

#endif // !INCLUDED_COOENGINE_CORE_WINDOWMESSAGEHANDLER_H
