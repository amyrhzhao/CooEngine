#include "GameState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	CooApp::AddState<GameState>("GameState");
	CooApp::Run({ "Hello Steering","../../Assets/", 1280, 720 });
}