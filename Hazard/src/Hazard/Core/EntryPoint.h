#pragma once

#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "Hazard/Core/HazardLoop.h"

#include <stdio.h>

extern Hazard::Application* Hazard::CreateApplication();

#ifdef HZR_PLATFORM_WINDOWS

	#ifdef HZR_DEBUG

		#pragma comment( linker, "/subsystem:console" )
		int main()
		{
			using namespace Hazard;
			Application* app = CreateApplication();
			Core::HazardLoop loop(app);
			loop.Start();
		}

	#else
		#pragma comment( linker, "/subsystem:windows" )
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
		{
			Hazard::Application* app = Hazard::CreateApplication();
			Hazard::Core::HazardLoop loop(app);
			loop.Start();
		}
	#endif // HZR_DEBUG

#else
	#error Unsupported platform for Hazard
#endif // HZR_PLATFORM_WINDOWS