#pragma once
#include "Core/Application.h"

#ifdef HZR_PLATFORM_WINDOWS
	
	//Do be defined in client
	extern Hazard::Application* Hazard::CreateApplication();

	int main() {
		//Create application and run
		Hazard::Application* app = Hazard::CreateApplication();
		app->Run();
		delete app;
	}

#else 
	#error NOT SUPPORTED PLATFORM
#endif // HZR_PLATFORM_WINDOWS
