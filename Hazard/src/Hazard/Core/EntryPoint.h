#pragma once

#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "Hazard/Core/HazardLoop.h"

#include <stdio.h>

extern Hazard::Application* Hazard::CreateApplication();

#ifdef HZR_PLATFORM_WINDOWS

	#if defined(HZR_DEBUG) || defined(HZR_GAME_ONLY)
		//Appliction starts as console-app
		#pragma comment( linker, "/subsystem:console" )
		int main()
		{
			using namespace Hazard;
			//Create Hazard application from external source and start running
			Application* app = CreateApplication();
			Core::HazardLoop loop(app);
			loop.Start();
		}

	#else
		//Gets rid of the console window for release mode
		#pragma comment( linker, "/subsystem:windows" )
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
		{
			//Create Hazard application from external source and start running
			Hazard::Application* app = Hazard::CreateApplication();
			Hazard::Core::HazardLoop loop(app);
			loop.Start();
		}
	#endif // HZR_DEBUG

#else
	#error Unsupported platform for Hazard
#endif // HZR_PLATFORM_WINDOWS