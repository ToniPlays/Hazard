#pragma once
#include "Core/Application.h"
#define ENTRY_H

#ifdef HZR_PLATFORM_WINDOWS

extern Hazard::Application* Hazard::CreateApplication();

int main() {

	Hazard::Application* app = Hazard::CreateApplication();
	app->Run();
	delete app;
}

#else 
	#error NOT SUPPORTED PLATFORM
#endif // HZR_PLATFORM_WINDOWS