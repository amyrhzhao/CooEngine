#include "Precompiled.h"
#include "Coo.h"

	Coo::App Coo::sApp;

	void CooApp::ChangeState(std::string name)
	{
		Coo::sApp.ChangeState(std::move(name));
	}

	void CooApp::Run(Coo::AppConfig appConfig)
	{
		Coo::sApp.Run(std::move(appConfig));
	}

	void CooApp::ShutDown()
	{
		PostQuitMessage(0);
	}
