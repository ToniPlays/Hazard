#pragma once
#include "HazardLoop.h"
#include "Hazard/Core/Application.h"

#ifdef HZR_PLATFORM_WINDOWS
	
	extern Hazard::Application* Hazard::CreateApplication();

	int main() {

		Hazard::Application* app = Hazard::CreateApplication();
		Hazard::HazardLoop* hazardLoop = new Hazard::HazardLoop(app);
		hazardLoop->Run();
		delete hazardLoop;
	}

#else
#error Unsupported platform
#endif // HZR_PLATFORM_WINDOWS