#include "EditState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Add maximize and docking
	CooApp::AddState<EditState>("EditState");
	Coo::AppConfig config{ "Coo Editor","../../Assets/", 1280, 720 };
	config.maximize = true;
	config.imGuiDocking = true;
	CooApp::Run(config);

}