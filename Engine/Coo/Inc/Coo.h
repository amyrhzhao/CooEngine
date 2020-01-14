#ifndef INCLUDED_COO_H
#define INCLUDED_COO_H

#include "Common.h"

#include "App.h"
#include "AppState.h"

namespace Coo
{
	extern App sApp;

} // namespace Coo

namespace CooApp
{
	template<class T>
	void AddState(std::string name)
	{
		Coo::sApp.AddState<T>(name);
	}

	void ChangeState(std::string name);
	void Run(Coo::AppConfig appConfig);
	void ShutDown();

} // namespace CooApp


#endif // !INCLUDED_COO_H
